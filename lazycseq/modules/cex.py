""" CSeq C Sequentialization Framework
	counterexample translation module

	written by Omar Inverso, University of Southampton.
"""
VERSION = 'cex-2015.07.16'
#VERSION = 'cex-2015.07.14'
#VERSION = 'cex-2015.07.07'
"""
The counterexample generation implemented in this module 
is specific for the Lazy schema (lazyseq.py) but most of it should in principle work
for any translation where the relationship betweeen the input of the first module and the output of the last module
is a points-to-set function, or equivalently,
the linemap from the output of the last module to the input of the first module
is a surjective (and therefore invertible) function.

TODO:
	- map thread creation and thread join using similar way for mutexes
	- pthread_cond_wait() cex entry does not show lock parameter

Changelog:
	2015.07.16  improved cbmc error trace readability by showing more simulated pthread states (Omar,Truc)
	2015.07.15  fixed line mapping for mutex destroy (Truc)
	2015.07.07  1st version
"""
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.module, core.parser, core.utils
import sys
import time

# Expressions to check for from the log to see whether the verification went fine.
verificationOK = {}
verificationOK['esbmc'] = 'VERIFICATION SUCCESSFUL'
verificationOK['cbmc'] = 'VERIFICATION SUCCESSFUL'
verificationOK['blitz'] = 'VERIFICATION SUCCESSFUL'
verificationOK['llbmc'] = 'No error detected.'
verificationOK['cpachecker'] = 'Verification result: TRUE.'
verificationOK['smack'] = 'Finished with 1 verified, 0 errors'
verificationOK['satabs'] = 'VERIFICATION SUCCESSFUL'
verificationOK['klee'] = 'DKJFHSDKJDFHSJKF' # no such thing for Klee?

# Expressions to check for from the log to see whether the verification failed.
verificationFAIL = {}
verificationFAIL['esbmc'] = 'VERIFICATION FAILED'
verificationFAIL['cbmc'] = 'VERIFICATION FAILED'
verificationFAIL['blitz'] = 'VERIFICATION FAILED'
verificationFAIL['llbmc'] = 'Error detected.'
verificationFAIL['cpachecker'] = 'Verification result: FALSE.' #verificationFAIL['smack'] = 'Error BP5001: This assertion might not hold.\n'
verificationFAIL['smack'] = 'Finished with 0 verified,'
verificationFAIL['satabs'] = 'VERIFICATION FAILED'
verificationFAIL['klee'] = 'ASSERTION FAIL: '

# Backend reject/error.
backendERROR = {}
backendERROR['cbmc'] = 'CONVERSION ERROR'


