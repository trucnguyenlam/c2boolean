""" Lazy-CSeq Conditionextractor module
    written by Omar Inverso, the University of Southampton.
"""
VERSION = 'conditionextractor-0.0-2015.07.02'
#VERSION = 'extractor-0.0-2014.12.24'    # CSeq-1.0beta
#VERSION = 'extractor-0.0-2014.06.03'    # CSeq-Lazy-0.6: newseq-0.6a, newseq-0.6c, SVCOMP15
#VERSION = 'extractor-0.0-2014.03.14'
#VERSION = 'extractor-0.0-2014.02.25' (CSeq-lazy-0.2)
"""

Transformation from:

	       if (complex_cond) { block }
	       while (complex_cond) { block }
	       for (init; complex_cond; next) { block }

	respectively into:

	       _Bool tmp = complex_cond  if (tmp) { block }
	       _Bool tmp = complex_cond  while (tmp) { block; tmp = complex_cond }
	       _Bool tmp = complex_cond  for (init; tmp; next) { block; tmp = complex_cond }

	where an expression is considered to be complex iff
	it contains calls to functions which body is defined in the source.

Prerequisites:
	- no  do..while  loops,
	  they should have been transformed into equivalent  for  or  while  loops
	 (see module remover.py/dowhileconverter.py)

TODO:
	split complex lvalue assignments (etc) containing more than one function calls
	to many individual lvalue assignments each containing one single function call
	(module  inliner  assumes that for each assignment there is at most one function call).

Changelog:
    2015.07.17  fix exception call to module.ModuleError (Truc)
    2014.12.09  further code refactory to match the new organisation of the CSeq framework
	2014.03.14  further code refactory to match  module.Module  class interface
	2014.02.25  switched to  module.Module  base class for modules

"""

import os, sys, getopt, time
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.module, core.parser, core.utils


class conditionextractor(core.module.Translator):
	funcCallFound = False
	ifCondCount = whileCondCount = forCondCount = 0


	def visit_If(self, n):
		extraBlock = ''

		s = 'if ('

		if n.cond:
			self.funcCallFound = False
			cond = self.visit(n.cond)

			###if self.funcCallFound == True:
			if True:   # force temporary variables regardless of the complexity of the expression
				extraBlock = '_Bool __cs_tmp_if_cond_%s; __cs_tmp_if_cond_%s = (%s); ' % (self.ifCondCount, self.ifCondCount, cond)
				s += '__cs_tmp_if_cond_%s' % (self.ifCondCount)
				s = extraBlock + '\n' + self._make_indent() + s
				self.ifCondCount += 1
			else:
				s += cond

		s += ')\n'

		t = self._generate_stmt(n.iftrue, add_indent=True)

      	# add brakets for single non parenthesised statements after if
		if not t.startswith(self._make_indent()+ ('{\n')):
			t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		s += t

		if n.iffalse:
			s += self._make_indent() + 'else\n'
			e = self._generate_stmt(n.iffalse, add_indent=True)

      		# add brakets for single non parenthesised statements after if
			if not e.startswith(self._make_indent()+ ('{\n')):
				e = self._make_indent() + '{\n' + e + self._make_indent() + '}\n'
			s += e

		return s


	def visit_DoWhile(self, n):
		# Truc -- fix typo
		raise core.module.ModuleError("do..while loop in input code.")


	def visit_While(self, n):
		cond = ''
		extraBlock = ''

		s = 'while ('

		t = self._generate_stmt(n.stmt, add_indent=True)

		if n.cond:
			self.funcCallFound = False
			cond = self.visit(n.cond)

			if self.funcCallFound == True:
				extraBlock = '_Bool __cs_tmp_while_cond_%s; __cs_tmp_while_cond_%s = (%s); ' % (self.whileCondCount, self.whileCondCount, cond)
				s += '__cs_tmp_while_cond_%s' % (self.whileCondCount)
				s = extraBlock + '\n' + self._make_indent() + s
				s += ')\n'

		      	# add brakets for single non parenthesised statements after if
				if not t.startswith(self._make_indent()+ ('{\n')):
					t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

				t = t[:t.rfind('}')]
				t = t + self._make_indent() + '__cs_tmp_while_cond_%s = (%s);\n' % (self.whileCondCount, cond)
				t = t + self._make_indent() + '}'

				self.whileCondCount += 1
			else:
				s += cond
				s += ')\n'

		      	# add brakets for single non parenthesised statements after if
				if not t.startswith(self._make_indent()+ ('{\n')):
					t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

		return s + t


	def visit_For(self, n):
		init = cond = next = ''
		extraBlock = ''

		if n.init: init = self.visit(n.init)
		if n.next: next = self.visit(n.next)
		if n.cond: cond = self.visit(n.cond)

		t = self._generate_stmt(n.stmt, add_indent=True)

		if n.cond:
			self.funcCallFound = False

			if self.funcCallFound == True:
				extraBlock = '_Bool __cs_tmp_for_cond_%s; __cs_tmp_for_cond_%s = (%s);\n' % (self.forCondCount, self.forCondCount, cond) + self._make_indent()

		      	# add brakets for single non parenthesised statements after if
				if not t.startswith(self._make_indent()+ ('{\n')):
					t = self._make_indent() + '{\n' + t + self._make_indent() + '}\n'

				t = t[:t.rfind('}')]
				t = t + self._make_indent() + '__cs_tmp_for_cond_%s = (%s);\n' % (self.forCondCount, cond)
				t = t + self._make_indent() + '}'

				cond = '; __cs_tmp_for_cond_%s' % (self.forCondCount)
				self.forCondCount += 1

		s = 'for (%s; %s; %s)' % (init, cond, next)

		return extraBlock + s + t


	def visit_FuncCall(self, n):
		fref = self._parenthesize_unless_simple(n.name)
		args = self.visit(n.args)

		if fref in self.Parser.funcBody:
			self.funcCallFound = True

		if n.args is None:
			self.lastFuncParams = []

		inl = fref + '(' + args + ')'

		return inl














