""" Lazy-CSeq Sequentialization module
	written by Omar Inverso, University of Southampton.
"""
VERSION = 'lazyseq-0.0-2015.10.19'   # CSeq-1.0 Release ASE2015
# VERSION = 'lazyseq-0.0-2015.07.18'   # CSeq-1.0 Release ASE2015
#VERSION = 'lazyseq-0.0-2015.07.16'   # main driver bug fixing (Truc)
#VERSION = 'lazyseq-0.0-2015.07.10'
#VERSION = 'lazyseq-0.0-2015.06.30'
#VERSION = 'lazyseq-0.0-2015.06.11'
#VERSION = 'labeler-0.0-2015.02.22'   # bugfix
#VERSION = 'labeler-0.0-2015.01.27'   # more efficient handling of mutexes
#VERSION = 'labeler-0.0-2015.01.17'   # small optimization
#VERSION = 'labeler-0.0-2015.01.12'   # back to CAV-style constraints in the main driver
#VERSION = 'labeler-0.0-2014.10.29'   # CSeq-1.0beta
#VERSION = 'labeler-0.0-2014.10.29'   # newseq-0.6c SVCOMP15
#VERSION = 'labeler-0.0-2014.10.26'   # newseq-0.6a
#VERSION = 'labeler-0.0-2014.10.15'
#VERSION = 'labeler-0.0-2014.06.26' (CSeq-Lazy-0.4)
#VERSION = 'labeler-0.0-2014.03.14' (CSeq-Lazy-0.3)
#VERSION = 'labeler-0.0-2014.02.25' (CSeq-Lazy-0.2)

