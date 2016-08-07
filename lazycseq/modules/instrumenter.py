""" CSeq backend instrumentation module
	written by Omar Inverso, Truc Ngyuen Lam, University of Southampton.
"""
VERSION = 'instrumenter-0.0-2015.07.15'
# VERSION = 'instrumenter-0.0-2015.07.09'
#VERSION = 'instrumenter-0.0-2015.06.25'
"""
	Transformation 1 (convert function calls and add implementation):
		__CSEQ_assert()   -->   verifier-specific assert
		__CSEQ_assume()   -->   verifier-specific assume

	Transformation 2 (convert bitvectors)
		convert any  int  or  unsigned int  for which there is
		__CSEQ_bitvector[k] --> ...

	Transformation 3 (raw line injections and indentation):
		__CSEQ_rawline("string"); --> string

		this transformation uses
		separate indentation for raw and non-raw lines, where
		a raw line is a line inserted by __CSEQ_rawline()
		any other line is non-raw.
		Raw line are indentend fully left,
		non-raw are shifted to the right.

Changelog:
	2015.07.15  back to output.strip to remove fake header content

"""
from time import gmtime, strftime
import math, re
import core.module, core.utils
import pycparser.c_ast

_backends = ['cbmc', 'esbmc', 'llbmc', 'blitz', 'satabs', '2ls', 'klee', 'cpachecker']

fmap = {}

fmap['cbmc', '__CSEQ_assume'] = '__CPROVER_assume'
fmap['cbmc', '__CSEQ_assertext'] = '__CPROVER_assert'
fmap['cbmc', '__CSEQ_assert'] = 'assert'
fmap['cbmc', '__CSEQ_nondet_int'] = 'nondet_int'
fmap['cbmc', '__CSEQ_nondet_uint'] = 'nondet_uint'

fmap['esbmc', '__CSEQ_assume'] = '__ESBMC_assume'
fmap['esbmc', '__CSEQ_assertext'] = '__ESBMC_assert'
fmap['esbmc', '__CSEQ_assert'] = 'assert'
fmap['esbmc', '__CSEQ_nondet_int'] = '__VERIFIER_nondet_int'
fmap['esbmc', '__CSEQ_nondet_uint'] = '__VERIFIER_nondet_uint'

fmap['llbmc', '__CSEQ_assume'] = '__llbmc_assume'
fmap['llbmc', '__CSEQ_assertext'] = '__llbmc_assert'
fmap['llbmc', '__CSEQ_assert'] = '__llbmc_assert'
fmap['llbmc', '__CSEQ_nondet_int'] = 'nondet_int'
fmap['llbmc', '__CSEQ_nondet_uint'] = 'nondet_int'

fmap['blitz', '__CSEQ_assume'] = '__CPROVER_assume'
fmap['blitz', '__CSEQ_assertext'] = 'assert'
fmap['blitz', '__CSEQ_assert'] = 'assert'
fmap['blitz', '__CSEQ_nondet_int'] = 'nondet_int'
fmap['blitz', '__CSEQ_nondet_uint'] = 'nondet_uint'

fmap['satabs', '__CSEQ_assume'] = '__CPROVER_assume'
fmap['satabs', '__CSEQ_assertext'] = 'assert'
fmap['satabs', '__CSEQ_assert'] = 'assert'
fmap['satabs', '__CSEQ_nondet_int'] = 'nondet_int'
fmap['satabs', '__CSEQ_nondet_uint'] = 'nondet_uint'

# fmap['2ls', '__CSEQ_assume'] = '__CPROVER_assume'
# fmap['2ls', '__CSEQ_assertext'] = 'assert'
# fmap['2ls', '__CSEQ_assert'] = 'assert'
# fmap['2ls', '__CSEQ_nondet_int'] = 'nondet_int'
# fmap['2ls', '__CSEQ_nondet_uint'] = 'nondet_uint'

fmap['klee', '__CSEQ_assume'] = 'KLEE_assume'
fmap['klee', '__CSEQ_assertext'] = 'klee_assert'
fmap['klee', '__CSEQ_assert'] = 'klee_assert'
fmap['klee', '__CSEQ_nondet_int'] = 'KLEE_nondet_int'
fmap['klee', '__CSEQ_nondet_uint'] = 'KLEE_nondet_uint'

fmap['cpachecker', '__CSEQ_assume'] = '__VERIFIER_assume'
fmap['cpachecker', '__CSEQ_assertext'] = '__VERIFIER_assert'
fmap['cpachecker', '__CSEQ_assert'] = '__VERIFIER_assert'
fmap['cpachecker', '__CSEQ_nondet_int'] = '__VERIFIER_nondet_int'
fmap['cpachecker', '__CSEQ_nondet_uint'] = '__VERIFIER_nondet_uint'

_maxrightindent = 40   # max columns right for non-raw lines
_rawlinemarker = '__CSEQ_removeindent'

