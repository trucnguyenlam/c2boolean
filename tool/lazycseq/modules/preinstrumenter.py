""" CSeq C Sequentialization Framework
	pre-instrumentation module

	written by Omar Inverso, University of Southampton.
"""
VERSION = 'preinstrumenter-2015.07.02' # merged with errorlabel-0.0-2015.06.25
#VERSION = 'preinstrumenter-0.0-2015.06.25'
"""
	Conceptually this module performs two tasks:
		1. convert input to use __CSEQ_assert() __CSEQ_assume() from any known equivalent primitive
		2. convert errorlabel reachability into an assertion failure
		3. map pthread_ specific api calls into cseq-specific calls.

	Transformation 1 (function calls):
		assert(),__CPROVER_assert(),__ESBMC_assert(),__VERIFIER_assert()   -->   __CSEQ_assert()
		assume(),__CPROVER_assume(),__ESBMC_assume(),__VERIFIER_assume()   -->   __CSEQ_assume()
		__VERIFIER_atomic_ABCDEF()   -->   __CSEQ_atomic_ABCDEF()

	Transformation 2 (function declarations):
		__VERIFIER_atomic_ABCDEF() { ... }   -->   __CSEQ_atomic_ABCDEF() { ... }

	Transformation 3 (error labels):
		- remove all ERROR labels
		- goto ERROR; ---> assert(0);
		- ERROR :;    ----> assert(0);

	Transformation 4:
		- rename all pthread_ id following the table in common.py
		- rename all other function calls (e.g. malloc) following the table in common.py

Changelog:
	2015.10.19  Fix problem when parsing struct
	2015.07.10  1st version

"""
import core.common, core.module
import pycparser.c_ast

class preinstrumenter(core.module.Translator):
	__errorlabel = ''

	namesmapping = {}

	namesmapping['assume'] = '__CSEQ_assume'
	namesmapping['__CPROVER_assume'] = '__CSEQ_assume'
	namesmapping['__ESBMC_assume'] = '__CSEQ_assume'
	namesmapping['__VERIFIER_assume'] = '__CSEQ_assume'

	namesmapping['assert'] = '__CSEQ_assert'
	namesmapping['__CPROVER_assert'] = '__CSEQ_assert'
	namesmapping['__ESBMC_assert'] = '__CSEQ_assert'
	namesmapping['__VERIFIER_assert'] = '__CSEQ_assert'


	def init(self):
 		self.addInputParam('error-label', 'error label for reachability check', 'l', 'ERROR', False)


	def loadfromstring(self, string, env):
		self.__errorlabel = self.getInputParamValue('error-label')
		super(self.__class__, self).loadfromstring(string, env)


	'''
	def visit_Assignment(self, n):
		rval_str = self._parenthesize_if(n.rvalue, lambda n: isinstance(n, pycparser.c_ast.Assignment))

		print "tipo: %s" % type(n.rvalue)

		if type(n.rvalue) == pycparser.c_ast.FuncCall and self.visit(n.rvalue.name) == 'malloc':


		return '%s %s %s' % (self.visit(n.lvalue), n.op, rval_str)
	'''


	def visit_Goto(self, n):
		if n.name == self.__errorlabel:
			return '__CSEQ_assert(0);'
		else: return 'goto ' + n.name + ';'


	def visit_Label(self, n):
		##### print "visiting LABEL, coords = %s\n" % self.Parser.nodecoords[n]

		if n.name == self.__errorlabel:
			return '__CSEQ_assert(0);'
		else:
			return n.name + ':\n' + self._generate_stmt(n.stmt)


	def visit_ID(self, n):
		if n.name.startswith('pthread_') and n.name not in core.common.changeID and n.name not in self.Parser.funcBlock:
			self.warn("%s is not handled.\n" % n.name)

		if n.name in core.common.changeID:
			return core.common.changeID[n.name]

		return n.name


	''' converts function calls '''
	def visit_FuncCall(self, n):
		fref = self._parenthesize_unless_simple(n.name)

		args = self.visit(n.args)

		if fref in self.namesmapping:
			fref = self.namesmapping[fref]
		elif fref.startswith('__VERIFIER_atomic_'):
			fref = '__CSEQ_atomic_'+fref[18:]

		if fref in core.common.changeID:
			fref = core.common.changeID[fref]

		#if fref == 'malloc':

		return fref + '(' + args + ')'


	''' converts function definitions '''
	def visit_FuncDef(self, n):
		decl = self.visit(n.decl)

		#if '__VERIFIER_atomic' in decl:
		#	decl = decl.replace('__VERIFIER_atomic', core.common.funcPrefixChange['__VERIFIER_atomic'], 1)

		if n.decl.name.startswith('__VERIFIER_atomic_'):
			decl = decl.replace('__VERIFIER_atomic_', '__CSEQ_atomic_', 1)

		self.indent_level = 0
		body = self.visit(n.body)
		if n.param_decls:
			knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
			return decl + '\n' + knrdecls + ';\n' + body + '\n'
		else:
			return decl + '\n' + body + '\n'


	''' converts function prototypes '''
	def visit_Decl(self, n, no_type=False):
		# no_type is used when a Decl is part of a DeclList, where the type is
		# explicitly only for the first delaration in a list.
		#
		s = n.name if no_type else self._generate_decl(n)

		if n.name and n.name.startswith('__VERIFIER_atomic_'):
			s = s.replace('__VERIFIER_atomic_', '__CSEQ_atomic_', 1)

		if n.bitsize: s += ' : ' + self.visit(n.bitsize)
		if n.init:
			if isinstance(n.init, pycparser.c_ast.InitList):
				s += ' = {' + self.visit(n.init) + '}'
			elif isinstance(n.init, pycparser.c_ast.ExprList):
				s += ' = (' + self.visit(n.init) + ')'
			else:
				s += ' = ' + self.visit(n.init)

		#print "type: %s   decl: %s\n" % (n.storage, n.name)
		for expr in core.common.changeID:
			if s.startswith(expr+' '):
				s = s.replace(expr+' ', core.common.changeID[expr]+' ')

		return s


	def visit_IdentifierType(self, n):
		s = ' '.join(n.names)

		if s in core.common.changeID:
			s = core.common.changeID[s]

		return s


	def visit_Typename(self, n):
		s = self._generate_type(n.type)

		if s[:s.rfind(' ')] in core.common.changeID:
			###print "changing  %s --> %s\n" % (s[:s.rfind(' ')], core.common.changeID[s[:s.rfind(' ')]])
			return core.common.changeID[s[:s.rfind(' ')]]
		else:
			return s











