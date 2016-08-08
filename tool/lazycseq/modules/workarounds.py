""" CSeq Workarounds module
    written by Omar Inverso for the University of Southampton.
"""
VERSION = 'workaround-0.0-2015.01.13'
#VERSION = 'workaround-0.0-2015.01.13'
#VERSION = 'workaround-0.0-2014.12.24'  # CSeq-1.0beta
#VERSION = 'workaround-0.0-2014.10.26'   # CSeq-Lazy-0.6: newseq-0.6a, newseq-0.6c, SVCOMP15
#VERSION = 'workaround-0.0-2014.10.09'
#VERSION = 'workaround-0.0-2014.06.05' (CSeq-Lazy-0.4)
#VERSION = 'workaround-0.0-2014.03.14' (CSeq-Lazy-0.3)
#VERSION = 'workaround-0.0-2014.02.25' (CSeq-Lazy-0.2)
"""

	In this module are performed all the code transformations needed as workarounds to known backend bugs/issues, or
	useful transformations to make the code more simple and meet assumptions of modules invoked later
	(later modules can thus avoid to handle the full C syntax)

	Transformation 2 (ESBMC 1.21.1 bug - not implemented yet, TODO)
	   (x >= k)  --> (x == k || x >= k+1)

	Transformation 3 (to avoid problem with strict syntax checking by some backend)
			insert (void *) argument to thread prototypes when missing, for example:
			void *thread()  --> void *thread(void *)

	Transformations inherited from old merger (light pre-processing to avoid corner-cases):
		- add brackets to single-statements blocks

		- split declaration of variables of the same kind:
			int x,y; --> int x; int y;

		- split declaration of local variables + init value to two separate statements:
			int x = value; --> int x; x = value;

		- remove  if(!1) { .. }  and  if(0) { .. }

		- assign a name to anonymous structures:
			struct { int f1; char f2; ... }    -->   struct __anon_0 { int f1; char f2; ... }

	Transformation 4:
		structure->field            -->     (*structure).field
		structure->field->field     -->   (*(*structure).field).field    TODO: test/check this one properly

	Transformation 5:
		__cs_thread_local_variablename --> variablename[pthread_self()]

	Transformation 6:
		remove auto,inline,volatile,register from beginning of declarations.

Changelog:
    2015.01.13  bugfixes
    2014.12.09  further code refactory to match the new organisation of the CSeq framework
	2014.10.26  structure dereference workaround (transformation 4)
	2014.10.09  moved in this module all the transformations from  merger.py
	2014.06.05  added workaround #3 (see above)
	2014.02.25  switched to  module.Module  base class for modules


"""

