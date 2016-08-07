import os, sys, getopt, time
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.module, core.parser, core.utils


class prova(core.module.Translator):
	def visit_UnaryOp(self, n):
		operand = self._parenthesize_unless_simple(n.expr)

		if n.op == 'p++':
			return '%s = %s + 1' % (operand, operand)
		elif n.op == 'p--':
			return '%s--' % operand
		elif n.op == 'sizeof':
			# Always parenthesize the argument of sizeof since it can be
			# a name.
			return 'sizeof(%s)' % self.visit(n.expr)
		else:
			return '%s%s' % (n.op, operand)