class instrumenter(core.module.Translator):
	def init(self):
		# Truc ---
		self.addInputParam('backend','backend to use for analysis, available choices are:\nbounded model-checkers: (blitz, cbmc, esbmc, llbmc)\nabstraction-based: (cpachecker, satabs)\ntesting: (klee)','b','cbmc',False)
		self.addInputParam('bitwidth','custom bidwidths for integers','w',None,True)
		self.addInputParam('header', 'raw text file to add on top of the instrumented file', 'h', '', True)


	def loadfromstring(self,string,env):
		self.env = env

		self.backend = self.getInputParamValue('backend')
		self.bitwidths = self.getInputParamValue('bitwidth')
		self.extheader = self.getInputParamValue('header')

		if self.backend not in _backends:
			raise core.module.ModuleError("backend '%s' not supported" % self.backend)

		super(self.__class__,self).loadfromstring(string,env)
		self.lastoutputlineno = 0
		self.removelinenumbers()
		# self.output = core.utils.strip(self.output)
		# self.inputtooutput = {}
		# self.outputtoinput = {}
		# self.generatelinenumbers()


		# Transformation 3:
		# shift indentation of raw lines fully left
		# removing the trailing marker _rawlinemarker+semicolon, and
		# shift any other line to the right depending to the longest rawline, and
		# in any case no longer than _maxrightindent.
		maxlinemarkerlen = max(len(l) for l in self.output.splitlines()) - len(_rawlinemarker+';')-2
		maxlinemarkerlen = min(maxlinemarkerlen,_maxrightindent)

		newstring = ''

		for l in self.output.splitlines():
			if l.endswith(_rawlinemarker+';'):
				newstring += l[:-len(_rawlinemarker+';')].lstrip() + '\n'
			else:
				newstring += ' '*(maxlinemarkerlen)+l+'\n'

		self.output = newstring

		self.insertheader(self.extheader)          # header passed by previous module
		self.insertheader(self._generateheader())  # top comment with translation parameters

		if self.backend == 'klee':
			self.insertheader(core.utils.printFile('modules/klee_extra.c'))
		if self.backend == 'cpachecker':
			self.insertheader(core.utils.printFile('modules/cpa_extra.c'))

	def visit_Decl(self,n,no_type=False):
		# no_type is used when a Decl is part of a DeclList, where the type is
		# explicitly only for the first delaration in a list.
		#
		s = n.name if no_type else self._generate_decl(n)

		# In case  x  has a custom bitwidth (passed by a previous module), convert
		# 'int x'  to  'bitvectors[k] x' or
		# 'unsigned int x'  to  'unsigned bitvectors[k] x'.
		ninitextra = ''
		if self.backend == 'cbmc':
			if self.bitwidths is not None:
				if s.startswith("int ") and (self.currentFunct,n.name) in self.bitwidths:
					s = s.replace("int ","__CPROVER_bitvector[%s] " % self.bitwidths[self.currentFunct,n.name],1)
					ninitextra = '(__CPROVER_bitvector[%s])' % self.bitwidths[self.currentFunct,n.name]
				elif s.startswith("unsigned int ") and (self.currentFunct,n.name) in self.bitwidths:
					s = s.replace("unsigned int ","unsigned __CPROVER_bitvector[%s] " % self.bitwidths[self.currentFunct,n.name],1)
					ninitextra = '(unsigned __CPROVER_bitvector[%s])' % self.bitwidths[self.currentFunct,n.name]

		if n.bitsize: s += ' : ' + self.visit(n.bitsize)
		if n.init:
			if isinstance(n.init,pycparser.c_ast.InitList):
				s += ' = {' + self.visit(n.init) + '}'
			elif isinstance(n.init,pycparser.c_ast.ExprList):
				s += ' = (' + self.visit(n.init) + ')'
			else:
				s += ' = ' + ninitextra + self.visit(n.init)
		return s


	''' converts function calls '''
	def visit_FuncCall(self,n):
		fref = self._parenthesize_unless_simple(n.name)

		# Transformation 3.
		if fref == '__CSEQ_rawline':
			return self.visit(n.args)[1:-1]+_rawlinemarker

		args = self.visit(n.args)
		if (fref == '__CSEQ_assertext' and
				self.backend not in ('cbmc', 'esbmc')):
			args = self.visit(n.args.exprs[0])   # Only get the first expression

		if (self.backend, fref) in fmap:
			fref = fmap[self.backend, fref]

		return fref + '(' + args + ')'

	def _generateheader(self):
		masterhash_framework = '0000'
		masterhash_modulechain = '0000'

		h  = '/*\n'
		h += ' *  generated by CSeq [ %s / %s ]\n' % (masterhash_framework,masterhash_modulechain)
		h += ' * \n'
		#h += ' *                    [ %s %s\n' % (core.utils.shortfilehash('cseq.py'),FRAMEWORK_VERSION)
		h += ' *                      %s %s\n' % (core.utils.shortfilehash('core/merger.py'),core.merger.VERSION)
		h += ' *                      %s %s\n' % (core.utils.shortfilehash('core/parser.py'),core.parser.VERSION)
		h += ' *                      %s %s ]\n' % (core.utils.shortfilehash('core/module.py'),core.module.VERSION)
		h += ' *\n'
		h += ' *  %s\n' %strftime("%Y-%m-%d %H:%M:%S",gmtime())
		h += ' *\n'
		h += ' *  params:\n'

		h += ' *    '
		for o,a in self.env.opts:
			 h+='%s %s, ' % (o,a)
		h+= '\n'
		h += ' *\n'

		h += ' *  modules:\n'

		for transforms,m in enumerate(self.env.modules):
			paramin = ' '.join(p.id for p in m.inputparamdefs)
			params = '(%s)'  % paramin
			h += ' *    %s %s-%s %s\n' %(core.utils.shortfilehash('modules/%s.py' % m.getname()),m.getname(),'0.0',params) # m.VERSION

		h += ' *\n'
		h += ' */\n'
		return h