import inspect, os, sys, getopt, time
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class workarounds(core.module.Translator):
	__threadLocals = []

	__parsingFunction = False
	currentAnonStructsCount = 0   # counts the number of anonymous structures (used to assign consecutive names)

	'''
	def countparams(self, funcname):
		count = 0

		if funcname not in self.Parser.varNames:
			return 0

		for var in self.Parser.varNames[funcname]:
			if self.Parser.varKind[funcname, var] == 'p':
				count += 1

		return count
	'''


	'''
	def visit_Typedef(self, n):
		s = super(self.__class__, self).visit_Typedef(n)
		return s+'\n'
	'''


	def visit_ID(self, n):
		if n.name in self.__threadLocals:
			return '__cs_thread_local_'+n.name+'[__cs_thread_index]'
		else:
			return n.name


	def visit_FuncDef(self, n):
		self.__parsingFunction = True

		decl = self.visit(n.decl)

		if decl.startswith('static '): decl = decl[7:]

		# workaround #1
		#if '__VERIFIER_atomic' in decl:
		#	decl = decl.replace('__VERIFIER_atomic', core.common.funcPrefixChange['__VERIFIER_atomic'], 1)

		# workaround #3: insert (void *) argument to thread prototypes when missing
		'''
		if n.decl.name in self.Parser.threadName:
			print "THREAD %s HAS %s params" % (n.decl.name, self.countparams(n.decl.name))
		'''
		if n.decl.name in self.Parser.threadName and  decl.endswith('()'):
			decl = decl[:decl.rfind('()')] + '(void *__cs_unused)'

		self.indent_level = 0
		body = self.visit(n.body)

		if n.param_decls:
			knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
			self.__parsingFunction = False
			return decl + '\n' + knrdecls + ';\n' + body + '\n'
		else:
			self.__parsingFunction = False
			return decl + '\n' + body + '\n'


	def visit_FuncCall(self, n):
		fref = self._parenthesize_unless_simple(n.name)
		#if fref.startswith('__VERIFIER_atomic'): fref = fref.replace('__VERIFIER_atomic', core.common.funcPrefixChange['__VERIFIER_atomic'], 1)

		return fref + '(' + self.visit(n.args) + ')'


	def visit_Decl(self, n, no_type=False):
		# no_type is used when a Decl is part of a DeclList, where the type is
		# explicitly only for the first delaration in a list.
		#
		s = n.name if no_type else self._generate_decl(n)

		if n.bitsize: s += ' : ' + self.visit(n.bitsize)

		# when an init expression is used,
		# remove it from the declaration statement and insert a separate
		# statement for the assignment.
		#
		assignmentStmt = ''

		if n.init:
			if isinstance(n.init, pycparser.c_ast.InitList):
				assignmentStmt = ' = {' + self.visit(n.init) + '}'
			elif isinstance(n.init, pycparser.c_ast.ExprList):
				assignmentStmt = ' = (' + self.visit(n.init) + ')'
			else:
				assignmentStmt = ' = ' + self.visit(n.init)

		# Transformation 6:
		# remove storage class keywords from any declaration,
		# as they are not used at all in this tool for verification.
		#
		if s.startswith('auto '): s = s[5:]
		if s.startswith('inline '): s = s[7:]
		if s.startswith('volatile '): s = s[9:]
		if s.startswith('register '): s = s[9:]

		# Split the declaration statement from initialization statement.
		#
		# Remember thread-local variables
		if (n.name is not None) and (n.name.startswith('__cs_thread_local_')):
			self.__threadLocals.append(n.name.replace('__cs_thread_local_', ''))

		'''   OMAROMAROMAR   '''
		if self.__parsingFunction and n.init:
			return s + '; ' + n.name + assignmentStmt
		else:
			return s + assignmentStmt

		return s + assignmentStmt


	def visit_If(self, n):
		cond = ''

		s = 'if ('
		cond = self.visit(n.cond)
		if n.cond: s += cond
		s += ')\n'

		# Eliminate dead code
		if cond == '0' or cond == '!1':
			return ''

		t = self._generate_stmt(n.iftrue, add_indent=True)

		if not t.startswith(self._make_indent()+ ('{\n')):   # always add brackets when missing
			t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		s += t

		if n.iffalse:
			s += self._make_indent() + 'else\n'
			e = self._generate_stmt(n.iffalse, add_indent=True)

			if not e.startswith(self._make_indent()+ ('{\n')):   # always add brackets when missing
				e = self._make_indent() + '{\n' + e + self._make_indent() + '}\n'

			s += e

		return s


	def visit_For(self, n):
		s = 'for ('

		if n.init: s += self.visit(n.init)

		s += ';'

		if n.cond: s += ' ' + self.visit(n.cond)

		s += ';'

		if n.next: s += ' ' + self.visit(n.next)

		s += ')\n'
		t = self._generate_stmt(n.stmt, add_indent=True)

		if not t.startswith(self._make_indent()+ ('{\n')):   # always add brackets when missing
			t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		return s+t


	def visit_Struct(self, n):
		# Assign a name to anonymous structs
		if n.name == None:
			n.name = 'anonstruct_' + str(self.currentAnonStructsCount)
			self.currentAnonStructsCount += 1

		return self._generate_struct_union(n, 'struct')


	def visit_StructRef(self, n):
		sref = self._parenthesize_unless_simple(n.name)

		ret = ''

		# workaround no. 4
		if n.type == '->': ret = ('(*' + sref + ').' + self.visit(n.field))
		else: ret = sref + n.type + self.visit(n.field)

		return ret


	def visit_While(self, n):
		s = 'while ('

		if n.cond: s += self.visit(n.cond)

		s += ')\n'

		t = self._generate_stmt(n.stmt, add_indent=True)

		if not t.startswith(self._make_indent()+ ('{\n')):   # always add brackets when missing
			t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		return s + t


	def visit_DoWhile(self, n):
		s = 'do\n'

		t = self._generate_stmt(n.stmt, add_indent=True)

		if not t.startswith(self._make_indent()+ ('{\n')):   # always add brackets when missing
			t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		s += t
		s += self._make_indent() + 'while ('

		if n.cond: s += self.visit(n.cond)

		s += ');'

		return s



