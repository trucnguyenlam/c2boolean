"""
    Lazy-CSeq
    Copyright 2011-2015 Truc Nguyen Lam, University of Southampton.

	>  Labeler module with CBA main driver (rather than RBA main driver - forked from CAV version) <
	>    variant III  - version optimised for performance but not for schedule partitioning  <

	NOTE NOTE NOTE:
	   linemapping is not stable and it has been disabled, look for the following statement in the code:
	   marker = outputcode[outputcode.rfind('#'):][:outputcode.find('\n')+1]


"""
VERSION = 'labeler-0.0-2015.05.07' # improved main driver constraints

"""

Transformation:
	- inserting labels at the beginning of each relevant line of code
	- instrumentation

Prerequisites:
	- all functions should have been inlined, except the main(), all thread functions, all __VERIFIER_atomic_ functions, and function __VERIFIER_assert
	- all loops should habe been unrolled
	- no two threads refers to the same thread function (use module duplicator.py)

TODO:
	- check the STOP() inserting mechanism
	- calculate max length of added stamps, and change label padding accordingly

Changelog:
	2015.05.07  rearranged constraints in main driver, added one extra constraint that speeds the analysis up
    2015.04.23  __globalAccess()  was causing  if(cond)  blocks to disappear
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
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class assignmentsplitter(core.module.Translator):
	__split = False
	__currentFunction = ''             # name of the current thread (also used to build thread labels)
	__globalMemoryAccessed = False   # used to limit context-switch points (when global memory is not accessed, no need to insert them)
	__atomicSection = False

	# data structs for partial-order reduction
	__visitingLRvalue = None   # set to None, L, R, or LR depending on the current assignment statement being visited (None, Lvalue, Rvalue, LR when visiting the left part in a += stmt etc.)
	__sharedVarsW = []       # set of shared vars in the current statement being visited
	__sharedVarsR = []       # set of shared vars in the current statement being visited

	def init(self):
		self.addInputParam('atomicAssignment', 'whether an assignment is treated atomically', '', False, True)


	def loadfromstring(self, string, env):
		if 'atomicAssignment' not in env.paramvalues:
			self.__split = False
		else:
			self.__split = True
		super(self.__class__, self).loadfromstring(string, env)


	def visit_FuncDef(self, n):
		self.__currentFunction = n.decl.name
		decl = self.visit(n.decl)
		self.indent_level = 0
		body = self.visit(n.body)
		self.__currentFunction = ''
		if n.param_decls:
			knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
			return decl + '\n' + knrdecls + ';\n' + body + '\n'
		else:
			return decl + '\n' + body + '\n'


	def visit_Assignment(self, n):
		if (self.__split or self.__atomicSection):
			rvalue = self._parenthesize_if(n.rvalue, lambda n: isinstance(n, pycparser.c_ast.Assignment))
			lvalue = self.visit(n.lvalue)
			ret = '%s %s %s' % (lvalue, n.op, rvalue)
			return ret

		LGlobalAccess = False
		RGlobalAccess = False

		self.__sharedVarsR = []
		self.__sharedVarsW = []

		self.__globalMemoryAccessed = False
		oldVisitingLRvalue = self.__visitingLRvalue
		if n.op == '=': self.__visitingLRvalue = 'R'
		rvalue = self._parenthesize_if(n.rvalue, lambda n: isinstance(n, pycparser.c_ast.Assignment))
		self.__visitingLRvalue = oldVisitingLRvalue
		RGlobalAccess = self.__globalMemoryAccessed

		self.__globalMemoryAccessed = False
		oldVisitingLRvalue = self.__visitingLRvalue
		if n.op == '=': self.__visitingLRvalue = 'L'
		# if n.op in ('+=', '-=', '/='): self.__visitingLRvalue = 'LR'
		lvalue = self.visit(n.lvalue)
		self.__visitingLRvalue = oldVisitingLRvalue
		LGlobalAccess = self.__globalMemoryAccessed

		##print "visiting assignent  <%s>  <%s>" % (rvalue, lvalue)
		##print "    stack:   <<< %s >>>" % self.stack
		##print "    LRvalue: <<< %s >>>" % self.__visitingLRvalue
		ret = '%s %s %s' % (lvalue, n.op, rvalue)

		if LGlobalAccess and RGlobalAccess:
			# print "This is the assignment need to be split", ret
			pass

		# print str((globalAccess, self.__sharedVarsR, self.__sharedVarsW))

		return ret

	def __sharedVarsREAD(self, var):
		if var not in self.__sharedVarsR:
			#print "new variable READ discovered! %s " % var
			self.__sharedVarsR.append(var)

	def __sharedVarsWRITE(self, var):
		if var not in self.__sharedVarsW:
			#print "new variable WRITE discovered! %s " % var
			self.__sharedVarsW.append(var)


	def visit_ID(self, n):
		# If this ID corresponds either to a global variable,
		# or to a pointer...
		#
		if ((self.__isGlobal(self.__currentFunction, n.name) or
				self.__isPointer(self.__currentFunction, n.name)) and not n.name.startswith('__cs_thread_local_')):
			#print "variable %s in %s is global\n" % (n.name, self.__currentFunction)
			self.__globalMemoryAccessed = True

		if self.__currentFunction != '' and self.__isGlobal(self.__currentFunction, n.name) and len(self.Parser.varReferenced['', n.name]) == 0:
			index = '' if self.stack[-2] != 'ArrayRef' else ('[' + self.visit(self.stacknodes[-2].subscript) + ']') # extract array index(es) if any

			#print "found ID:<%s>"  % n.name
			#print "   stack:<%s> prev:<%s>" % (self.stack,  self.stack[-2])
			#print "  L or R:<%s>" % self.__visitingLRvalue
			#print "       f:<%s>" % self.__currentFunction
			#print "   array:<%s>" % (self.stack[-2] == 'ArrayRef')
			#print "   index:<%s>" % index
			#print ""

			# global variable at the left in an assignment statement:   var = ...;
			if self.__visitingLRvalue == 'L': ## and self.stack[-2] == 'Assignment':  # this would exclude arrays
				##print "---------->  global variable '%s' is written" % n.name
				self.__sharedVarsWRITE(n.name+index)
			# global variable at the left in a compound operator assignment:   var += ...;
			elif self.__visitingLRvalue == 'LR':
				##print "---------->  global variable '%s' is read & written" % n.name
				self.__sharedVarsWRITE(n.name+index)
				self.__sharedVarsREAD(n.name+index)
			# global variable at the right in an assignment statement:   ... = var;
			elif self.__visitingLRvalue == 'R' and self.stack[-2] != 'UnaryOp':
				##print "---------->  global variable '%s' is read" % n.name
				self.__sharedVarsREAD(n.name+index)
			# any other case it is assumed to be a read (references/dereferences are excluded by the top-level if)
			else:
				##print "---------->  global variable '%s' is read" % n.name
				self.__sharedVarsREAD(n.name+index)

		return n.name


	def visit_FuncCall(self, n):
		fref = self._parenthesize_unless_simple(n.name)
		if fref == "__CS_atomic_begin":
			self.__atomicSection = True
		elif fref == "__CS_atomic_end":
			self.__atomicSection = False

		args = self.visit(n.args)
		return fref + '(' + args + ')'


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



