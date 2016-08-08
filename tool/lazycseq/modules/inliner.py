""" CSeq function inlining module
    written by Omar Inverso, Gennaro Parlato, University of Southampton.
"""
VERSION = 'inliner-0.0-2014.10.19'
# VERSION = 'inliner-0.0-2014.07.15'
#VERSION = 'inliner-0.0-2014.12.24'    # CSeq-1.0beta
#VERSION = 'inliner-0.0-2014.10.31'    # CSeq-Lazy-0.6: newseq-0.6a, newseq-0.6c, SVCOMP15
#VERSION = 'inliner-0.0-2014.10.28'
#VERSION = 'inliner-0.0-2014.03.14'
#VERSION = 'inliner-0.0-2014.03.06 (CSeq-Lazy-0.2)'
#VERSION = 'inliner-0.0-2014.02.27'
#VERSION = 'inliner-0.0-2014.02.25'
#VERSION = 'inliner-0.0-2013.12.02'
#VERSION = 'inliner-0.0-2013.10.24-Gennaro-Omar'

"""

Transformations:
    - inlining of all the function calls,
      for functions which body is defined (except main() and __CSEQ_atomic_ functions)

    - renames main() to main_thread()

    - in threads:
        - pthread_exit;  are converted into  goto thread_exit;  (pthread_exit() argument is ignored)
        - return;  and  return value;  are converted into  goto thread_exit;  (return value is ignored)
        - local variables are converted into static local variables (to hold the value across context switches)


Prerequisites:
    - no function calls in if, while, for conditions (e.g. if(f(g)), while(cond), ...) ???
     (use module extractor.py)
    - no overlapping variable names as in regression testcase 102
     (use module varnames.py)


Limitations:
    - pthread_exit() parameter is ignored
    - two function in the same expression, nested, e.g.: g(f(x));


Todo:
    - limit recursion depth (otherwise parsing recursive functions will give a python stack overflow)

    - handle f(g(x)):  g(x) is in n.args therefore at the moment would not be inlined?

    - rename labels (& corresponding gotos) in inlined blocks of code to avoid label duplication
     (use Parser.self.funcLabels)

Changelog:
    2015.10.19  fix in _inlineFunction
    2015.07.16  fix inlining function in a label statement (Truc)
    2015.07.15  fixed linemapping for inlined function blocks + expanded parameter passing (Truc)
    2014.12.09  further code refactory to match the new organisation of the CSeq framework
    2014.10.31  bugfix: when dealing with expressions such as: if(!f(x)) would inline the function twice
    2014.10.28  inlining optimization: ....
    2014.03.14  further code refactory to match  module.Module  class interface
    2014.03.09  bugfix: external module  varnames.py  to fix regression overlapping variable names (see regression/102,103 )
    2014.03.06  bugfix: inliner wrong handling array as parameters (see regression/100_inline_struct_array.c)
    2014.02.27  improved indentation in inlined blocks
    2014.02.25  switched to  module.Module  base class for modules
    2013.12.02  bugfix: local struct variables not converted into static struct variables (e.g. struct a --> static struct a;)

"""

import copy,re
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
from pycparser import c_ast
import core.common, core.module, core.parser, core.utils


