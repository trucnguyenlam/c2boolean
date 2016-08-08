""" Lazy-CSeq boolean transformer module
"""
VERSION = '0.1'

"""

Transformation:
	- syntax transformation
	- variables transformation
		any type  -->   boolean type

	- C concurrent to Boolean concurrent
		POSIX thread --> boolean thread
		mutex   -->  atomic lock


Limitations:
	-

Prerequisites:
	- all functions should have been inlined, except:
		+ the main(),
		+ all thread functions,
		+ all __CSEQ_atomic_ functions,
		+ and function __CSEQ_assert
	- all loops should habe been unrolled
	- no two threads refers to the same thread function (use module duplicator.py)

TODO:
	- based on ../dev/labelerRCInterproc.py

Changelog:
	2016.08.08  Intial version

"""
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils


class booleantransformer(core.module.Translator):
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

