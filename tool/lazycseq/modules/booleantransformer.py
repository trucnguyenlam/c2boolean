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

Description of Concurrent Boolean programs:

	/* Global variable declaration */
	decl atomic,
		g1,
		g2,
		...;

	/* Init function */
	void init ()
	begin
		/* statements, separate by ; */
		endinit: skip;
	end

	/* Thread function, no definition of main thread, every thread starts simultaneously after init */
	void threadXYZ()
	begin
		/* statements */
		/* To write an atomic statement, please wrap the statement inside
			atomic := T;
			// stmt
			atomic := F;
		*/
	end

	/* Atomic function */
	<return_value(s)> atomic_function(parameters)
	begin

	end

	/*  SLIC_ERROR is the label for checking reachability */

Limitations:
	-

Prerequisites:
	- all functions should have been inlined, except:
		+ the main(),
		+ all thread functions,
		+ all __CSEQ_atomic_ functions,
		+ and function __CSEQ_assert
	- all loops should have been unrolled
	- no two threads refers to the same thread function (use module duplicator.py)
	- information can be retrieved through self.Parser.<attribute> (please refer to core/parser.py for more details)

TODO:
	- based on ../dev/labelerRCInterproc.py

	- Translation from C program (please refer to the above Prerequisites) into concurrent Boolean program:

		-- Thread/Function level --
		+ Translation of threads
			* normal thread
			* main thread
		+ Translation of init
		+ Translation of atomic functions (with __CSEQ_atomic_ prefix)

		-- Statement level --
		+ Translation of declaration statement
		+ Translation of pthread primitive:
			* thread_create/ thread_join
			* mutex lock/unlock
		+ All other type of statements, e.g. assignment, call, if..else, ...
	 	+ Wrap visible statement (cav13-lazycseq) inside atomic block (context switch)


	 	-- Expression level --
	 	+ arithmetic operation
	 	+ access to global variable (write or read) - handle in the future

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
		# the translation start from this point
		# this call ignites the translation following any custom generator function (visit_<NODE>) we write
		super(self.__class__, self).loadfromstring(string, env)

		# after the above call, we obtain the output in self.output
		# we can do any post process thing on self.output


	# TODO: write custom translator, please refer to dev/c_generator.py to know how a node can be rewritten (in C)
	#