class cex(core.module.BasicModule):
	debug = True

	def init(self):
		self.addInputParam('backend', 'backend', 'b', default='cbmc', optional=False)
		self.addInputParam('linemap', 'show linemap', '', default=False, optional=True)
		self.addInputParam('cex', 'show counterexample (cbmc only)', '', default=False, optional=True)
		self.addInputParam('exitcode', 'backend exit-code', '', default=0, optional=True)
		self.addInputParam('threadnamesmap', 'map from thread copies to thread function', '', default=None, optional=True)
		self.addInputParam('threadindexes', 'map from thread copies to thread indexes', '', default=None, optional=True)
		self.addInputParam('threadindextoname', 'map from thread index to thread function name', '', default=None, optional=True)
		self.addInputParam('varnamesmap', 'map for replaced variable names', '', default=None, optional=True)
		self.addInputParam('coordstofunctions', 'map from input coords to function ids', '', default=None, optional=True)


	def loadfromstring(self, string, env):
		self.env = env
		self.backend = self.getInputParamValue('backend')
		self.code = self.getInputParamValue('exitcode')
		self.threadnamesmap = self.getInputParamValue('threadnamesmap')
		self.threadindexes = self.getInputParamValue('threadindexes')
		self.threadindextoname = self.getInputParamValue('threadindextoname')
		self.varnamesmap = self.getInputParamValue('varnamesmap')
		self.coordstofunctions = self.getInputParamValue('coordstofunctions')
		self.outputtofiles = self.env.outputtofiles


		if self.getInputParamValue('linemap') is not None:
			print "Line mapping:\n"
			self._showfullmapback()
			print ''

		'''
		self._showfullmapback()
		for x in self.coordstofunctions: print "line:%s file:%s function:%s " % (x[0],x[1],self.coordstofunctions[x])
		print str(self.threadindextoname)
		print str(self.coordstofunctions)
		for x in self.outputtofiles: print ("%s -> %s" % (x, self.outputtofiles[x]))
		sys.exit(1)
		'''

		if self.getInputParamValue('cex') is None:
			self.output = self._shortanswer(string)
		else:
			self.output = self._translateCPROVERcex(string)
			self.output += self._shortanswer(string)


	def _showfullmapback(self):
		# Note: since the same input line may correspond to
		#       multiple lines in the final output,
		#       the tracing has to be done backwards.
		#
		lastmodule = len(self.env.maps)
		nextkey = 0
		inputfile = ''

		#print "LAST MODULE: %s" % lastmodule
		#print "LAST LINE IN LAST MODULE: %s" % self.env.lastlinenoinlastmodule

		additionalspace = 2
		symbolspace = '.'

		for lineno in range(1,self.env.lastlinenoinlastmodule):
			lastmodule = len(self.cseqenv.maps)
			nextkey = 0
			inputfile = ''

			if self.cseqenv.maps[len(self.cseqenv.maps)-1].has_key(lineno):
				firstkey = nextkey = lastkey = lineno

				printfirstkey = symbolspace*(additionalspace+len(str(max(self.cseqenv.maps[len(self.cseqenv.maps)-1]))) - len(str(firstkey)))+str(firstkey)

				buff = ''
				buff +="%s" % printfirstkey

				for modno in reversed(range(0,lastmodule)):
					if nextkey in self.cseqenv.maps[modno] and nextkey != 0:
						lastkey = nextkey
						nextkey = self.cseqenv.maps[modno][nextkey]

						printnextkey = symbolspace*(additionalspace+len(str(max(self.cseqenv.maps[modno]))) - len(str(nextkey)))+str(nextkey)

						buff+="%s" % printnextkey
					else:
						nextkey = 0

					if modno == 0 and lastkey in self.cseqenv.outputtofiles:
						inputfile = self.cseqenv.outputtofiles[lastkey]
						buff +=" %s" %inputfile


			if not buff.endswith('_fake_typedefs.h') and (buff.endswith('.c') or buff.endswith('.h')): print buff


	''' Short interpretation of the backend answer.
	'''
	def _shortanswer(self,input):
		outcome = ''
		mem = maxmem = cpu = stale = variables = clauses = 0

		'''
		for line in input:  # variables and clauses extraction
			if ' variables, ' in line:
				splitline = line.split()
				variables = splitline[0]
				clauses = splitline[2]
		'''

		# scan the backend's output to check the outcome of the verification
		for line in input.splitlines():
			if verificationFAIL[self.backend] in line:
				outcome = 'UNSAFE'
				break
			elif verificationOK[self.backend] in line:
				outcome = 'SAFE'
				break
			elif self.backend == 'cbmc' and self.code == 6:
				outcome = 'BACKENDREJECT'

		if outcome == '' and self.code == -9: outcome = 'TIMEOUT' # backend timeout
		elif outcome == '': outcome = 'UNKNOWN'

		#
		if outcome == 'UNKNOWN': result = core.utils.colors.YELLOW + outcome + core.utils.colors.NO
		elif outcome == 'BACKENDREJECT': result = core.utils.colors.BLUE + outcome + core.utils.colors.NO
		elif outcome == 'TIMEOUT': result = core.utils.colors.YELLOW + outcome + core.utils.colors.NO
		elif outcome == 'SAFE': result = core.utils.colors.GREEN + outcome + core.utils.colors.NO
		elif outcome == 'UNSAFE': result = core.utils.colors.RED + outcome + core.utils.colors.NO

		return "%s, %s, %0.2f" % (self.env.inputfile,result,time.time()-self.env.starttime)


	''' Full counterexample translation for CBMC
	   (and many tools based on the CPROVER framework)
	'''
	def _translateCPROVERcex(self,cex):
		if self.backend != 'cbmc':
			self.warn('error trace translation for backend %s not supported.' % self.backend)
			return ''


		translatedcex = ''
		lines = cex.split('\n')
		k = cex[:cex.find('Counterexample:')].count('\n')+1+1
		separator = "----------------------------------------------------"

		while k<len(lines):
			# case 1: another transition to fetch
			if lines[k].startswith('State ') and lines[k+1] == separator:
				A,B,C = lines[k],lines[k+1],lines[k+2]

				# the part below the separator might be
				# more than one line long..
				j=1
				while ( k+2+j<len(lines) and
					not lines[k+2+j].startswith('State ') and
					not lines[k+2+j].startswith('Violated property') ):
					C+=lines[k+2+j]
					j+=1

				X,Y,Z = self._mapCPROVERstate(A,B,C)

				if X != '':
					translatedcex += '%s\n' % X
					if Y != '': translatedcex += '%s\n' % Y
					if Z != '': translatedcex += '%s\n' % Z
					translatedcex += '\n'
			# case 2: final transation with property violation
			elif lines[k].startswith('Violated property'):
				Y,Z,W = self._mapCPROVERendstate(lines[k+1],lines[k+2],lines[k+3])

				translatedcex += 'Violated property:\n%s\n%s\n%s\n' % (Y,Z,W)
				translatedcex += '\nVERIFICATION FAILED'

			k+=1

		if len(translatedcex) > 0:
			translatedcex = "Counterexample:\n\n" + translatedcex + '\n\n'

		return translatedcex


	''' Returns the coords of the original input file
		in the format (line,file)
		corresponding to the given output line number, or
		(?,?) if unable to map back.
	'''
	def sbizz(self,lineno):
		nextkey = 0
		inputfile = ''

		lastmodule = len(self.env.maps)

		if self.env.maps[len(self.env.maps)-1].has_key(lineno):
			firstkey = nextkey = lastkey = lineno

			for modno in reversed(range(0,lastmodule)):
				if nextkey in self.env.maps[modno] and nextkey != 0:
					lastkey = nextkey
					nextkey = self.env.maps[modno][nextkey]
				else:
					nextkey = 0

				if nextkey!=0 and modno == 0 and lastkey in self.outputtofiles:
					inputfile = self.env.outputtofiles[lastkey]

		if nextkey == 0: nextkey = '?'
		if inputfile == '': inputfile = '?'

		return (nextkey, inputfile)

	__lastthreadID = ''

	def _mapCPROVERstate(self,A,B,C,showbinaryencoding=False):
		Aout = Bout = Cout = ''
		keys = {}

		# Fetch values.
		try:
			# 1st line
			tokens = A.split()

			for key,value in zip(tokens[0::2],tokens[1::2]):
				keys[key] = value

			stateout = keys['State']

			# 3rd line
			line3 = C.strip()
			lvalue = line3[:line3.find('=')]
			rvalue = line3[len(lvalue)+1:]

			# double-check parsing correctness
			if 'function' in keys: Aout = "State %s file %s line %s function %s thread %s" % (keys['State'],keys['file'],keys['line'],keys['function'],keys['thread'])
			else: Aout = "State %s file %s line %s thread %s" % (keys['State'],keys['file'],keys['line'],keys['thread'])
			Cout = "  %s=%s" % (lvalue,rvalue)

			if A != Aout or C != Cout:
				self.warn('unable to parse counterexample state %s' % keys['State'])
				return ('','','')
		except Exception as e:
			self.warn('unable to parse counterexample state')
			return ('','','')

		# Special case: context switching.
		if lvalue.startswith('__cs_thread_index') and 'function' in keys and keys['function'] != '':
			threadout = rvalue[:rvalue.find(' ')]
			threadindexout = ''
			self.__lastthreadID = threadout
			if int(threadout) in self.threadindextoname: threadindexout = self.threadindextoname[int(threadout)]
			Aout = "State %s" % (stateout)
			Cout = "  thread %s (%s) scheduled" % (threadout,threadindexout)
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: thread creation
		if lvalue == '__cs_threadID':
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))
			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_create(thread %s)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: cond signal
		if lvalue == '__cs_cond_to_signal':
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))
			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_cond_signal(%s)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: cond wait
		if lvalue == '__cs_cond_to_wait_for':
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))
			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_cond_wait(%s,?)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: mutexes lock and unlock
		if lvalue == '__cs_mutex_to_lock' and 'function' in keys and not keys['function']=='__cs_cond_wait_2':
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))
			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_mutex_lock(%s)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		if lvalue == '__cs_mutex_to_unlock' and 'function' in keys and not keys['function']=='__cs_cond_wait_1' :
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))

			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_mutex_unlock(%s)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: mutexes destroy
		if lvalue == '__cs_mutex_to_destroy':
			threadout = ''
			threadindexout = ''
			fileout = ''
			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))
			Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,self.__lastthreadID)
			Cout = '  pthread_mutex_destroy(%s)' % (rvalue[:rvalue.find(' (')])
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# Special case: explicit __CSEQ_message().
		if lvalue== '__cs_message':
			threadout = ''
			threadindexout = ''

			if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))

			if 'function' in keys:
				#if keys['function'] in self.threadindexes: threadout = self.threadindexes[keys['function']]
				if keys['function'] in self.threadnamesmap: functionout = self.threadnamesmap[keys['function']]

			if 'function' in keys:
				if keys['function'] in self.threadindexes: threadout = self.threadindexes[keys['function']]
				#if keys['function'] in self.threadnamesmap: functionout = self.threadnamesmap[keys['function']]

			message = rvalue[:rvalue.find(' (')][1:-1]
			Aout = "State %s thread %s" % (stateout,self.__lastthreadID)
			Cout = '  '+message
			return Aout,"- - - - - - - - - - - - - - - - - - - - - - - - - - ",Cout

		# State mapping for the lazy schema,
		# general case.
		fileout = functionout = ''
		lineout = 0
		# Truc -- dirty fix
		threadout = -1

		if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))

		if 'function' in keys:
			if keys['function'] in self.threadindexes: threadout = self.threadindexes[keys['function']]
			if keys['function'] in self.threadnamesmap: functionout = self.threadnamesmap[keys['function']]

		# Truc
		if threadout == -1:    # Cannot find the thread id from line map
			threadout = self.__lastthreadID

		if self.coordstofunctions is not None and (lineout,fileout) in self.coordstofunctions: functionout = self.coordstofunctions[lineout,fileout]

		if lvalue in self.varnamesmap: lvalue = self.varnamesmap[lvalue]

		rightvar = rvalue[:rvalue.rfind(' (')]

		if rightvar[0] != '&' and rightvar in self.varnamesmap: rvalue = rvalue.replace(rightvar,self.varnamesmap[rightvar],1)
		elif rightvar[0] == '&' and rightvar[1:] in self.varnamesmap: rvalue = '&'+rvalue.replace(rightvar,self.varnamesmap[rightvar[1:]],1)

		if not showbinaryencoding: rvalue = rvalue[:rvalue.rfind(' (')]

		if functionout != '': Aout = "State %s file %s line %s function %s thread %s" % (stateout,fileout,lineout,functionout,threadout)
		else: Aout = "State %s file %s line %s thread %s" % (stateout,fileout,lineout,threadout)

		Cout = "  %s=%s" % (lvalue,rvalue)

		# Filter out extra computations due to simulation code injected when translating.
		if lvalue.startswith('__cs_') and lvalue!= '__cs_message':
			return '','',''

		# Filter out __CPROVER internal states.
		if lineout == '?' and fileout == '?': return '','',''

		return (Aout,B,Cout)


	def _mapCPROVERendstate(self,A,B,C):
		mapback = {}

		'''
		'Violated property:'
		'  file _cs_lazy_unsafe.c line 318 function thread3_0'
		'  assertion 0 != 0'
		'  0 != 0'

		'''
		# Fetch values.
		try:
			# 1st line
			tokens = A.split()
			keys = {}

			for key,value in zip(tokens[0::2], tokens[1::2]):
				keys[key] = value

			line = filename = function = ''
		except Exception as e:
			self.warn('unable to parse counterexample final state')
			return ('','','')

		#if 'file' in keys: filename = keys['file']

		#if 'line' in keys and int(keys['line']) in mapback: line = mapback[int(keys['line'])]
		lineout = fileout = ''
		if 'line' in keys: lineout,fileout = self.sbizz(int(keys['line']))

		if 'function' in keys and int(keys['line']) in mapback:
			function = keys['function']

			if function in self.threadindexes: thread = self.threadindexes[function]
			if function in self.threadnamesmap: function = self.threadnamesmap[function]

			A = '  file %s line %s function %s' % (fileout,lineout,function)
		else:
			A = '  file %s line %s' % (fileout,lineout)

		return (A,B,C)





















































