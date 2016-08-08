""" CSeq C Sequentialization Framework
	function tracker module
	
	written by Omar Inverso, University of Southampton.
"""
VERSION = 'preinstrumenter-2015.07.02' # merged with errorlabel-0.0-2015.06.25
#VERSION = 'preinstrumenter-0.0-2015.06.25'
"""

"""
import core.common, core.module
import pycparser.c_ast

class functiontracker(core.module.Translator):
	currentfunctionname = ''
	inputcoordstofunctions = {}

	def init(self):
 		self.addOutputParam('coordstofunctions')

	def loadfromstring(self, string, env):
		super(self.__class__,self).loadfromstring(string, env)
		self.setOutputParam('coordstofunctions', self.inputcoordstofunctions)

	def visit(self,node):
		if hasattr(node, 'coord') and self.currentInputLineNumber!=0 and self.currentfunctionname!='':
			#print "function %s, coords: %s" % (self.currentfunctionname,self.currentInputLineNumber)
			#print "function %s, origincoords: %s" % (self.currentfunctionname,self._mapbacklineno(self.currentInputLineNumber))
			self.inputcoordstofunctions[self._mapbacklineno(self.currentInputLineNumber)] = self.currentfunctionname

		s = super(self.__class__,self).visit(node)

		return s

	def visit_FuncDef(self,n):
		self.currentfunctionname = n.decl.name

		s = super(self.__class__, self).visit_FuncDef(n)
		self.currentfunctionname = ''

		return s


