"""

Transformation:
	implements the lazy sequentialization schema
	(see Inverso, Tomasco, Fischer, La Torre, Parlato, CAV'14)

Prerequisites:
	- all functions should have been inlined, except the main(), all thread functions, all __CSEQ_atomic_ functions, and function __CSEQ_assert
	- all loops should habe been unrolled
	- no two threads refers to the same thread function (use module duplicator.py)

TODO:
	- get rid of _init_scalar() (see ext. notes)
	- check the STOP() inserting mechanism
	- this schema assumes no mutex_lock() in main() - is this fine?

Changelog:
	2015.10.19  fix bug of __CSEQ_atomic_begin (definitely have a context switch before this statement)
	2015.07.18  new --schedule parameter to set schedule restrictions (Omar)
	2015.07.15  changed visit of atomic function definitions (Truc,Omar)
	2015.07.10  no context switch between __CSEQ_atomic_begin() and __CSEQ_atomic_end()
	2015.06.30  major refactory (converted to stand-alone instrumentation, etc.)
	2015.04.23  __globalAccess()  was causing  if(cond)  blocks to disappear
	2015.02.22  __CSEQ_assume() without occurrences of shared vars produces no context switch points
	2015.01.27  using macros rather than functions to model pthread_mutex_lock/unlock() avoids using ptrs and thus yields faster analysis
	2014.01.17  main driver: first thread execution context must have length > 0 (faster analysis)
	2014.12.24  linemap (overrides the one provided by core.module)
				bugfixes, code maintenance
	2014.12.09  further code refactory to match the new organisation of the CSeq framework
	2014.10.29  bitvector encoding for all control variables (pc, pc_cs, ...)
				new main driver where guessed jump lenghts are all at the top (this allows inserting p.o.r. constraints right after them)
	2014.10.26  removed dead/commented-out/obsolete code
	2014.10.15  removed visit() and moved visit call-stack handling to module class (module.py)
	2014.06.26  improved backend-specific instrumentation
				added new backend Klee
	2014.03.14  further code refactory to match  module.Module  class interface
	2014.02.25  switched to  module.Module  base class for modules
	2014.01.19  fixed assert()s missing their stamps at the beginning

"""
import math, re
from time import gmtime, strftime
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class lazyseq(core.module.Translator):
	__lines = {}                     # lines for each thread
	__threadName = ['main']          # name of threads, as they are found in pthread_create(s) - the threads all have different names
	__threadIndex = {}               # index of the thread = value of threadcount when the pthread_create to that thread was discovered
	__threadCount = 0                # pthread create()s found so far

	__labelLine = {}                 # statement number where labels are defined [function, label]
	__gotoLine = {}                  # statement number where goto to labels appear [function, label]
	__maxInCompound = 0              # max label within a compound
	__labelLength = 55               # for labels to have all the same length, adding padding when needed
	__startChar = 't'                # special char to distinguish between labeled and non-labelled lines

	__stmtCount = -1                 # thread statement counter (to build thread labels)

	__currentThread = ''             # name of the current thread (also used to build thread labels)

	__threadbound = 0                # bound on the number of threads

	__firstThreadCreate = False      # set once the first thread creation is met
	__globalMemoryAccessed = False   # used to limit context-switch points (when global memory is not accessed, no need to insert them)

	__first = False
	__atomic = False                 # no context-switch points between atomic_start() and atomic_end()

	_bitwidth = {}                   # custom bitwidth for specific int variables, e.g. ['main','var'] = 4

	_deadlockcheck = False

	__explicitround = []             # explicit schedules restrictions


	def init(self):
		self.addInputParam('rounds', 'round-robin schedules', 'r', '1', False)
		self.addInputParam('schedule', 'schedule restriction (example: 1,2:+:3)', 's', default='', optional=True)
		self.addInputParam('threads', 'max no. of thread creations (0 = auto)', 't', '0', False)
		self.addInputParam('deadlock', 'check for deadlock', '', default=False, optional=True)

		self.addOutputParam('bitwidth')
		self.addOutputParam('header')


	def loadfromstring(self, string, env):
		if self.getInputParamValue('deadlock') is not None:
			self._deadlockcheck = True

		threads = int(self.getInputParamValue('threads'))
		rounds = int(self.getInputParamValue('rounds'))
		backend = self.getInputParamValue('backend')

		schedule = self.getInputParamValue('schedule')

		#
		if schedule is not None:
			while schedule.startswith(':'): schedule = schedule[1:]
			while schedule.endswith(':'): schedule = schedule[:-1]
			while schedule.find('::') != -1: schedule = schedule.replace('::',':')
			while schedule.find(',,') != -1: schedule = schedule.replace(',,',',')
			while schedule.startswith(','): schedule = schedule[1:]
			while schedule.endswith(','): schedule = schedule[:-1]

		#
		if schedule is not None and schedule != '':
			for i in schedule.split(':'):
				self.__explicitround.append(i)

		for x in self.__explicitround:
			for y in x.split(','):
				if y != '+' and not y.isdigit():
					self.warn("invalid scheduling ignored")
					self.__explicitround = []
				elif y.isdigit() and int(y) > threads:
					self.warn("invalid scheduling ignored (thread %s does not exist)" % y)
					self.__explicitround = []


		# schedules > rounds: adjust rounds
		# schedules < rounds: add more unconstrained entries
		if len(self.__explicitround) > rounds:
			#self.warn('round bound increased to %s due to longer schedule' % len(schedule.split(':')))
			rounds = len(schedule.split(':'))
		elif len(self.__explicitround) < rounds:
			for i in range(len(self.__explicitround),rounds):
				self.__explicitround.append('+')

		self.__explicitround[0] += ',0'   # main thread must always be schedulable in the 1st round


		self.__threadbound = threads

		super(self.__class__, self).loadfromstring(string, env)

		# Add the new main().
		self.output += self.__createMain(rounds)

		# Insert the thread sizes (i.e. number of visible statements).
		lines = ''

		i = maxsize = 0

		for t in self.__threadName:
			if i <= self.__threadbound:
				if i>0: lines += ', '
				lines += str(self.__lines[t])
				maxsize = max(int(maxsize), int(self.__lines[t]))
				#print "CONFRONTO %s %s " % (int(maxsize), int(self.__lines[t]))
			i +=1

		ones = ''
		if i <= self.__threadbound:
			if i>0: ones += ', '
			ones += '-1'
		i +=1

		# Generate the header.
		#
		# the first part is not parsable (contains macros)
		# so it is passed to next module as a header...
		header = core.utils.printFile('modules/lazyseqA.c')
		header = header.replace('<insert-maxthreads-here>',str(threads))
		header = header.replace('<insert-maxrounds-here>',str(rounds))
		self.setOutputParam('header', header)

		# ..this is parsable and is added on top of the output code,
		# as next module is able to parse it.
		if not self._deadlockcheck:
			header = core.utils.printFile('modules/lazyseqB.c').replace('<insert-threadsizes-here>',lines)
		else:
			header = core.utils.printFile('modules/lazyseqBdeadlock.c').replace('<insert-threadsizes-here>',lines)
			header = header.replace('<insert-all1-here>',  ones)

		self.insertheader(header)

		# Calculate exact bitwidth size for a few integer control variables of the seq. schema,
		# good in case the backend handles bitvectors.
		k = int(math.floor(math.log(maxsize,2)))+1
		self._bitwidth['','__cs_active_thread'] = 1
		self._bitwidth['','__cs_pc'] = k
		self._bitwidth['','__cs_pc_cs'] = k+1
		self._bitwidth['','__cs_thread_lines'] = k
		self.setOutputParam('__cs_bitwidth', self._bitwidth)

		# Fix gotos by inserting ASS_GOTO(..) blocks before each goto,
		# excluding gotos which destination is the line below.
		for (a,b) in self.__labelLine:
			if (a,b) in self.__gotoLine and (self.__labelLine[a,b] == self.__gotoLine[a,b]+1):
				self.output = self.output.replace('<%s,%s>' % (a,b), '')
			else:
				self.output = self.output.replace('<%s,%s>' % (a,b), 'ASS_GOTO(%s)' % self.__labelLine[a,b])

		self.setOutputParam('bitwidth', self._bitwidth)


	def visit_Compound(self, n):
		s = self._make_indent() + '{\n'
		self.indent_level += 1

		# Insert the labels at the beginning of each statement,
		# with a few exclusions to reduce context-switch points...
		#
		if n.block_items:
			for stmt in n.block_items:
				# Case 1: last statement in a thread (must correspond to last label)
				if type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == core.common.changeID['pthread_exit']: ##if type(stmt) == pycparser.c_ast.FuncCall and self._parenthesize_unless_simple(stmt.name) == core.common.changeID['pthread_exit']:
					self.__stmtCount += 1
					self.__maxInCompound = self.__stmtCount
					stamp = '__CSEQ_rawline("%s%s_%s: ");\n' % (self.__startChar, self.__currentThread, str(self.__stmtCount))
					code = self.visit(stmt)
					newStmt =  stamp + code + ';\n'
					s += newStmt
				# Case 2: labels
				elif (type(stmt) in (pycparser.c_ast.Label,)):
					# --1-- Simulate a visit to the stmt block to see whether it makes any use of pointers or shared memory.
					#
					globalAccess = self.__globalAccess(stmt)
					newStmt = ''

					# --2-- Now rebuilds the stmt block again,
					#       this time using the proper formatting
					#      (now we know if the statement is accessing global memory,
					#       so to insert the stamp at the beginning when needed)
					#
					if not self.__atomic and self.__stmtCount == -1:   # first statement in a thread
						self.__stmtCount += 1
						self.__maxInCompound = self.__stmtCount
						threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
						stamp = '__CSEQ_rawline("IF(%s,%s,%s%s_%s)");\n' % (threadIndex,str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
						code = self.visit(stmt)
						newStmt = stamp + code + ';\n'
					elif (
						(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == '__CSEQ_atomic_begin') or
						(not self.__atomic and
							(globalAccess or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == core.common.changeID['pthread_create']) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == core.common.changeID['pthread_join']) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__CSEQ_atomic') and not stmt.name.name == '__CSEQ_atomic_end') or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__CSEQ_assume')) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == '__cs_cond_wait_2')
							)
						)
						):
						self.__stmtCount += 1
						self.__maxInCompound = self.__stmtCount
						threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
						stamp = '__CSEQ_rawline("%s%s_%s: IF(%s,%s,%s%s_%s)");\n' % (self.__startChar, self.__currentThread, str(self.__stmtCount),threadIndex,str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
						newStmt = stamp + self.visit(stmt.stmt) + ';\n'
					else:
						newStmt = self.visit(stmt.stmt) + ';\n'

					threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
					#newStmt = self._make_indent()+ stmt.name + ': ' + 'GUARD(%s,%s)\n' % (threadIndex,self.__stmtCount+1) + newStmt+ '\n'
					newStmt = self._make_indent()+ stmt.name + ': ' + '__CSEQ_assume( __cs_pc_cs[%s] >= %s );\n' % (threadIndex,self.__stmtCount+1) + newStmt+ '\n'

					s += newStmt
				# Case 3: all the rest....
				elif (type(stmt) not in (pycparser.c_ast.Compound, pycparser.c_ast.Goto, pycparser.c_ast.Decl)
					and not (self.__currentThread=='main' and self.__firstThreadCreate == False) or (self.__currentThread=='main' and self.__stmtCount == -1)) :

					# --1-- Simulate a visit to the stmt block to see whether it makes any use of pointers or shared memory.
					#
					globalAccess = self.__globalAccess(stmt)
					newStmt = ''

					self.lines = []   # override core.module marking behaviour, otherwise  module.visit()  won't insert any marker

					# --2-- Now rebuilds the stmt block again,
					#       this time using the proper formatting
					#      (now we know if the statement is accessing global memory,
					#       so to insert the stamp at the beginning when needed)
					#
					if not self.__atomic and self.__stmtCount == -1:   # first statement in a thread
						self.__stmtCount += 1
						self.__maxInCompound = self.__stmtCount
						threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
						stamp = '__CSEQ_rawline("IF(%s,%s,%s%s_%s)");\n' % (threadIndex,str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
						code = self.visit(stmt)
						newStmt = stamp + code + ';\n'
					elif (
						(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == '__CSEQ_atomic_begin') or
						(not self.__atomic and
							(globalAccess or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == core.common.changeID['pthread_create']) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == core.common.changeID['pthread_join']) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__CSEQ_atomic') and not stmt.name.name == '__CSEQ_atomic_end') or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__CSEQ_assume')) or
							(type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name == '__cs_cond_wait_2')
							)
						)
						):
						self.__stmtCount += 1
						self.__maxInCompound = self.__stmtCount
						threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
						stamp = '__CSEQ_rawline("%s%s_%s: IF(%s,%s,%s%s_%s)");\n' % (self.__startChar, self.__currentThread, str(self.__stmtCount),threadIndex,str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
						code = self.visit(stmt)
						newStmt =  stamp + code + ';\n'
					else:
						newStmt = self.visit(stmt) + ";\n"

					s += newStmt
				else:
					newStmt = self.visit(stmt) + ";\n"
					s += newStmt

		self.indent_level -= 1
		s += self._make_indent() + '}\n'

		return s


	def visit_FuncDef(self, n):
		if (n.decl.name.startswith('__CSEQ_atomic_') or
			#n.decl.name.startswith(core.common.funcPrefixChange['__CSEQ_atomic']) or
			n.decl.name == '__CSEQ_assert' or
			n.decl.name in self.Parser.funcReferenced ):   # <--- functions called through pointers are not inlined yet
			# return self.Parser.funcBlock[n.decl.name]

			#ret = self.otherparser.visit(n)
			oldatomic = self.__atomic
			self.__atomic = True
			decl = self.visit(n.decl)
			body = self.visit(n.body)
			self.__atomic = oldatomic
			return decl + '\n' + body + '\n'

		self.__first = False
		self.__currentThread = n.decl.name
		self.__firstThreadCreate = False

		decl = self.visit(n.decl)
		self.indent_level = 0
		body = self.visit(n.body)

		f = ''

		self.__lines[self.__currentThread] = self.__stmtCount
		###print "THREAD %s, LINES %s \n\n" % (self.__currentThread, self.__lines)

		#
		if n.param_decls:
			knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
			self.__stmtCount = -1
			#body = body[:body.rfind('}')] + self._make_indent() + returnStmt + '\n}'
			f = decl + '\n' + knrdecls + ';\n'
		else:
			self.__stmtCount = -1
			#body = body[:body.rfind('}')] + self._make_indent() + returnStmt + '\n}'
			f = decl + '\n'

		# Remove arguments (if any) for main() and transform them into local variables in main_thread.
		# TODO re-implement seriously.
		if self.__currentThread == 'main':
			#f = f.replace(' main(', ' main_thread(')
			#f = re.sub(r' main([-a-zA-Z0-9_, *]+)', r' main_thread(void)', f)
			f = re.sub(r' main(.*)', r' main_thread(void)', f)

			if self.Parser.funcBlockIn['main'] != 'void':
				body = '{' + self.Parser.funcBlockIn['main'].replace(',', ';')+';' + body[body.find('{')+1:]
			else:
				body = '{' + body[body.find('{')+1:]

			# Change *argv[] and **argv[] --> **argv
			body = re.sub(r'\*(.*)\[\]', r'** \1', body)
			body = re.sub(r'(.*)\[\]\[\]', r'** \1', body)

		f += body + '\n'

		return f + '\n\n'


	def visit_If(self, n):
		ifStart = self.__maxInCompound   # label where the if stmt begins

		s = 'if ('

		if n.cond:
			condition = self.visit(n.cond)
			s += condition

		s += ')\n'
		s += self._generate_stmt(n.iftrue, add_indent=True)

		ifEnd = self.__maxInCompound   # label for the last stmt in the if block:  if () { block; }
		nextLabelID = ifEnd+1

		if n.iffalse:
			elseBlock = self._generate_stmt(n.iffalse, add_indent=True)

			elseEnd = self.__maxInCompound   # label for the last stmt in the if_false block if () {...} else { block; }

			if ifStart < ifEnd:
				threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
				#elseHeader = 'GUARD(%s,%s)' % (threadIndex, str(ifEnd+1))
				elseHeader = '__CSEQ_assume( __cs_pc_cs[%s] >= %s );' % (threadIndex, str(ifEnd+1))
			else:
				elseHeader = ''

			nextLabelID = elseEnd+1
			s += self._make_indent() + 'else\n'

			elseBlock = elseBlock.replace('{', '{ '+elseHeader, 1)
			s += elseBlock

		header = ''

		if ifStart+1 < nextLabelID:
			threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
			#footer = 'GUARD(%s,%s)' % (threadIndex, nextLabelID)
			footer = '__CSEQ_assume( __cs_pc_cs[%s] >= %s );' % (threadIndex, nextLabelID)
		else:
			footer = ''

		'''
		if n.iffalse:
			header = 'ASS_ELSE(%s, %s, %s)' % (condition, ifEnd+1, elseEnd+1) + '\n' + self._make_indent()
		else:
			if ifEnd > ifStart:
				header = 'ASS_THEN(%s, %s)' % (condition, ifEnd+1) + '\n' + self._make_indent()
			else: header = ''
		'''

		return header + s + self._make_indent() + footer


	def visit_Return(self, n):
		if self.__currentThread != '__CSEQ_assert' and self.__currentThread not in self.Parser.funcReferenced and not self.__atomic:
			self.error("error: %s: return statement in thread '%s'.\n" % (self.getname(), self.__currentThread))

		s = 'return'
		if n.expr: s += ' ' + self.visit(n.expr)
		return s + ';'


	def visit_Label(self, n):
		self.__labelLine[self.__currentThread, n.name] = self.__stmtCount
		return n.name + ':\n' + self._generate_stmt(n.stmt)


	def visit_Goto(self, n):
		self.__gotoLine[self.__currentThread, n.name] = self.__stmtCount
		extra = '<%s,%s>\n' % (self.__currentThread, n.name) + self._make_indent()
		extra = ''
		return extra + 'goto ' + n.name + ';'


	def visit_ID(self, n):
		# If this ID corresponds either to a global variable,
		# or to a pointer...
		#
		if ((self.__isGlobal(self.__currentThread, n.name) or self.__isPointer(self.__currentThread, n.name)) and not
			n.name.startswith('__cs_thread_local_')):
			#print "variable %s in %s is global\n" % (n.name, self.__currentThread)
			self.__globalMemoryAccessed = True

		# Rename the IDs of main() arguments
		#if self.__currentThread == 'main' and n.name in self.Parser.varNames['main'] and self.Parser.varKind['main',n.name] == 'p':
		#   return '__main_params_' + n.name

		return n.name


	def visit_FuncCall(self, n):
		fref = self._parenthesize_unless_simple(n.name)
		args = self.visit(n.args)

		if fref == '__CSEQ_atomic_begin': self.__atomic = True
		elif fref == '__CSEQ_atomic_end': self.__atomic = False
		elif fref.startswith('__CSEQ_atomic_'): self.__globalMemoryAccessed = True
		elif fref == core.common.changeID['pthread_cond_wait']:
			self.error('pthread_cond_wait in input code (use conditional wait converter module first)')


		# When a thread is created, extract its function name
		# based on the 3rd parameter in the pthread_create() call:
		#
		# pthread_create(&id, NULL, f, &arg);
		#                          ^^^
		#
		if fref == core.common.changeID['pthread_create']: # TODO re-write AST-based (see other modules)
			fName = args[:args.rfind(',')]
			fName = fName[fName.rfind(',')+2:]
			fName = fName.replace('&', '')

			##print "checking fName = %s\n\n" % fName

			if fName not in self.__threadName:
				self.__threadName.append(fName)
				self.__threadCount = self.__threadCount + 1

				args = args + ', %s' % (self.__threadCount)
				self.__threadIndex[fName] = self.__threadCount
			else:
				# when visiting from the 2nd time on (if it happens),
				# reuse the old thread indexS!
				args = args + ', %s' % (self.__threadIndex[fName])

			self.__firstThreadCreate = True

		if fref == core.common.changeID['pthread_exit']:
			if self.Parser.funcBlockOut[self.__currentThread] != 'void':
				return 'STOP_NONVOID(%s)' % (self.__stmtCount)
			else:
				return 'STOP_VOID(%s)' % (self.__stmtCount)

		# Avoid using pointers to handle mutexes
		# by changing the function calls,
		# there are two cases:
		#
		#    pthread_mutex_lock(&l)   ->  __cs_mutex_lock(l)
		#    pthread_mutex_lock(ptr)  ->  __cs_mutex_lock(*ptr)
		#
		# TODO:
		#    this needs proper implementation,
		#    one should check that the argument is not referenced
		#    elsewhere (otherwise this optimisation will not work)
		#
		'''
		if (fref == core.common.changeID['pthread_mutex_lock'] ) or (fref == core.common.changeID['pthread_mutex_unlock']):
			if args[0] == '&': args = args[1:]
			else: args = '*'+args
		'''

		return fref + '(' + args + ')'


	def __createMain(self, ROUNDS):
		# the new main() is created according to the following sort of scheduling:
		#
		# main_thread    t1 t2    t1 t2   t1 t2    t1 t2     t1 t2    t1 t2      t1 t2    t1 t2    main_thread
		#
		round = 0

		main = ''
		main += "int main(void) {\n"

		''' Part I:
			Pre-guessed jump lenghts have a size in bits depending on the size of the thread.
		'''
		for r in range(0, ROUNDS):
			for t in range(0,self.__threadbound+1):
				#print "round %s thread %s schedule:%s" %(r,t,self.__explicitround[r].split(','))
				if str(t) in self.__explicitround[r].split(',') or '+' in self.__explicitround[r]:
					threadsize = self.__lines[self.__threadName[t]]

					k = int(math.floor(math.log(threadsize,2)))+1
					#main += "          unsigned __CSEQ_bitvector[%s] tmp_t%s_r%s;\n" % (k, t, r)
					main += "          unsigned int __cs_tmp_t%s_r%s = __CSEQ_nondet_uint();\n" % (t, r)
					self._bitwidth['main','__cs_tmp_t%s_r%s' % (t,r)] = k

		k = int(math.floor(math.log(self.__lines['main'],2)))+1
		#main += "          unsigned __CSEQ_bitvector[%s] tmp_t%s_r%s;\n" % (k, 0, ROUNDS)
		main += "          unsigned int __cs_tmp_t%s_r%s = __CSEQ_nondet_uint();\n" % (0, ROUNDS)
		self._bitwidth['main','__cs_tmp_t%s_r%s' % (0, ROUNDS)] = k

		''' Part II:
			Schedule pruning constraints.
		'''
		'''
		main += '\n'

		schedulesPruned = []  # remeember which rounds have been pruned

		for t in range(0,self.__threadbound+1):
			#print "thread %s,  name %s,   maxrepr %s,  threadsize %s" % (t,self.__threadName[t],maxrepresentable, threadsize)
			threadsize = self.__lines[self.__threadName[t]]
			maxrepresentable =  2**int((math.floor(math.log(threadsize,2)))+1) - 1

			sumall = ''

			for r in range(0, ROUNDS):
				sumall += '__cs_tmp_t%s_r%s%s' % (t,r, ' + ' if r<ROUNDS-1 else '')

			if t == 0:
				sumall += ' + __cs_tmp_t0_r%s' % (ROUNDS)

			######if (maxrepresentable > threadsize+1) and int((math.floor(math.log(threadsize,2)))+1)+1 > 4:
			if (maxrepresentable > threadsize+1) and int((math.floor(math.log(threadsize,2)))+1)+1 > 4:
				schedulesPruned.append(True)
				if t == 0:
					wow =   int(math.ceil(math.log((maxrepresentable*(ROUNDS+1)),2)))
				else:
					wow =   int(math.ceil(math.log((maxrepresentable*ROUNDS),2)))
				##wow =   int(math.ceil(math.log((maxrepresentable*ROUNDS),2)))

				#main += "          unsigned __CSEQ_bitvector[%s] top%s = %s;\n" % (wow, t, threadsize)
				main += "          unsigned int __cs_top%s = %s;\n" % (t, threadsize)
				self._bitwidth['main','__cs_top%s' % t] = wow
				#main += "          unsigned __CSEQ_bitvector[%s] sum%s = %s;\n" % (wow, t, sumall)
				#main += "          __CSEQ_assume(sum%s <= top%s);\n" % (t,t)
			else:
				schedulesPruned.append(False)
		'''


		''' Part III:
		'''
		# 1st round (round 0)
		#
		main +="__CSEQ_rawline(\"/* round  %s */\");\n" % round
		main += '         __CSEQ_assume(__cs_tmp_t0_r0 > 0);\n'
		main +="          __cs_thread_index = 0;\n"
		main +="          __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r0;\n"
		main +="          __CSEQ_assume(__cs_pc_cs[0] > 0);\n"
		main +="          __CSEQ_assume(__cs_pc_cs[0] <= %s);\n" % (self.__lines['main'])
		main +="          main_thread();\n"
		main +="          __cs_pc[0] = __cs_pc_cs[0];\n"
		main +="\n"

		i = 1
		for t in self.__threadName:
			if t == 'main': continue
			if i <= self.__threadbound:
				if str(i) in self.__explicitround[0].split(',') or '+' in self.__explicitround[0]:
					main +="          if (__cs_active_thread[%s] == 1) {\n" % i
					main +="             __cs_thread_index = %s;\n" % i
					main +="             __cs_pc_cs[%s] = __cs_pc[%s] + __cs_tmp_t%s_r%s;\n" % (i, i, i, 0)
					main +="             __CSEQ_assume(__cs_pc_cs[%s] <= %s);\n" % (i, self.__lines[t])
					main +="             %s(%s);\n" % (t, '__cs_threadargs[%s]') % (i)
					main +="             __cs_pc[%s] = __cs_pc_cs[%s];\n" % (i,i)
					main +="          }\n\n"
				i += 1

		# remaining rounds
		#
		for round in range(1,ROUNDS):
			i=0
			main +="__CSEQ_rawline(\"/* round  %s */\");\n" % round

			if str(i) in self.__explicitround[round].split(',') or '+' in self.__explicitround[round]:
				main +="          if (__cs_active_thread[0] == 1) { " # // %s\n"  % ('main')
				main +="              __cs_thread_index = 0;\n"
				main +="              __cs_pc_cs[%s] = __cs_pc[%s] + __cs_tmp_t%s_r%s;\n" % (i, i, i, round)
				# Truc --
				main +="              __CSEQ_assume(__cs_pc_cs[%s] >= __cs_pc[%s]);\n" % (i, i)
				main +="              __CSEQ_assume(__cs_pc_cs[%s] <= %s);\n" % (i, self.__lines['main'])
				main +="              main_thread();\n"
				main +="              __cs_pc[0] = __cs_pc_cs[0];\n"
				main +="          }\n\n"

			i = 1
			for t in self.__threadName:
				if t == 'main': continue
				if i <= self.__threadbound:
					if str(i) in self.__explicitround[round].split(',') or '+' in self.__explicitround[round]:
						main +="          if (__cs_active_thread[%s] == 1) {\n" % i
						main +="             __cs_thread_index = %s;\n" % i
						main +="             __cs_pc_cs[%s] = __cs_pc[%s] + __cs_tmp_t%s_r%s;\n" % (i, i, i, round)
						# Truc --
						main +="              __CSEQ_assume(__cs_pc_cs[%s] >= __cs_pc[%s]);\n" % (i, i)
						main +="             __CSEQ_assume(__cs_pc_cs[%s] <= %s);\n" % (i, self.__lines[t])
						main +="             %s(%s);\n" % (t, '__cs_threadargs[__cs_thread_index]')
						main +="             __cs_pc[%s] = __cs_pc_cs[%s];\n" % (i,i)
						main +="          }\n\n"
					i += 1

		# Last call to main()
		#
		round = ROUNDS-1
		main +="          if (__cs_active_thread[0] == 1) {\n"
		main +="             __cs_thread_index = 0;\n"
		main +="             __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r%s;\n" % (ROUNDS)
		# Truc --
		main +="              __CSEQ_assume(__cs_pc_cs[0] >= __cs_pc[0]);\n"

		main +="             __CSEQ_assume(__cs_pc_cs[0] <= %s);\n" % (self.__lines['main'])
		main +="             main_thread();\n"
		main +="          }\n\n"
		main += "   return 0;\n"

		main += "}\n\n"

		return main


	# Checks whether variable  v  from function  f  is a pointer.
	#
	def __isPointer(self, f, v):
		if v in self.Parser.varNames[f] and self.Parser.varType[f,v].endswith('*'): return True
		elif v in self.Parser.varNames[''] and self.Parser.varType['',v].endswith('*'): return True
		else: return False


	# Checks whether variable  v  from function  f  is global.
	#
	def __isGlobal(self, f, v):
		if (v in self.Parser.varNames[''] and v not in self.Parser.varNames[f]): return True
		else: return False


	# Check whether the given AST node accesses global memory or uses a pointer.
	#
	# TODO: this overapproximation is very rough,
	#      (variable dependency, pointer analysis etc,
	#       could be useful for refinement)
	#
	def __globalAccess(self, stmt):
		if self.__atomic: return False  # if between atomic_begin() and atomic_end() calls no context switchs needed..

		oldStmtCount = self.__stmtCount             # backup counters
		oldMaxInCompound = self.__maxInCompound
		oldGlobalMemoryAccessed = self.__globalMemoryAccessed

		globalAccess = False
		self.__globalMemoryAccessed = False

		if type(stmt) not in (pycparser.c_ast.If, ):
			tmp = self._generate_stmt(stmt)
		else:
			tmp = self._generate_stmt(stmt.cond)

		globalAccess = self.__globalMemoryAccessed

		self.__stmtCount = oldStmtCount             # restore counters
		self.__maxInCompound = oldMaxInCompound
		self.__globalMemoryAccessed = oldGlobalMemoryAccessed

		return globalAccess



