""" Lazy-CSeq memory converter module
"""
VERSION = '0.1'

"""

Transformation:
	- Convert pointer and pointer operation to READ and WRITE to a memory model

Limitations:
	-

Prerequisites:
	-

TODO:
	- Base on ../dev/labelerRC_MEM.py


Changelog:
	2016.08.08  Intial version

"""
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class memconverter(core.module.Translator):
	def init(self):
		''' Initialization of a module, please refer to core.module.Translator
			Use two function to set (optional) input or output of this module
		'''
		# self.addInputParam(self,id,description,datatype,default,optional)
		# self.addOutputParam(self,id,description='',datatype='')
		pass

	def loadfromstring(self, string, env):
		'''	Load string from the previous module (string) and perform translation
			please refer to core.module.Translator loadfromstring for more details
		'''
		super(self.__class__, self).loadfromstring(string, env)
