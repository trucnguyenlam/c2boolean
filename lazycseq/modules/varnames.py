""" CSeq C Sequentialization Framework
	scope-based variable renaming module
    
    written by Omar Inverso, University of Southampton.
"""
VERSION = 'varnames-0.0-2015.07.08'
#VERSION = 'varnames-0.0-2014.12.24'    # CSeq-1.0beta
#VERSION = 'varnames-0.0-2014.10.26'    # CSeq-Lazy-0.6: newseq-0.6a, newseq-0.6c, SVCOMP15
#VERSION = 'varnames-0.0-2014.10.26'
#VERSION = 'varnames-0.0-2014.10.15'
#VERSION = 'varnames-0.0-2014.03.14'
#VERSION = 'varnames-0.0-2014.03.08' first version (Cseq-Lazy-0.2)
"""

	This module performs variable renaming based on variable scope,
	so that no two functions share a variable id after it.

	to make function inlining easier:
	(doing so avoids future problems with the inliner module, see regression/102,103 )

	At the end of the renaming, the map of variable name changes
	is available in newIDs (useful for counterexamples,
	to translate back variable names to the corrisponding original name). 

	Transformation:
		int f(int P) {
			int L;
		}

	into:
		int f(int __cs_f_P) {
			int __cs_f_L;
		}

TODO:
	the new variables introduced should be guaranteed not to reuse existing symbols

Changelog:
	2015.07.08  map with variable renames returned as an output parameter
    2014.12.09  further code refactory to match the new organisation of the CSeq framework
	2014.10.27  different prefixes for local variables and function parameters
	2014.10.26  changed  __stack  to  stack  (to inherit stack handling from module.py)
	2014.10.15  removed visit() and moved visit call-stack handling to module class (module.py)
	2014.03.14  further code refactory to match  module.Module  class interface

"""