class inliner(core.module.Translator):
    functionlines  = {}         # map function names to sets of line numbers
    linestofunctions = {}        # map from lines to function names

    ##__functionsToBeInlined = []     # ids of all the functions to be inlined
    currentFunction = ['']
    currentFunctionParams = []        # while parsing a function call, keeps the list of parameters used for the actual call

    inlinedStack = []                 # inlined function to add before a statement
    indexStack = []                   # current index (= functionname_inliningcountforthisfunction) used for labels and gotos

    parametersToRemoveStack = [[]]
    switchTo = {}

    __parsingStruct = False           #

    # old
    funcInlinedCount = {}             # number of times a function call has been inlined, by function


    def init(self):
        #self.addOutputParam('linestofunctions')
        pass


    def loadfromstring(self, string, env):
        super(self.__class__, self).loadfromstring(string, env)
        #self.setOutputParam('linestofunctions', self.linestofunctions)


    ''' Check whether or not the input source code has been fully inlined,
        i.e. whether every function defined in the original source code has been inlined,
        or the function inlining bound has been met.
    '''
    def inlined(self):
        pass


    def visit_UnaryOp(self, n):
        operand = self._parenthesize_unless_simple(n.expr)

        #print "N.OP     %s" % n.op
        #print "OPERAND: %s" % operand
        #print "STACK: %s" % str(self.parametersToRemoveStack[-1])

        #
        if n.op == 'p++':
            return '%s++' % operand
        elif n.op == 'p--':
            return '%s--' % operand
        elif n.op == 'sizeof':
            # Always parenthesize the argument of sizeof since it can be
            # a name.
            return 'sizeof(%s)' % self.visit(n.expr)
        elif n.op == '*' and self.switchTo.has_key(operand):
            return self.switchTo[operand]
        else:
            return '%s%s' % (n.op, operand)


    def visit_Compound(self, n):
        s = self._make_indent() + '{\n'
        self.indent_level += 1

        ##print "COMPOUND %s START   " % (self.indent_level)

        if n.block_items:
            for stmt in n.block_items:
                '''
                if hasattr(stmt, 'coord'):
                    print "COORDS: %s" % (stmt.coord )
                else:
                    print "COORDS NO"
                '''
                k = self._inlineIfNeeded(stmt)
                ##print "/ \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ /\n"
                #######print k
                ###print "\\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\\n"
                s += k

        ###print "COMPOUND %s END" % self.indent_level

        self.indent_level -= 1
        s += self._make_indent() + '}\n'

        return s


    def visit_ExprList(self, n):
        visited_subexprs = []

        for expr in n.exprs:
            if isinstance(expr, pycparser.c_ast.ExprList):
                visited_subexprs.append('{' + self.visit(expr) + '}')
            else:
                visited_subexprs.append(self.visit(expr))

        if visited_subexprs not in self.currentFunctionParams:
            self.currentFunctionParams.append(visited_subexprs)

        return ', '.join(visited_subexprs)


    def visit_FuncDef(self, n):
        # Function definitions of inlined functions must disappear (except thread functions).
        #
        #if n.decl.name in self.__functionsToBeInlined and n.decl.name not in self.Parser.threadName:  OMAROMAROMAROMAR
        if self.____needsInlining(n.decl.name) and n.decl.name not in self.Parser.threadName and n.decl.name not in self.Parser.funcReferenced:
            ##return 'int __cs_function_%s_inlined = 1;\n' % n.decl.name;
            return ''

        self.currentFunction.append(n.decl.name)

        decl = self.visit(n.decl)
        self.indent_level = 0
        body = self.visit(n.body)

        # At the bottom of each thread, add a pthread_exit() statement
        #
        returnStmt = ''

        if (self.currentFunction[-1] in self.Parser.threadName or self.currentFunction[-1] == 'main'):
            returnStmt = self.INDENT_SPACING + '__exit_%s: ; %s(0);\n' % (self.currentFunction[-1], core.common.changeID['pthread_exit'])

        # Continue the visit.
        if n.param_decls:
            knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
            body = body[:body.rfind('}')] + self._make_indent() + returnStmt + '}'
            block = decl + '\n' + knrdecls + ';\n' + body + '\n'
        else:
            body = body[:body.rfind('}')] + self._make_indent() + returnStmt + '}'
            block = decl + '\n' + body + '\n'


        self.currentFunction.pop()

        return block


    '''
    '''
    def visit_FuncCall(self, n):
        self.currentFunctionParams = []

        fref = self._parenthesize_unless_simple(n.name)

        #print "function call: %s" % fref
        #print "stack: %s" % str(self.stack)
        #print "\n\n"

        # Pthread exit()s can only be within thread functions,
        # no need to check whether we're in a thread.
        #
        if fref == core.common.changeID['pthread_exit']:  # pthread_exit() param is ignored for the moment
            return 'goto __exit_%s ' % (self.currentFunction[-1])

        args = self.visit(n.args)

        s = fref + '(' + args + ')'

        if n.args is None:
            self.currentFunctionParams.append([])

        if self.____needsInlining(fref):
            if fref not in self.funcInlinedCount:
                self.funcInlinedCount[fref] = 0

            self.funcInlinedCount[fref] += 1
            self.indexStack.append('_%s_%s' % (fref, self.funcInlinedCount[fref]))

            self.inlinedStack[-1] += (self._inlineFunction(self.Parser.funcASTNode[fref], n, False))+'\n'

            if self.Parser.funcIsVoid[fref]: s = 'DELETETHIS'
            else: s =  '__cs_retval_%s' % (self.indexStack[-1])

            self.indexStack.pop()

        return s


    '''
    '''
    '''
    def visit_Return(self, n):
        if self.currentFunction[-1] in self.Parser.threadName:
            return 'goto __exit_%s; /* return stmt */' % (self.currentFunction[-1])
            #return 'goto _RETURN_exit_%s_%s;' % (self.currentFunction, self.funcInlinedCount[self.currentFunction])
        elif self.currentFunction[-1] == 'main':
            return 'goto __exit_main;  /* return stmt in main() */'

        s = 'return'
        if n.expr: s += ' ' + self.visit(n.expr)

        return s + ';'
    '''


    def visit_Return(self, n):
        if len(self.indexStack) > 0:
            if self.Parser.funcIsVoid[self.currentFunction[-1]]:
                #print "function name %s  -  is void\n\n" % self.currentFunction[-1]
                return 'goto __exit_%s;' % (self.indexStack[-1]) # void
            else:
                #print "function name %s  -  is not void (%s)\n\n" % (self.currentFunction[-1], self.Parser.funcBlockOut[self.currentFunction[-1]])
                return '__cs_retval_%s = %s; goto __exit_%s;' % (self.indexStack[-1], self.visit(n.expr), self.indexStack[-1]) # non-void

        if self.currentFunction[-1] in self.Parser.threadName:
            #~return 'goto __exit_%s; /* return stmt */' % (self.currentFunction[-1])
            return 'goto __exit_%s; ' % (self.currentFunction[-1])
            #return 'goto _RETURN_exit_%s_%s;' % (self.currentFunction, self.funcInlinedCount[self.currentFunction])
        elif self.currentFunction[-1] == 'main':
            #~return 'goto __exit_main;  /* return stmt in main() */'
            return 'goto __exit_main; '


        s = 'return'
        if n.expr: s += ' ' + self.visit(n.expr)
        return s + ';'


    ''' TODO: labels inside inlined functions must be indexed using  indexStack
    '''
    '''
    def visit_Label(self, n):
        if self.currentFunction in self.__functionsToBeInlined:
            return n.name + self.indexStack[-1] + ':\n' + self._generate_stmt(n.stmt)
        else:
            return n.name + ':\n' + self._generate_stmt(n.stmt)
    '''

    ''' TODO gotos-to-labels inside inlined functions must be indexed using  indexStack
    '''
    '''
    def visit_Goto(self, n):
        if self.currentFunction in self.__functionsToBeInlined:
            return 'goto ' + n.name + self.indexStack[-1] + '; /* updated label index from previous goto stmt */'
        else:
            return 'goto ' + n.name + ';'
    '''


    def visit_Struct(self, n):
        #
        oldParsingStruct = self.__parsingStruct
        self.__parsingStruct = True
        s = self._generate_struct_union(n, 'struct')
        self.__parsingStruct = oldParsingStruct

        return s


    def visit_Decl(self, n, no_type=False):
        # no_type is used when a Decl is part of a DeclList, where the type is
        # explicitly only for the first delaration in a list.
        #
        s = n.name if no_type else self._generate_decl(n)

        if n.bitsize: s += ' : ' + self.visit(n.bitsize)

        # Change local variables to be static vars,
        # needed for this particular encoding to remember the old values of local variables
        # between simulated context switches.
        #
        # If the variable is scalar or it is an array of fixed size, then just add  static  to its declaration.
        # If the variable is an array of non fixed size, then change it to a static pointer and adds a call to malloc() to complete the initialization,
        # (e.g.    int x[size];  -->  static int * x; x = (int *)malloc(sizeof(int)*size);  )
        #
        # TODO: init_scalar()/malloc() should not be called when variables have init expressions!
        #

        if (isinstance(n, c_ast.Decl) and
            self.currentFunction[-1] != '' and
            self.indent_level > 0 and
            not s.startswith('static ') and
            not self.__parsingStruct):

            if (self.__isScalar(self.currentFunction[-1], n.name) or self.__isStruct(self.currentFunction[-1], n.name)) and not self.Parser.varInitExpr[self.currentFunction[-1], n.name]:
            #if self.__isScalar(self.currentFunction[-1], n.name) and not self.Parser.varInitExpr[self.currentFunction[-1], n.name]:
                s = 'static ' + s + '; __cs_init_scalar(&%s, sizeof(%s))' % (n.name, self.Parser.varType[self.currentFunction[-1], n.name])
                #s = 'static ' + s + '; malloc(&%s, sizeof(%s))' % (n.name, self.Parser.varType[self.currentFunction[-1], n.name])
            elif self.__isScalar(self.currentFunction[-1], n.name) and self.Parser.varInitExpr[self.currentFunction[-1], n.name]:
                s = 'static ' + s
            elif self.__isArray(self.currentFunction[-1], n.name):
                stars = '*' * self.Parser.varArity[self.currentFunction[-1], n.name]
                vartype = self.Parser.varType[self.currentFunction[-1],n.name]

                s = 'static %s %s %s; ' % (self.Parser.varType[self.currentFunction[-1], n.name], stars, n.name)
                s += n.name + ' = (%s %s)malloc(sizeof(%s)*%s); __CSEQ_assume(%s)' % (vartype, stars, vartype, self._totalSize(self.currentFunction[-1], n.name), n.name)

            '''
            if self._hasFixedSize(self.currentFunction[-1], n.name) and n.name in self.Parser.varNames[self.currentFunction[-1]]:
                s = 'static ' + s
            else:
                stars = '*' * self.Parser.varArity[self.currentFunction[-1], n.name]
                vartype = self.Parser.varType[self.currentFunction[-1],n.name]

                s = 'static %s %s %s; ' % (self.Parser.varType[self.currentFunction[-1], n.name], stars, n.name)
                #s += n.name + ' = (%s %s)malloc(sizeof(%s)*%s); __CSEQ_assume(%s)' % (vartype, stars, vartype, self._totalSize(self.currentFunction[-1], n.name), n.name)
                s += n.name + ' = (%s %s)__cs_safe_malloc(sizeof(%s)*%s)' % (vartype, stars, vartype, self._totalSize(self.currentFunction[-1], n.name))
            '''

        if n.init:
            if isinstance(n.init, c_ast.InitList):
                s += ' = {' + self.visit(n.init) + '}'
            elif isinstance(n.init, c_ast.ExprList):
                s += ' = (' + self.visit(n.init) + ')'
            else:
                s += ' = ' + self.visit(n.init)

        return s

    # def visit_Label(self, n):
    #     # Truc (method 1: simply add an empty statement)
    #     return n.name + ':;\n' + self._generate_stmt(n.stmt)

    ########################################################################################

    def _inlineIfNeeded(self, stmt):
        # Truc comment this for method 2
        # self.inlinedStack.append('')

        # original = self._generate_stmt(stmt)
        # original = original.replace('DELETETHIS;\n', '')
        # original = self.inlinedStack[-1] + original

        # self.inlinedStack.pop()

        # Truc (method 2: Identify inlined function call by inlinedStacked
        # and change things according to type of statements)
        self.inlinedStack.append('')

        original = ''
        if type(stmt) == pycparser.c_ast.Label:
            label = stmt.name
            original = self._generate_stmt(stmt.stmt)
            if self.inlinedStack[-1] == '':  # If this statement doesn't contain inlined function
                original = label + ':\n' + original
            else:
                original = original.replace('DELETETHIS;\n', '')
                original = label + ':;\n' + self.inlinedStack[-1] + original
        else:
            original = self._generate_stmt(stmt)
            original = original.replace('DELETETHIS;\n', '')
            original = self.inlinedStack[-1] + original
        self.inlinedStack.pop()

        return original


    ''' Generate the function body,
        for either including it in a function definition, or
        for inserting it into a statement
    '''
    def _inlineFunction(self, n, fcall_ast_node, simple):
        fInput = fOutput = ''
        fref = n.decl.name
        #print "inlining function:%s %s" % (fref, str(self.currentFunctionParams))
        # Simulate input parameter passing.
        #
        # Build argument initialization statement(s) if needed, to simulate parameter passing
        # (see transformation details below)
        #
        # args = self.visit(fcall_ast_node.args)  # ?????

        # Analysis of function-call parameters
        #
        self.parametersToRemoveStack.append([])
        self.switchTo = {}

        if fcall_ast_node.args is not None:
            paramNo = -1

            #
            #
            for expr in fcall_ast_node.args.exprs:   # for each parameter in the function call
                paramNo += 1
                if (type(expr) == pycparser.c_ast.UnaryOp and
                    expr.op == '&' and
                    expr.expr.name not in self.Parser.varNames[self.currentFunction[-1]] and
                    expr.expr.name in self.Parser.varNames[''] and
                    len(self.Parser.varOccurrence[fref, self.Parser.funcParams[fref][paramNo]]) - len(self.Parser.varDeReferenced[fref,self.Parser.funcParams[fref][paramNo]]) == 0):
                    #print "varname: %s     currentscope:%s    currentfinlined:%s    parameterno:%s" % (expr.expr.name, self.currentFunction[-1], fref, paramNo)
                    #print "variable %s is global and referenced!!" % expr.expr.name
                    #print "the corrseponding function parameter is %s" % (self.Parser.funcParams[fref][paramNo])
                    #print "is it always dereferenced? %s %s" % (len(self.Parser.varOccurrence[fref, self.Parser.funcParams[fref][paramNo]]), len(self.Parser.varDeReferenced[fref,self.Parser.funcParams[fref][paramNo]]) )
                    #print "\n"
                    #exit(12345)
                    ##print "REMOVE reference to global variable '&%s' from the fuction call!!!!" % expr.expr.name
                    self.parametersToRemoveStack[-1].append('&'+expr.expr.name)  # parameter  expr.expr.name  in the call to  fref()  can to be removed
                    ##print "IN THE FUNCTION BODY CHANGE (*%s) -> %s" % (self.Parser.funcParams[fref][paramNo], expr.expr.name)
                    self.switchTo[self.Parser.funcParams[fref][paramNo]] = expr.expr.name;

        if fcall_ast_node.args is not None:
            i = 0

            for p in self.Parser.varNames[fref]:
                if self.Parser.varKind[fref,p] == 'p':
                    #print "parameters to remove %s" % str(self.parametersToRemoveStack[-1])
                    #print "p = %s" % p
                    #print "\n\n"

                    if self.currentFunctionParams[-1][i] in self.parametersToRemoveStack[-1]:
                        i += 1
                        continue  # this parameter is not needed
                    if not self.__isPointerToFunction(fref,p) and not self.__isArray(fref,p):
                        ##print "    p       %s " % p
                        ##print "    fref    %s" % fref
                        ##print "    type    %s" % self.Parser.varTypeUnExpanded[fref,p]
                        ##print "    param   %s\n" % self.currentFunctionParams[-1][i]
                        fInput += 'static %s %s; %s = %s; ' % (self.Parser.varTypeUnExpanded[fref,p], p, p, self.currentFunctionParams[-1][i])
                        i += 1
                    elif not self.__isPointerToFunction(fref,p) and self.__isArray(fref,p):
                        varSize = ''
                        stars = ''
                        '''
                        for s in self.Parser.varSize[fref,p]:
                            if s != -1: varSize += '[%s]' % s
                            else: varSize += '[]'
                        '''
                        for s in self.Parser.varSize[fref,p]:
                            #varSize += '[%s]' % (s if s != -1 else '')
                            #####varSize += '[]'   # ignore the size for array passed as function parameters
                            stars += '*'

                        #####fInput += 'static %s %s%s; %s = %s; ' % (self.Parser.varTypeUnExpanded[fref,p], p, varSize, p, self.currentFunctionParams[-1][i])
                        fInput += 'static %s %s%s; %s = %s; ' % (self.Parser.varTypeUnExpanded[fref,p], stars, p, p, self.currentFunctionParams[-1][i])
                    else:
                        x = self.Parser.varTypeUnExpanded[fref,p].replace('(*)', '(*%s)' % p)
                        fInput += 'static %s; %s = %s; ' % (x, p, self.currentFunctionParams[-1][i])
                        i += 1


        # Simulate output parameter returning.
        #

        if not self.Parser.funcIsVoid[fref]:
            fOutput = 'static %s __cs_retval_%s;\n' % (self.Parser.funcBlockOut[fref], self.indexStack[-1])
        else:    # simple function call without assignment (e.g. f(x);)
            fOutput = ''
        # Truc - dirty fix, just inlude the line map of that function call
        fOutput = self._getCurrentCoords(fcall_ast_node) + '\n' + fOutput

        # Transform the function body by:
        #
        #   1. adding the initialization statement(s) (if any) at the top
        #   2. adding one exit label at the bottom where to jump to in order to simulate return statements
        #   3. change return statements to goto statements pointing to the exit label added in previous step
        #   4. all the rest is unchanged
        #

        # body (adds one indent each line)
        self.currentFunction.append(fref)
        #inlined = self._shiftIndent(self.visit(self.Parser.funcASTNode[fref].body))

        # save the old length so after the inlining self.lines can be trimmed back to its contents before the inlining,
        # this removes the elements added while inlining,
        # otherwise when inlining the same function more than once,
        # the linemapping is only generated on the first inlined function call.
        oldlineslen = len(self.lines)
        inlined = self.visit(self.Parser.funcASTNode[fref].body)
        self.functionlines[fref] = self.lines[oldlineslen:]
        self.lines = self.lines[:oldlineslen]

        # top
        #~inlined = inlined.replace(self.INDENT_SPACING+'{', '/*** INLINING START %s ***********************************/\n' % fref + self.INDENT_SPACING + fOutput + self._make_indent() +'{\n' + self._make_indent() + fInput, 1)
        inlined = inlined[inlined.find('{')+1:]
        addedheader = self.INDENT_SPACING + fOutput + self._make_indent() + '{\n' + self._make_indent(1) + fInput
        inlined = addedheader + inlined

        # bottom
        inlined = inlined[:inlined.rfind('}')] + '%s __exit_%s: ;  \n' % (self._make_indent(1), self.indexStack[-1]) + self._make_indent() +'}\n'
        #~inlined += '\n' + self._make_indent() + '/*** INLINING END %s **************************************/' % fref

        self.parametersToRemoveStack.pop()
        self.currentFunction.pop()

        return inlined


    # Shift one indent each line.
    #
    def _shiftIndent(self, s):
        new = ''

        for line in s.splitlines():
            new += self.INDENT_SPACING+ line + '\n'

        return new


    ''' Check whether variable  v  from function  f  has a fixed size,
        or not (e.g.  int x[expr]   with expr not constant.
    '''
    def _hasFixedSize(self, f, v):
        if self.Parser.varArity[f,v] > 0:
            for i in range(0, self.Parser.varArity[f,v]):
                if not self.Parser.varSize[f,v][i].isdigit():
                    return False

        return True


    ''' Return the total size of a given array in a string,
        as the expression of the product of all sizes.

        For example:

            int x[10][expr][3];

        returns:

            size = 10*(expr)*30;
    '''
    def _totalSize(self, f, v):
        sizeExpression = ''

        for i in range(0, self.Parser.varArity[f,v]):
            #if self.Parser.varSize[f,v][i].isdigit():     # simple digit
            sizeExpression += str(self.Parser.varSize[f,v][i]) + '*'

        sizeExpression = sizeExpression[:-1]

        return sizeExpression


    # Checks whether variable  v  from function  f  is an array.
    #
    def __isArray(self, f, v):
        if self.Parser.varArity[f,v] > 0:
            return 1
        else:
            return 0


    # Checks whether variable  v  from function  f  is scalar.
    # TODO redo properly at parser-level
    #
    def __isScalar(self, f, v):
        if self.Parser.varArity[f, v] == 0  and not self.Parser.varType[f,v].startswith('struct ') and not self.Parser.varType[f,v].startswith('union '):
            return 1
        else:
            return 0


    # Checks whether variable  v  from function  f  is a struct.
    # TODO redo properly at parser-level
    #
    def __isStruct(self, f, v):
        result = 0

        if self.Parser.varType[f, v].startswith('struct '):
            result = 1

        return result


    def __isPointerToFunction(self, f, v):
        if (f,v) in self.Parser.varPtrToFunct: return True
        else: return False
        #if '(*)' in self.Parser.varType[f,v]: return True
        #else: return False


    ''' Check whether function  f  needs to be inlined.
    '''
    def ____needsInlining(self, f):
        return (f in self.Parser.funcBlock and    # defined functions need to be inlined when called (if at all)
                not f.startswith('__CSEQ_atomic') and
                #not f.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']) and
                not f == '__CSEQ_assert' and
                ##f not in self.Parser.threadName and
                f != '' and
                f != 'main')