import inspect, os, sys, getopt, time
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class varnames(core.module.Translator):
	__debug = False
	__visitingStructRef = False   # to avoid considering struct fields as local variables

	prefix = '__cs_local_'        # prefix for local variables
	paramprefix = '__cs_param_'   # prefix for function params

	newIDs = {}   # mapping of old variable names to new variable names

	varmap = {}

	__currentFunction = ''

	__visitingParam = 0    # depth of params in a function prototype
	__visitingDecl = 0   
	__visitFuncDef = 0
	__visitStructUnionEnum = 0 

	def init(self):
		self.addOutputParam('varnamesmap')


	def loadfromstring(self, string, env):
		super(self.__class__, self).loadfromstring(string, env)
		self.setOutputParam('varnamesmap', self.varmap)
		#print str(self.newIDs).replace(', ','\n')


	def visit_Decl(self, n, no_type=False):
		# no_type is used when a Decl is part of a DeclList, where the type is
		# explicitly only for the first delaration in a list.
		#
		self.__visitingDecl += 1

		s = n.name if no_type else self._generate_decl(n)

		if n.bitsize: s += ' : ' + self.visit(n.bitsize)
		if n.init:
			if isinstance(n.init, pycparser.c_ast.InitList):
				s += ' = {' + self.visit(n.init) + '}'
			elif isinstance(n.init, pycparser.c_ast.ExprList):
				s += ' = (' + self.visit(n.init) + ')'
			else:
				s += ' = ' + self.visit(n.init)

		self.__visitingDecl -= 1

		return s


	def visit_FuncDef(self, n):
		self.__visitFuncDef += 1
		s = super(self.__class__, self).visit_FuncDef(n)
		self.__visitFuncDef -= 1
		return s


	def visit_ParamList(self, n):
		out = ''
		for i, p in enumerate(n.params):
			spacer = '' if i==0 else ', '
			self.__visitingParam += 1
			out += spacer + self.visit(p)
			self.__visitingParam -= 1

		#return ', '.join(self.visit(param) for param in n.params)
		return out

	def visit_Struct(self, n):
		self.__visitStructUnionEnum += 1
		s = super(self.__class__, self).visit_Struct(n)
		self.__visitStructUnionEnum -= 1
		return s

	def visit_Union(self, n):
		self.__visitStructUnionEnum += 1
		s = super(self.__class__, self).visit_Union(n)
		self.__visitStructUnionEnum -= 1
		return s

	def visit_Enum(self, n):
		self.__visitStructUnionEnum += 1
		s = super(self.__class__, self).visit_Enum(n)
		self.__visitStructUnionEnum -= 1
		return s


	def visit_Typedef(self, n):
		self.__visitStructUnionEnum += 1
		s = super(self.__class__, self).visit_Typedef(n)
		self.__visitStructUnionEnum -= 1
		return s


	def visit_FuncDef(self, n):
		self.__currentFunction = n.decl.name
		f =  super(self.__class__, self).visit_FuncDef(n)
		self.__currentFunction = ''
		return f


	def visit_StructRef(self, n):
		sref = self._parenthesize_unless_simple(n.name)
		oldvisitingStructRef = False
		self.__visitingStructRef = True
		if self.__debug: print "------- ------- ------- ------- ------- ------- VISITING STRUCT REF START (%s)" % sref
		retval = sref + n.type + self.visit(n.field)
		if self.__debug: print "------- ------- ------- ------- ------- ------- VIITING STRUCT REF END"
		self.__visitingStructRef = 	oldvisitingStructRef	
		return retval


	def visit_ID(self, n):
		prefix = ''

		#if n.name in self.Parser.varNames[self.__currentFunction] and self.Parser.varKind[self.__currentFunction,n.name] == 'p':
		if n.name in self.Parser.varNames[self.__currentFunction]:
			if self.__debug: print "visiting ID: [%s,%s]" % (self.__currentFunction,n.name)

		if (n.name in self.Parser.varNames[self.__currentFunction] and
		    self.__currentFunction != '' and 
		    not self.__visitingStructRef ):
		    #str(self.stack[len(self.stack)-2]) != 'StructRef' ):        # e.g. visiting ID: x->ID or x.ID (this is not a local var, but a field)
			if self.__debug: print "     local PARAMETER"
			if self.__debug: print "     stack: "+  str(self.stack) + '   prev:' + str(self.stack[len(self.stack)-2])

			prefix = self.newIDs[self.__currentFunction, super(self.__class__, self).visit_ID(n)] 

			self.varmap[prefix+super(self.__class__, self).visit_ID(n)] = super(self.__class__, self).visit_ID(n)
			#self.warn('%s -> %s' % (prefix+super(self.__class__, self).visit_ID(n), super(self.__class__, self).visit_ID(n)))
			
		return prefix + super(self.__class__, self).visit_ID(n)


	def _generate_type(self, n, modifiers=[]):
		""" Recursive generation from a type node. n is the type node.
		    modifiers collects the PtrDecl, ArrayDecl and FuncDecl modifiers
		    encountered on the way down to a TypeDecl, to allow proper
		    generation from it.
		"""
		typ = type(n)

		#~ print(n, modifiers)

		if typ == pycparser.c_ast.TypeDecl:
			s = ''
			if n.quals: s += ' '.join(n.quals) + ' '
			s += self.visit(n.type)

			# Local variables & parameter renaming.
			#
			# Variable name substitution only applies to local variables or parameters names within function prototypes
			# (thus, global variables and function names need to be excluded)
			#
			# case 1: level-0 function parameters (no remanimg for nested parameters)
			# case 2: local variable declaration (thus excluding functions, global vars, struct-enum-union fields, nested parameters)
			#
			if self.__visitingParam == 1:                          # case 1
				if self.__debug: print "SETTING NEWID for [%s,%s] (case I)" % (self.__currentFunction,n.declname)
				self.newIDs[self.__currentFunction,n.declname] = self.paramprefix + self.__currentFunction + '_'
				n.declname = (self.paramprefix + self.__currentFunction + '_' + n.declname) if n.declname else ''
			elif (self.__visitingParam == 0 and                    # case 2
			      self.__visitFuncDef == 0 and
			      n.declname not in self.Parser.funcName and 
				  #n.declname not in self.Parser.varNames[''] and 
				  self.__currentFunction != '' and 
				  self.__visitStructUnionEnum == 0):
				if self.__debug: print "SETTING NEWID for [%s,%s] (case II)" % (self.__currentFunction,n.declname)
				self.newIDs[self.__currentFunction,n.declname] = self.prefix + self.__currentFunction + '_'
				n.declname = self.prefix + self.__currentFunction + '_' + n.declname if n.declname else ''

			nstr = n.declname if n.declname else ''

			# Resolve modifiers.
			# Wrap in parens to distinguish pointer to array and pointer to
			# function syntax.
			#
			for i, modifier in enumerate(modifiers):
				if isinstance(modifier, pycparser.c_ast.ArrayDecl):
					if (i != 0 and isinstance(modifiers[i - 1], pycparser.c_ast.PtrDecl)):
						nstr = '(' + nstr + ')'
					nstr += '[' + self.visit(modifier.dim) + ']'
				elif isinstance(modifier, pycparser.c_ast.FuncDecl):
					if (i != 0 and isinstance(modifiers[i - 1], pycparser.c_ast.PtrDecl)):
						nstr = '(' + nstr + ')'
					nstr += '(' + self.visit(modifier.args) + ')'
				elif isinstance(modifier, pycparser.c_ast.PtrDecl):
					if modifier.quals:
						nstr = '* %s %s' % (' '.join(modifier.quals), nstr)
					else:
						nstr = '*' + nstr
			if nstr: s += ' ' + nstr
			return s
		elif typ == pycparser.c_ast.Decl:
			return self._generate_decl(n.type)
		elif typ == pycparser.c_ast.Typename:
			return self._generate_type(n.type)
		elif typ == pycparser.c_ast.IdentifierType:
			return ' '.join(n.names) + ' '
		elif typ in (pycparser.c_ast.ArrayDecl, pycparser.c_ast.PtrDecl, pycparser.c_ast.FuncDecl):
			return self._generate_type(n.type, modifiers + [n])
		else:		
			return self.visit(n)








