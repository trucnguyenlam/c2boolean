import sys
import math
import re
import copy
import time
from time import gmtime, strftime

import pycparser.c_parser
import pycparser.c_ast
import pycparser.c_generator

import core.common
import core.module
import core.parser
import core.utils

import mem_config
from module_utils import eval_expr, comment_out

"""
    CSeq
    Copyright 2011-2015 Truc Nguyen Lam, Gennaro Parlato, University of Southampton.

    >  LabelerRC with MEMORY MODEL <

"""
VERSION = 'labelerRC_MEM-0.1-2015-November'
"""

Transformation:
    1. Inserting label
        - inserting labels at the beginning of each relevant line of code
        - instrumentation
        - renames main() to main_thread()
    2. Transformation of memory:
        - All structs and arrays are turned into address
        - For struct:
            + field is indexed by consecutive address
            + if field is a static array, calculate all the size (do not really know better way)
            + if field is a pointer (it must be created by some malloc)
            + if field is a struct: +1 for the address of that struct and recursively follow
        - For array:
            + One dimensional array: the same as the semantics of C
        - All pointers are removed from the code

    4. Grammar translation:
        Assuming we have static malloc in which we already know the sizeof that malloc statically.
        All the SCALAR (global, pointer, locals) are reserved in the first place (as it appears in the program)
        All the mallocs are reserved in the remaining space, as it appears

        INDEX ARRAY
        -------------------------------------------------------------------------------
        |  0  |  1  |  2  |  3  |  4  |     |     |     |     |     |     |     |     |
        -------------------------------------------------------------------------------
                                       |
        VALUE ARRAY
        -------------------------------------------------------------------------------
        |  0  |  1  |  2  |  3  |  4  |     |     |     |     |     |     |     |     |
        -------------------------------------------------------------------------------
                                       |
        \-------------\/--------------/\-----------------------\/---------------------/
                scalars                                mallocs
        MEMORY

        Here: p, q are pointers, a is an array, index is an experssion, v is a variable
        i. p = q: assignment of pointer
            := value(p) = value(q)
        ii. *p
            := value(value(p))
        iii. a[index]
            := a + index*sizeof(element)
        iv.
            a.  s.f
                := s + offset(f)
            b.  ps->f
                := value(ps) + offset(f)
        v. &v
            := index(v)

Prerequisites:
    - all functions should have been inlined, except
        + the main(),
        + all thread functions,
        + all __VERIFIER_atomic_ functions,
        + and function __VERIFIER_assert
    - all loops should habe been unrolled
    - no two threads refers to the same thread function (use module duplicator.py)

TODO:
    - converting void* type, look for cast expression on that variable
    - disable memory translation in local variable and parameters
    - parameters in function passing (when there is a function call)
    - choose between define and value (should now allow declaration of variable in the first case)
    - limit context-switch points, implement here variable dependency, pointer analysis, etc .....

Limitations:
    - union   ???
    - char *    type
    - long type ???
    - different type (rather than int)
    - variables with same name in different scope?

Changelog:
    2015.11.13  add support for memory model
    2015.08.20  add support for sched_yield statement (this can be a good option here)
    2015.08.18  add discarding context count for printf family
    2015.08.18  add some suggestions from LazyCSeq-SP
    2015.07.09  add 2ls backend
    2015.07.06  add option for passing main argv and argc
    2015.07.02  option for choosing number of bit for guess_pc
    2015.06.21  fix support for atomic region (see new category in visit_Compound)
    2015.06.18  remove __VERIFIER_atomic_begin and __VERIFIER_atomic_end from code
    2015.03.05  support __VERIFIER_atomic_begin and __VERIFIER_atomic_end (require not nesting atomic call)
    2015.02.22  fix translation to FramaC (now the version for FramaC is moved to other labeler module)
    2015.01.13  improvements: no context-switch for __cs_init_scalar function call
    2015.01.12  fix bug: problem of stamping compund statement right after main_thread
    2015.01.10  new feature, now move main_thread before main driver, for parsing with CBMC
    2015.01.10  fix bugs: changing main thread name
    2015.01.08  fix bugs: redundant last semicolon
    2015.01.03  fix bugs: in translating for different backend (only CBMC uses bitvector)
    2015.01.03  clone from labeler_bitvectors
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


class LabelerRC_MEM(core.module.Module):
    __lines = {}                     # lines for each thread
    # name of threads, as they are found in pthread_create(s) - the threads
    # all have different names
    __threadName = ['main']
    # index of the thread = value of threadcount when the pthread_create to
    # that thread was discovered
    __threadIndex = {}
    __threadCount = 0                # pthread create()s found so far

    # statement number where labels are defined [function, label]
    __labelLine = {}
    # statement number where goto to labels appear [function, label]
    __gotoLine = {}
    __maxInCompound = 0              # max label within a compound
    # for labels to have all the same length, adding padding when needed
    __labelLength = 55
    # special char to distinguish between labeled and non-labelled lines
    __startChar = 't'

    # thread statement counter (to build thread labels)
    __stmtCount = -1

    # name of the current thread (also used to build thread labels)
    __currentThread = ''

    __threadbound = 0                # bound on the number of threads

    __firstThreadCreate = False      # set once the first thread creation is met
    # used to limit context-switch points (when global memory is not accessed,
    # no need to insert them)
    __globalMemoryAccessed = False

    # Truc's modification
    __backend = 'cbmc'
    __main_thread = ''
    __visitMain = False
    __atomic_stack = []
    __inAtomicRegion = False

    __isAtomic = False
    ###      #    #####    #
    #  #    # #     #     # #
    #  #   #   #    #    #   #
    #  #   #####    #    #####
    #  #   #   #    #    #   #
    ###    #   #    #    #   #

    primitives = ('int', 'unsigned int', 'size_t', '_Bool', 'bool', 'char', 'unsigned char')
    __refs = []

    __pointer_map = {}
    __num_mallocs = 0
    __considered_var = {}     # Variables that will be considered in the memory model
    __variable_map = {}

    __visitLValue = False
    __visitRValue = False

    __initGlobals = []

    __helpParser = pycparser.c_generator.CGenerator()

    def visit_Compound(self, n):
        s = self._make_indent() + '{\n'
        self.indent_level += 1
        threadIndex = self.Parser.threadIndex[
            self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0

        # Insert the labels at the beginning of each statement,
        # with a few exclusions to reduce context-switch points...
        #
        if n.block_items:
            for stmt in n.block_items:
                if self.__isAtomic:
                    s += self._generate_stmt(stmt) + ';\n'

                # Case 1: last statement in a thread (must correspond to last
                # label)
                elif (type(stmt) == pycparser.c_ast.FuncCall and
                        stmt.name.name == core.common.changeID['pthread_exit']):
                    self.__stmtCount += 1
                    self.__maxInCompound = self.__stmtCount
                    stamp = '%s%s_%s: ' % (
                        self.__startChar, self.__currentThread, str(self.__stmtCount))
                    padding = ' ' * (self.__labelLength - len(stamp))
                    outputcode = self.visit(stmt)
                    # Truc's modification
                    marker = outputcode[outputcode.find('#'):]
                    marker = marker[:marker.find('\n') + 1]
                    code = outputcode[outputcode.find('\n') + 1:]
                    # Truc's modification
                    newStmt = marker + stamp + padding + code
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'

                # Case 1.a: printf family functions will be ignore
                elif (type(stmt) == pycparser.c_ast.FuncCall and
                        stmt.name.name in ('fprintf', 'printf', 'fputs', 'puts')):    # Non exhastive list
                    newStmt = self.visit(stmt)
                    s += newStmt + ';\n'

                # Case 2: labels
                elif (type(stmt) in (pycparser.c_ast.Label,)):
                    # --1-- Simulate a visit to the stmt block to see whether it makes any use of pointers or shared memory.
                    #
                    globalAccess = self.__globalAccess(stmt)
                    newStmt = ''

                    # --2-- Now rebuilds the stmt block again,
                    #       this time using the proper formatting
                    #      (now we know if the statement is accessing global memory,
                    #       so to insert the stamp at the beginning when needed)
                    #
                    if self.__stmtCount == -1:   # first statement in a thread
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = 'IF(%s,%s,%s%s_%s)' % (threadIndex, str(
                            self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount + 1))
                        padding = ' ' * (self.__labelLength - len(stamp))
                        newStmt = stamp + padding + self.visit(stmt.stmt)
                    elif (globalAccess or
                            # Suggestion from Lazy-CSeq-SP
                            # (type(stmt) == pycparser.c_ast.FuncCall and
                            # stmt.name.name ==
                            # core.common.changeID['pthread_create']) or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name == core.common.changeID['pthread_join'])):
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = '%s%s_%s: IF(%s,%s,%s%s_%s)' % (self.__startChar, self.__currentThread, str(self.__stmtCount), threadIndex, str(
                            self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount + 1))
                        padding = ' ' * (self.__labelLength - len(stamp))
                        newStmt = stamp + padding + self.visit(stmt.stmt)
                    else:
                        newStmt = self.visit(stmt.stmt)

                    newStmt = self._make_indent() + stmt.name + ': ' + \
                        'GUARD(%s,%s)\n' % (threadIndex,
                                            self.__stmtCount + 1) + newStmt

                    # Truc's modification, make sure the last semicolon is
                    # placed in the right place
                    if newStmt.endswith('\n'):
                        newStmt = newStmt[:-1]
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'

                # Case 3: all the rest....
                elif (type(stmt) not in (pycparser.c_ast.Compound, pycparser.c_ast.Goto, pycparser.c_ast.Decl)
                      # and not (type(stmt) == pycparser.c_ast.FuncCall and self._parenthesize_unless_simple(stmt.name) == 'assert') Omar 19.Jan.2014 - missing context-switch points before assert()s
                      # and not (type(stmt) == pycparser.c_ast.FuncCall and
                      # self._parenthesize_unless_simple(stmt.name) ==
                      # 'assume')
                      and not (self.__currentThread == 'main' and self.__firstThreadCreate == False) or (self.__currentThread == 'main' and self.__stmtCount == -1)):

                    # --1-- Simulate a visit to the stmt block to see whether it makes any use of pointers or shared memory.
                    #
                    globalAccess = self.__globalAccess(stmt)
                    newStmt = ''

                    self.lines = []   # override core.module marking behaviour,
                    # otherwise  module.visit()  won't insert any marker

                    # --2-- Now rebuilds the stmt block again,
                    #       this time using the proper formatting
                    #      (now we know if the statement is accessing global memory,
                    #       so to insert the stamp at the beginning when needed)
                    #
                    if self.__stmtCount == -1:   # first statement in a thread
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = 'IF(%s,%s,%s%s_%s)' % (threadIndex, str(
                            self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount + 1))
                        padding = ' ' * (self.__labelLength - len(stamp))
                        outputcode = self.visit(stmt)
                        # Truc's modification
                        if type(stmt) == pycparser.c_ast.Compound:
                            newStmt = stamp + padding + outputcode
                        else:
                            marker = outputcode[outputcode.find('#'):]
                            marker = marker[:marker.find('\n') + 1]
                            code = outputcode[outputcode.find('\n') + 1:]
                            newStmt = marker + stamp + padding + code

                    elif (self.__inAtomicRegion and    # New category
                            not ((type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__VERIFIER_atomic')) or
                                 (type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic'])))):
                        newStmt = self.visit(stmt)

                    elif (type(stmt) == pycparser.c_ast.FuncCall and     # Special for yield statement
                            stmt.name.name == "__cs_sched_yield"):
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = '%s%s_%s: IF(%s,%s,%s%s_%s)' % (self.__startChar, self.__currentThread, str(self.__stmtCount), threadIndex, str(
                            self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount + 1))
                        padding = ' ' * (self.__labelLength - len(stamp))
                        code = "__cs_sched_yield(%s)" % self.__stmtCount
                        newStmt = stamp + padding + code

                    elif (globalAccess or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name == core.common.changeID['pthread_create']) or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name == core.common.changeID['pthread_join']) or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name.startswith('__VERIFIER_atomic')) or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name.startswith('__VERIFIER_assume')) or
                            (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']))):
                        if (type(stmt) == pycparser.c_ast.FuncCall and
                                stmt.name.name == '__cs_init_scalar'):
                            newStmt = self.visit(stmt)
                        else:
                            self.__stmtCount += 1
                            self.__maxInCompound = self.__stmtCount
                            stamp = '%s%s_%s: IF(%s,%s,%s%s_%s)' % (self.__startChar, self.__currentThread, str(self.__stmtCount), threadIndex, str(
                                self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount + 1))
                            padding = ' ' * (self.__labelLength - len(stamp))
                            outputcode = self.visit(stmt)
                            # Truc's modification
                            marker = outputcode[outputcode.find('#'):]
                            marker = marker[:marker.find('\n') + 1]
                            code = outputcode[outputcode.find('\n') + 1:]
                            newStmt = marker + stamp + padding + code
                    else:
                        newStmt = self.visit(stmt)

                    # Truc's modification
                    if newStmt.endswith('\n'):
                        newStmt = newStmt[:-1]
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'
                else:
                    newStmt = self.visit(stmt)
                    # Truc's modification
                    if newStmt.endswith('\n'):
                        newStmt = newStmt[:-1]
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'

        self.indent_level -= 1
        s += self._make_indent() + '}\n'

        return s

    def visit_FuncDef(self, n):
        if (n.decl.name.startswith('__VERIFIER_atomic_') or
                n.decl.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']) or
                n.decl.name == '__VERIFIER_assert' or
                n.decl.name in self.Parser.funcReferenced):   # <--- functions called through pointers are not inlined yet
            oldatomic = self.__isAtomic
            self.__isAtomic = True
            self.__currentThread = n.decl.name
            decl = self.visit(n.decl)
            body = self.visit(n.body)
            self.__isAtomic = oldatomic
            self.__currentThread = ''
            return decl + '\n' + body + '\n'

        self.__currentThread = n.decl.name

        if self.__currentThread != 'main' and self.__currentThread not in self.Parser.threadName:
            print 'This function %s is not a thread' % self.__currentThread
            sys.exit(2)

        # Reset for every thread
        self.__atomic_stack = []
        self.__inAtomicRegion = False
        if self.__currentThread == 'main' and self.__visitMain:
            print 'More than one main function in the input file'
            sys.exit(2)

        self.__firstThreadCreate = False

        decl = self.__helpParser.visit(n.decl)
        self.indent_level = 0
        body = self.visit(n.body)

        f = ''

        self.__lines[self.__currentThread] = self.__stmtCount
        # print "THREAD %s, LINES %s \n\n" % (self.__currentThread,
        # self.__lines)

        #
        if n.param_decls:
            knrdecls = ';\n'.join(self.visit(p) for p in n.param_decls)
            self.__stmtCount = -1
            f = decl + '\n' + knrdecls + ';\n'
        else:
            self.__stmtCount = -1
            f = decl + '\n'

        # Remove arguments (if any) for main() and transform them into local variables in main_thread.
        #
        if self.__currentThread == 'main':
            # f = f.replace(' main(', ' main_thread(')
            # f = re.sub(r' main([-a-zA-Z0-9_, *]+)', r' main_thread(void)', f)
            # f = re.sub(r' main(.*)', r' main_thread(void)', f)
            # Truc's modification
            # Provide declaration for command line arguments
            f = '%s main_thread(void)\n' % self.Parser.funcBlockOut[
                self.__currentThread]

            main_args = self.Parser.funcBlockIn['main']
            args = ''
            if main_args.find('void') != -1 or main_args == '':
                main_args = ''
            else:
                # Change **argv --> *argv[]
                main_args = re.sub(r'\*\*(.*)', r'*\1[]', main_args)
                # Change argv[][] --> *argv[]
                main_args = re.sub(r'(.*)\[\]\[\]', r'*\1[]', main_args)
                # split argument
                main_args = main_args.split(',')
                if len(main_args) != 2:
                    print 'main function may have been defined wrongly'
                    sys.exit(2)
                args = 'static ' + main_args[0] + ' = %s; ' % self.__argc
                argv = self.__argv.split(' ')
                argv = '{' + ','.join(['\"%s\"' % v for v in argv]) + '}'
                args += 'static ' + main_args[1] + ' = %s;' % argv
            body = '{' + args + body[body.find('{') + 1:]

        f += body + '\n\n\n'

        # Truc's modification
        if self.__currentThread == 'main':
            self.__main_thread = f
            self.__visitMain = True
            return ''  # save it for later
        else:
            return f

    def visit_If(self, n):
        ifStart = self.__maxInCompound   # label where the if stmt begins

        s = 'if ('

        if n.cond:
            condition = self.visit(n.cond)
            s += condition

        s += ')\n'
        s += self._generate_stmt(n.iftrue, add_indent=True)

        # label for the last stmt in the if block:  if () { block; }
        ifEnd = self.__maxInCompound
        nextLabelID = ifEnd + 1

        if n.iffalse:
            elseBlock = self._generate_stmt(n.iffalse, add_indent=True)

            # label for the last stmt in the if_false block if () {...} else {
            # block; }
            elseEnd = self.__maxInCompound

            if ifStart < ifEnd:
                threadIndex = self.Parser.threadIndex[
                    self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
                elseHeader = 'GUARD(%s,%s)' % (threadIndex, str(ifEnd + 1))
            else:
                elseHeader = ''

            nextLabelID = elseEnd + 1
            s += self._make_indent() + 'else\n'

            elseBlock = elseBlock.replace('{', '{ ' + elseHeader, 1)
            s += elseBlock

        header = ''

        if ifStart + 1 < nextLabelID:
            threadIndex = self.Parser.threadIndex[
                self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
            footer = 'GUARD(%s,%s)' % (threadIndex, nextLabelID)
        else:
            footer = ''

        '''
        if n.iffalse:
            header = 'ASS_ELSE(%s, %s, %s)' % (condition, ifEnd+1, elseEnd+1) + '\n' + self._make_indent()
        else:
            if ifEnd > ifStart:
                header = 'ASS_THEN(%s, %s)' % (condition, ifEnd+1) + '\n' + self._make_indent()
            else: header = ''
        '''

        return header + s + self._make_indent() + footer

    def visit_Return(self, n):
        if (not (self.__currentThread.startswith('__VERIFIER_atomic_') or
            self.__currentThread.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']) or
            self.__currentThread == '__VERIFIER_assert' or
            self.__currentThread in self.Parser.funcReferenced)):
            print "error: labeler.py: return statement in thread '%s'.\n" % self.__currentThread
            sys.exit(2)

        s = 'return'
        if n.expr:
            s += ' ' + self.visit(n.expr)
        return s + ';'

    def visit_Label(self, n):
        self.__labelLine[self.__currentThread, n.name] = self.__stmtCount
        return n.name + ':\n' + self._generate_stmt(n.stmt)

    def visit_Goto(self, n):
        self.__gotoLine[self.__currentThread, n.name] = self.__stmtCount
        extra = '<%s,%s>\n' % (self.__currentThread,
                               n.name) + self._make_indent()
        extra = ''
        return extra + 'goto ' + n.name + ';'

    def visit_ID(self, n):
        # If this ID corresponds either to a global variable,
        # or to a pointer...
        #
        if ((self.__isGlobal(self.__currentThread, n.name) or
             self.__isPointer(self.__currentThread, n.name)) and
                not n.name.startswith('__cs_thread_local_')):
            # Truc's modication
            if not self.__inAtomicRegion:
                self.__globalMemoryAccessed = True
        return n.name

    ##     ## ######## ##     ##  #######  ########  ##    ##    ##     ##  #######  ########  ######## ##
    ###   ### ##       ###   ### ##     ## ##     ##  ##  ##     ###   ### ##     ## ##     ## ##       ##
    #### #### ##       #### #### ##     ## ##     ##   ####      #### #### ##     ## ##     ## ##       ##
    ## ### ## ######   ## ### ## ##     ## ########     ##       ## ### ## ##     ## ##     ## ######   ##
    ##     ## ##       ##     ## ##     ## ##   ##      ##       ##     ## ##     ## ##     ## ##       ##
    ##     ## ##       ##     ## ##     ## ##    ##     ##       ##     ## ##     ## ##     ## ##       ##
    ##     ## ######## ##     ##  #######  ##     ##    ##       ##     ##  #######  ########  ######## ########

    def visit_FuncCall(self, n):
        fref = self._parenthesize_unless_simple(n.name)
        # Truc's modification
        if fref == '__CS_atomic_begin':
            self.__atomic_stack.append(self.__currentThread)
            self.__inAtomicRegion = True
            return '//__CS_atomic_begin()'
        elif fref == '__CS_atomic_end':
            if len(self.__atomic_stack) == 0:  # There must be something wrong in the thread
                self.__inAtomicRegion = False
            elif len(self.__atomic_stack) == 1:   # Correct value
                self.__atomic_stack.pop()
                self.__inAtomicRegion = False
            else:   # Nested atomic region, must be something wrong
                self.__atomic_stack.pop()
                self.__inAtomicRegion = False
            return '//__CS_atomic_end()'

        # When a thread is created, extract its function name
        # based on the 3rd parameter in the pthread_create() call:
        #
        # pthread_create(&id, NULL, f, &arg);
        #                          ^^^
        elif fref == core.common.changeID['pthread_create']:
            args = self.__helpParser.visit(n.args)
            fName = self.__helpParser.visit(n.args.exprs[2])
            fName = fName.replace('&', '').strip()
            if '\n' in fName:   # Some thing goes wrong with the name
                fName = fName[fName.rfind('\n') + 1:]

            if fName not in self.__threadName:
                self.__threadName.append(fName)
                self.__threadCount = self.__threadCount + 1

                args = args + ', %s' % (self.__threadCount)
                self.__threadIndex[fName] = self.__threadCount
            else:
                # when visiting from the 2nd time on (if it happens),
                # reuse the old thread indexS!
                args = args + ', %s' % (self.__threadIndex[fName])

            self.__firstThreadCreate = True
            return fref + '(' + args + ')'

        elif fref == core.common.changeID['pthread_exit']:
            if self.Parser.funcBlockOut[self.__currentThread] != 'void':
                return 'STOP_NONVOID(%s)' % (self.__stmtCount)
            else:
                return 'STOP_VOID(%s)' % (self.__stmtCount)

        elif fref.startswith('pthread_') or (fref.startswith('__cs_') and 'malloc' not in fref):
            args = self.__helpParser.visit(n.args)
            return fref + '(' + args + ')'

        ####### ##   ## ######   ##### ###### #### #####  ######   #####
        ##      ##   ## ##   ## ##   ##  ##    ## ##   ## ##   ## ##   ##
        ##      ##   ## ##   ## ##       ##    ## ##   ## ##   ## ##
        #####   ##   ## ##   ## ##       ##    ## ##   ## ##   ##  #####
        ##      ##   ## ##   ## ##       ##    ## ##   ## ##   ##      ##
        ##      ##   ## ##   ## ##   ##  ##    ## ##   ## ##   ## ##   ##
        ##       #####  ##   ##  #####   ##   #### #####  ##   ##  #####

        elif fref == mem_config.functions['write_malloc']:
            # Write to a pointer a value of malloc
            if len(n.args.exprs) != 2:
                # There must be two argument to this function
                print "Only two arguments in function %s" % fref
                sys.exit(1)
            left = self.visit(n.args.exprs[0])
            right = self.visit(n.args.exprs[1])
            # Assuming that the information about malloc is already known after this
            mallocsize = right[right.find('(')+1:right.rfind(')')]
            try:
                mallocsize = int(eval_expr(mallocsize))
            except TypeError:
                print "malloc with dynamic size is not supported"
                sys.exit(1)
            # replace right with the malloc will be replaced by number
            right = 'MALLOC_POINTER_%s' % self.__num_mallocs
            self.__pointer_map[left] = (int(mallocsize), self.__num_mallocs)
            self.__num_mallocs += 1
            return mem_config.functions['write_ptr'] + '(' + left + ', ' + right + ')'

        elif fref == mem_config.functions['address']:
            '''
            In order to get a position, the variable need to be dectected in the context
            1. If this is a single scalar --> return by the map of scalar
            2. If this is a compound scalar
              a. Check if a part of it belong to the malloc map
              b. If not, using the check on the map of scalar
            '''
            args = self.visit(n.args)
            return args

        # elif fref == mem_config.functions['write']:
        #     # Write to memory
        # elif fref == mem_config.functions['read']:
        #     # Return the location (index of the array)
        #     pass
        # elif fref == mem_config.functions['read_ptr']:
        #     pass
        # elif fref == mem_config.functions['write_ptr']:
        #     pass
        elif fref == '__cs_safe_malloc':
            args = self.visit(n.args)
            return mem_config.functions['malloc'] + '(' + args + ')'
        else:
            args = self.visit(n.args)
            return fref + '(' + args + ')'

    # Disable declaration
    def visit_Typedef(self, n):
        return ''
        # s = ''
        # if n.storage: s += ' '.join(n.storage) + ' '
        # s += self._generate_type(n.type)
        # return s

    def _generate_struct_union(self, n, name):
        """ Generates code for structs and unions.
            name should be either 'struct' or 'union'.
        """
        return ''
        # s = name + ' ' + (n.name or '')
        # if n.decls:
        #     s += '\n'
        #     s += self._make_indent()
        #     self.indent_level += 2
        #     s += '{\n'
        #     for decl in n.decls:
        #         s += self._generate_stmt(decl)
        #     self.indent_level -= 2
        #     s += self._make_indent() + '}'
        # return s

    def visit_Decl(self, n, no_type=False):
        # no_type is used when a Decl is part of a DeclList, where the type is
        # explicitly only for the first declaration in a list.
        #

        s = ''
        if n.name and self.__isVariable(self.__currentThread, n.name):
            var_type = self._get_typeof_variable(self.__currentThread, n.name)

            if '__cs_' in var_type[0]:  # Suppose to be thread-related variable
                s = n.name if no_type else self._generate_decl(n)
                if n.bitsize: s += ' : ' + self.visit(n.bitsize)
                if n.init:
                    s += ' = ' + self._visit_expr(n.init)
                return s

            # TODO: preserve static if possible
            if '*' in var_type[0]:
                s += 'unsigned int ' + n.name
            elif 'struct ' in var_type[0]:
                s += 'unsigned int ' + n.name
            elif var_type[1] > 0:
                s += 'unsigned int ' + n.name
            else:
                # All other type will become an int
                s += 'int ' + n.name

            if 'static ' in var_type[0]:
                s = 'static ' + s
            # Process init statement
            init = ''
            initType = 0
            if n.init:
                if isinstance(n.init, pycparser.c_ast.InitList):
                    init = '{' + self.visit(n.init) + '}'
                    initType = 1
                elif isinstance(n.init, pycparser.c_ast.ExprList):
                    init = '(' + self.visit(n.init) + ')'
                    initType = 0
                else:
                    init = self.visit(n.init)
                    initType = 0

                if initType == 1:
                    print "Constant experssion initialization is not supported yet"
                    sys.exit(1)

                if n.name in self.Parser.varNames['']:
                    self.__initGlobals.append((n.name, init))
                else:
                    s += '; %s(%s, %s)' % (mem_config.functions['write'], n.name, init)
            return s
        else:
            s = n.name if no_type else self._generate_decl(n)
            if n.bitsize: s += ' : ' + self.visit(n.bitsize)
            if n.init:
                s += ' = ' + self._visit_expr(n.init)
            return s

    def visit_Cast(self, n):
        # Do not want to have and cast operation here
        # s = '(' + self._generate_type(n.to_type) + ')'
        # return s + ' ' + self._parenthesize_unless_simple(n.expr)
        return self._parenthesize_unless_simple(n.expr)

    def visit_UnaryOp(self, n):
        operand = self._parenthesize_unless_simple(n.expr)
        if n.op == 'p++':
            return '%s++' % operand
        elif n.op == 'p--':
            return '%s--' % operand
        elif n.op == 'sizeof':
            # Exploration of type
            #   Any primitives type is equal to the size of an     int
            #   Struct size is calculated by the total size of its fields
            #   a pointer is equal to the size of an   int
            #
            typ = self.visit(n.expr)
            if '*' in typ:
                # A pointer is the size of an    int   (no matter how many stars)
                return '(1)'
            elif typ.strip() in self.primitives:
                return '(1)'
            elif typ in self.Parser.typedefs:
                typename = self.Parser.typedefExpansion[typ]
                size = self._get_sizeof_type((typename, 0, []))
                return '(%s)' % str(size)
            elif typ.startswith('struct '):
                size = self._get_sizeof_struct(typ)
                return '(%s)' % str(size)
            else:
                # Don't really know what will be here
                return '(1)'
        else:
            return '%s%s' % (n.op, operand)

    def visit_ArrayRef(self, n):
        if not isinstance(n.name, (pycparser.c_ast.ArrayRef, pycparser.c_ast.StructRef)):
            # Final calculation
            ret = ''
            origin_name = n.name.name
            varType, varArity, varSize = self._get_typeof_variable(self.__currentThread, n.name.name)
            subscript = self.visit(n.subscript)
            if varArity == 0:
                # Must be a pointer type
                if '*' not in varType:
                    print "expecting pointer %s" % origin_name
                    sys.exit(1)
                varType = varType.replace('*', '', 1)
                origin_name = mem_config.data['value'] + '[%s]' % origin_name
            elif varArity == 1:
                varArity -= 1
                varSize.pop()
            else:
                print "In visit_ArrayRef, dimensional problem in %s" % origin_name
                sys.exit(1)
            ret = origin_name + ' + ' + subscript + '*' + str(self._get_sizeof_type((varType, varArity, varSize)))

            # Combining all the rest
            self.__refs.reverse()   # Reverse to match top-down parser
            for item in self.__refs:
                if item[0] == 'aRef':   # An array reference
                    if item[1]:    # Array is a pointer
                        ret = mem_config.data['value'] + '[%s]' % ret + ' + ' + '(' + item[3] + ')' + '*' + str(item[2])
                    else:
                        ret = ret + ' + ' + '(' + item[3] + ')' + '*' + str(item[2])
                else:
                    if item[1] == '.':    # Just a static struct
                        ret = ret + ' + ' + str(item[2])
                    else:    # pointer
                        ret = mem_config.data['value'] + '[%s]' % ret + ' + ' + str(item[2])
            self.__refs = []
            return ret
        else:
            arrType, arrArity, arrSize = self._get_typeof_node(n.name)
            if arrArity == 0:
                isPointer = True
                arrType = arrType.replace('*', '', 1)
            else:
                isPointer = False
                arrArity -= 1
                arrSize.pop()
            subscript = self.visit(n.subscript)
            self.__refs.append(('aRef', isPointer, self._get_sizeof_type((arrType, arrArity, arrSize)), subscript))
            return self.visit(n.name)

    def visit_StructRef(self, n):
        if not isinstance(n.name, (pycparser.c_ast.ArrayRef, pycparser.c_ast.StructRef)):
            # Final calculation
            ret = ''
            origin_name = n.name.name
            struct_type = self._get_typeof_variable(self.__currentThread, origin_name)
            field_name = self.visit(n.field)
            offset = self._get_field_offset(struct_type[0], field_name)
            if n.type == '.':
                ret = origin_name
            else:
                ret = mem_config.data['value'] + '[%s]' % origin_name
            ret += ' + ' + str(offset)
            self.__refs.reverse()
            for item in self.__refs:
                if item[0] == 'aRef':   # An array reference
                    if item[1]:    # Array is a pointer
                        ret = mem_config.data['value'] + '[%s]' % ret + ' + ' + '(' + item[3] + ')' + '*' + str(item[2])
                    else:
                        ret = ret + ' + ' + '(' + item[3] + ')' + '*' + str(item[2])
                else:
                    if item[1] == '.':    # Just a static struct
                        ret = ret + ' + ' + str(item[2])
                    else:    # pointer
                        ret = mem_config.data['value'] + '[%s]' % ret + ' + ' + str(item[2])
            self.__refs = []
            return ret
        else:
            struct_type = self._get_typeof_node(n.name)
            field_name = self.visit(n.field)
            offset = self._get_field_offset(struct_type[0], field_name)
            self.__refs.append(('sRef', n.type, offset))
            return self.visit(n.name)

    ##   ## ####### ##      ######  ####### ######   #####
    ##   ## ##      ##      ##   ## ##      ##   ## ##   ##
    ##   ## ##      ##      ##   ## ##      ##   ## ##
    ####### #####   ##      ######  #####   ######   #####
    ##   ## ##      ##      ##      ##      ##   ##      ##
    ##   ## ##      ##      ##      ##      ##   ## ##   ##
    ##   ## ####### ####### ##      ####### ##   ##  #####

    def _get_typeof_variable(self, currentThread, variablename):
        if currentThread not in self.Parser.varNames:
            print "In function _get_typeof_variable, cannot find function %s" % currentThread
            sys.exit(2)

        if variablename in self.Parser.varNames[currentThread]:
            # Look for variable in thread (function) first
            varArity = self.Parser.varArity[currentThread, variablename]
            varType = self.Parser.varType[currentThread, variablename]
            varSize = self.Parser.varSize[currentThread, variablename]
            # Strip out redundant
            # varType = varType.replace("static ", '')
            varType = varType.replace("const ", '')
            varType = varType.replace("volatile ", '')
            return (varType, varArity, copy.deepcopy(varSize))

        elif variablename in self.Parser.varNames['']:
            # Look up this variable in the global scope
            varArity = self.Parser.varArity['', variablename]
            varType = self.Parser.varType['', variablename]
            varSize = self.Parser.varSize['', variablename]

            # Strip out redundant
            # varType = varType.replace("static ", '')
            varType = varType.replace("const ", '')
            varType = varType.replace("volatile ", '')
            return (varType, varArity, copy.deepcopy(varSize))
        else:
            print "This variable %s does not belong to any scope" % variablename
            sys.exit(1)

    def _get_typeof_node(self, n):
        if type(n) == pycparser.c_ast.StructRef:
            if type(n.name) == pycparser.c_ast.ID:
                struct_type = self._get_typeof_variable(self.__currentThread, (n.name.name))
            else:
                struct_type = self._get_typeof_node(n.name)
            field_name = n.field.name
            return self._get_typeof_field(struct_type[0], field_name)
        elif type(n) == pycparser.c_ast.ArrayRef:
            if type(n.name) == pycparser.c_ast.ID:
                array_type = self._get_typeof_variable(self.__currentThread, n.name.name)
            else:
                array_type = self._get_typeof_node(n.name)
            varType, varArity, varSize = array_type
            if varArity == 0:
                varType = varType.replace('*', '', 1)
            else:
                varArity -= 1
                varSize.pop()
            return (varType, varArity, copy.deepcopy(varSize))
        elif type(n) == pycparser.c_ast.FuncCall:
            name = self._parenthesize_unless_simple(n.name)
            if name not in self.Parser.funcBlockOut:
                print "Function %s not found" % name
                sys.exit(1)
            return (self.Parser.funcBlockOut[name], 0, [])
        elif type(n) == pycparser.c_ast.ID:
            return self._get_typeof_variable(self.__currentThread, n.name)
        else:
            print "This type of operation is not supported"
            sys.exit(1)

    def _get_typeof_field(self, struct_type, field_name):
        struct_type = struct_type.replace('struct ', '', 1)
        struct_type = struct_type.replace('*', '').strip()
        if struct_type in self.Parser.typedefs:
            struct_type = self.Parser.typedefExpansion[struct_type].replace('struct ', '')
        struct_type = struct_type.strip()
        if struct_type not in self.Parser.varNames:
            print "struct %s is not defined" % struct_type
            sys.exit(1)

        if field_name not in self.Parser.varNames[struct_type]:
            print "field %s is not defined in struct %s" % (field_name, struct_type)
            sys.exit(1)

        varArity = self.Parser.varArity[struct_type, field_name]
        varType = self.Parser.varType[struct_type, field_name]
        varSize = self.Parser.varSize[struct_type, field_name]
        return (varType, varArity, varSize)

    def _get_field_offset(self, struct_type, field_name):
        struct_type = struct_type.replace('struct ', '', 1)
        struct_type = struct_type.replace('*', '').strip()
        if struct_type in self.Parser.typedefs:
            struct_type = self.Parser.typedefExpansion[struct_type].replace('struct ', '')
        struct_type = struct_type.strip()
        if struct_type not in self.Parser.structName:
            print "In function _get_field_offset, cannot find struct %s" % struct_type
            sys.exit(2)
        offset = 0
        for v in self.Parser.varNames[struct_type]:
            if v == field_name:
                break
            else:
                if '*' in self.Parser.varType[struct_type, v]:  # a pointer anyway
                    offset += 1
                # equivalent to a pointer
                elif self.Parser.varArity[struct_type, v] > 0:
                    arity = 1
                    for i in range(0, self.Parser.varArity[struct_type, v]):
                        arity *= self.Parser.varSize[struct_type, v][i]
                    offset += arity
                elif self.Parser.varType[struct_type, v].startswith('struct'):
                    offset += self._get_sizeof_struct(
                        self.Parser.varType[struct_type, v])
                else:
                    offset += 1
        return offset

    def _get_sizeof_struct(self, struct_type):
        struct_type = struct_type.replace('struct ', '', 1)
        if struct_type in self.Parser.typedefs:
            struct_type = self.Parser.typedefExpansion[struct_type].replace('struct ', '')
        struct_type = struct_type.strip()
        if struct_type not in self.Parser.structName:
            print "In function _get_sizeof_struct, cannot find struct %s" % struct_type
            sys.exit(2)
        total = 0
        for v in self.Parser.varNames[struct_type]:
            if '*' in self.Parser.varType[struct_type, v]:  # a pointer anyway
                total += 1
            elif self.Parser.varArity[struct_type, v] > 0:
                arity = 1
                for i in range(0, self.Parser.varArity[struct_type, v]):
                    arity *= self.Parser.varSize[struct_type, v][i]
                total += arity
            elif self.Parser.varType[struct_type, v].startswith('struct '):
                total += self._get_sizeof_struct(self.Parser.varType[struct_type, v])
            else:
                total += 1
        return total

    def _get_sizeof_type(self, typename):
        typ, arity, size = typename
        typ = typ.replace('static ', '')
        typ = typ.replace('volatile ', '')
        typ = typ.strip()
        if '*' in typ:           # pointer
            return 1
        elif typ.startswith('struct '):
            return self._get_sizeof_struct(typ)
        elif typ in self.primitives:
            if arity == 0:
                return 1
            else:
                mul = 1
                for i in range(0, arity):
                    mul *= size[i]
                return mul
        else:
            if typ in ('__cs_t', '__cs_mutex_t', '__cs_cond_t'):
                return 1
            print 'Cannot get size of type %s' % str(typename)
            sys.exit(1)


    def _is_tracked(self, currentFunction, variable):
        '''
            A variable is TRACKED (on the global memory) if:
            - it is a scalar and:
                + v is used in & form and there is some write to the pointer pointing v
            - it is a pointer and:
                + v is already pointing to some location which is new (not previously known)
            - it is a static array and
                + exists an index which is not statically determined
            - it is a static struct and
                + at least one of its fields is tracked
            - it is a pointer to struct
        '''

    def _build_considered_variables(self):
        count = 0
        for f in self.Parser.funcName:
            if f == '':
                # GLOBALS
                for v in self.Parser.varNames[f]:
                    varType = self.Parser.varType[f, v]
                    varArity = self.Parser.varArity[f, v]
                    varSize = self.Parser.varSize[f, v]
                    size = self._get_sizeof_type((varType, varArity, varSize))
                    if '__cs_' not in varType:
                        # This variable is taken into consideration
                        self.__considered_var[f, v] = count
                        count += size
            else:
                # LOCALS
                for v in self.Parser.varNames[f]:
                    varType = self.Parser.varType[f, v]
                    varKind = self.Parser.varKind[f, v]
                    varArity = self.Parser.varArity[f, v]
                    varReferenced = self.Parser.varReferenced[f, v]
                    varSize = self.Parser.varSize[f, v]
                    size = self._get_sizeof_type((varType, varArity, varSize))

                    # if '__cs_' not in varType:
                        # if varKind == 'p':   # always taking care of paramter
                        # count += size

    def __create_mem_header(self):
        ret = ''
        # Declarations
        count = 0
        for f in self.Parser.funcName:
            if f == '':
                # GLOBALS
                for v in self.Parser.varNames[f]:
                    varType = self.Parser.varType[f, v]
                    varArity = self.Parser.varArity[f, v]
                    varSize = self.Parser.varSize[f, v]
                    size = self._get_sizeof_type((varType, varArity, varSize))
                    ret += '#define ' + v + ' ' + str(count) + '\n'
                    self.__variable_map[ret] = (self.Parser.varKind[f, v], count)
                    count += size
            else:
                # LOCALS
                # TODO: need special treatment for parameters since it will be wrong in the declaration
                for v in self.Parser.varNames[f]:
                    varType = self.Parser.varType[f, v]
                    varArity = self.Parser.varArity[f, v]
                    varSize = self.Parser.varSize[f, v]
                    size = self._get_sizeof_type((varType, varArity, varSize))
                    ret += '#define ' + v + ' ' + str(count) + '\n'
                    self.__variable_map[ret] = (self.Parser.varKind[f, v], count)
                    count += size

        # MALLOCS
        # Size is from that point of all static variable
        for s in self.__pointer_map:
            ret += '#define MALLOC_POINTER_%s' % self.__pointer_map[s][1] + ' ' + str(count) + '\n'
            count += self.__pointer_map[s][0]

        # INIT
        # Place for the initial of global variable
        ret += 'void %s(void){\n' % mem_config.functions['init']
        for init in self.__initGlobals:
            ret += '    %s(%s, %s);\n' % (mem_config.functions['write'], init[0], init[1])
        ret += '}\n'

        ret += '''
int %s[%s];
unsigned int %s = 1;
        ''' % (mem_config.data['value'], mem_config.data['size'], mem_config.data['malloc'])

        # Functions definition
        # MALLOC
        ret += '''
unsigned int %s(unsigned int size)
{
    unsigned int tmp = %s;
    %s += size + 1;
    return tmp + 1;
}
        ''' % (mem_config.functions['malloc'], mem_config.data['malloc'], mem_config.data['malloc'])

        # FREE
        ret += '''
void %s(unsigned int address)
{
}
        ''' % mem_config.functions['free']

        # READ
        ret += '''
int %s(unsigned int position)
{
    return %s[position];
}
        ''' % (mem_config.functions['read'], mem_config.data['value'])

        # READ_PTR
        ret += '''
int %s(unsigned int position)
{
    return %s[position];
}
        ''' % (mem_config.functions['read_ptr'], mem_config.data['value'])

        # WRITE
        ret += '''
void %s(unsigned int position, int value)
{
    %s[position] = value;
}
        ''' % (mem_config.functions['write'], mem_config.data['value'])

        # WRITE_PTR
        ret += '''
void %s(unsigned int position, int value)
{
    %s[position] = value;
}
        ''' % (mem_config.functions['write_ptr'], mem_config.data['value'])

        ret = '#define %s %s\n' % (mem_config.data['size'], str(count+1)) + ret

        return ret

    ##########################################################################

    def __createMain(self, ROUNDS):
        # the new main() is created according to the following sort of scheduling:
        #
        # main_thread    t1 t2    t1 t2   t1 t2    t1 t2     t1 t2    t1 t2      t1 t2    t1 t2    main_thread
        #

        # Calculate thread lines max bits
        kbits = {}
        for t in self.__lines:
            k = int(math.floor(math.log(self.__lines[t], 2))) + 1
            kbits[t] = k

        round = 0

        main = ''
        main += "int main(void) {\n"

        main += "          // round %s\n" % round
        main += "          thread_index = 0;\n"
        if self.__backend == 'framac':
            main += "          pc_cs = Frama_C_unsigned_int_interval(1, %s);\n" % self.__lines[
                'main']
            main += "          %s\n" % self._generateConstraintsFramaC(
                'pc_cs', self.__lines['main'])
            # main += "          assume(pc_cs <= %s);\n" % (self.__lines['main'])
        elif self.__backend == 'cbmc':
            main += "          pc_cs = pc[0] + guess_pc();\n"
            main += "          assume((pc_cs > 0) && (pc_cs <= %s));\n" % (
                self.__lines['main'])
            # main += "          assume((pc_cs > 0) & (pc_cs <= %s));\n" % (self.__lines['main'])
        else:
            main += "          pc_cs = pc[0] + guess_pc();\n"
            main += "          assume((pc_cs > 0) && (pc_cs <= %s));\n" % (
                self.__lines['main'])
            # main += "          assume((pc_cs > 0) & (pc_cs <= %s));\n" % (self.__lines['main'])
        main += "          main_thread();\n"
        main += "          pc[0] = pc_cs;\n"
        if self.__backend == 'framac':
            # main += "          assume(pc[0] <= %s);\n" % self.__lines['main']
            # main += "          %s\n" % self._generateConstraintsFramaC('pc[0]', self.__lines['main'])
            pass
        main += "\n"

        i = 1
        for t in self.__threadName:
            if t == 'main':
                continue
            if i <= self.__threadbound:
                main += "          thread_index = %s;\n" % i
                main += "          if (active_thread[%s] == 1) { /** %s **/\n" % (
                    i, t)
                if self.__backend == 'framac':
                    main += "             pc_cs = Frama_C_unsigned_int_interval(0, %s);\n" % (
                        self.__lines[t])
                    main += "             %s\n" % self._generateConstraintsFramaC(
                        'pc_cs', self.__lines[t])
                    # main += "             assume(pc_cs <= %s);\n" % (self.__lines[t])
                elif self.__backend == 'cbmc':
                    main += "             pc_cs = pc[%s] + guess_pc();\n" % i
                    main += "             assume((pc_cs >= 0) && (pc_cs <= %s));\n" % (
                        self.__lines[t])
                    # main += "             assume((pc_cs >= 0) & (pc_cs <= %s));\n" % (self.__lines[t])
                else:
                    main += "             pc_cs = pc[%s] + guess_pc();\n" % i
                    main += "             assume((pc_cs >= 0) && (pc_cs <= %s));\n" % (
                        self.__lines[t])
                    # main += "             assume((pc_cs >= 0) & (pc_cs <= %s));\n" % (self.__lines[t])
                main += "             %s(%s);\n" % (t, 'threadargs[%s]' % i)
                main += "             pc[%s] = pc_cs;\n" % i
                if self.__backend == 'framac':
                    # main += "             assume(pc[%s] <= %s);\n" % (i, self.__lines[t])
                    # main += "             %s\n" % self._generateConstraintsFramaC('pc[%s]' % i, self.__lines[t])
                    pass
                main += "          }\n\n"
                i += 1

        for round in range(1, ROUNDS):
            # ####main +="          // round %s\n" % round
            main += "          // round %s\n" % round
            main += "          thread_index = 0;\n"
            main += "          if (active_thread[0] == 1) { /** %s **/\n" % 'main'
            if self.__backend == 'framac':
                main += "              pc_cs = Frama_C_unsigned_int_interval(pc[0], %s);\n" % self.__lines[
                    'main']
                main += "              %s\n" % self._generateConstraintsFramaC(
                    'pc_cs', self.__lines['main'])
                # main += "              assume(pc_cs <= %s);\n" % (self.__lines['main'])
            elif self.__backend == 'cbmc':
                main += "              pc_cs = pc[0] + guess_pc();\n"
                main += "              assume((pc_cs >= pc[0]) && (pc_cs <= %s));\n" % (
                    self.__lines['main'])
                # main += "              assume((pc_cs >= pc[0]) & (pc_cs <= %s));\n" % (self.__lines['main'])
            else:
                main += "              pc_cs = pc[0] + guess_pc();\n"
                main += "              assume((pc_cs >= pc[0]) && (pc_cs <= %s));\n" % (
                    self.__lines['main'])
                # main += "              assume((pc_cs >= pc[0]) & (pc_cs <= %s));\n" % (self.__lines['main'])
            main += "              main_thread();\n"
            main += "              pc[0] = pc_cs;\n"
            if self.__backend == 'framac':
                # main += "              assume(pc[0] <= %s);\n" % (self.__lines['main'])
                # main += "              %s\n" % self._generateConstraintsFramaC('pc[0]', self.__lines['main'])
                pass
            main += "          }\n\n"

            i = 1
            for t in self.__threadName:
                if t == 'main':
                    continue
                if i <= self.__threadbound:
                    main += "          thread_index = %s;\n" % i
                    main += "          if (active_thread[%i] == 1) { /** %s **/\n" % (
                        i, t)
                    if self.__backend == 'framac':
                        main += "             pc_cs = Frama_C_unsigned_int_interval(pc[%s], %s);\n" % (
                            i, self.__lines[t])
                        main += "             %s\n" % self._generateConstraintsFramaC(
                            'pc_cs', self.__lines[t])
                        # main += "             assume(pc_cs <= %s);\n" % (self.__lines[t])
                    elif self.__backend == 'cbmc':
                        main += "             pc_cs = pc[%s] + guess_pc();\n" % i
                        main += "             assume((pc_cs >= pc[%s]) && (pc_cs <= %s));\n" % (
                            i, self.__lines[t])
                        # main += "             assume((pc_cs >= pc[%s]) & (pc_cs <= %s));\n" % (i, self.__lines[t])
                    else:
                        main += "             pc_cs = pc[%s] + guess_pc();\n" % i
                        main += "             assume((pc_cs >= pc[%s]) && (pc_cs <= %s));\n" % (
                            i, self.__lines[t])
                        # main += "             assume((pc_cs >= pc[%s]) & (pc_cs <= %s));\n" % (i, self.__lines[t])
                    main += "             %s(%s);\n" % (t,
                                                        'threadargs[%s]' % i)
                    main += "             pc[%s] = pc_cs;\n" % i
                    if self.__backend == 'framac':
                        # main += "             assume(pc[%s] <= %s);\n" % (i, self.__lines[t])
                        # main += "             %s\n" % self._generateConstraintsFramaC('pc[%s]' % i, self.__lines[t])
                        pass
                    main += "          }\n\n"
                    i += 1

        # Last call to main()
        round = ROUNDS - 1
        main += "          thread_index = 0;\n"
        main += "          if (active_thread[0] == 1) {\n"
        if self.__backend == 'framac':
            main += "             pc_cs = Frama_C_unsigned_int_interval(pc[0], %s);\n" % self.__lines[
                'main']
            main += "             %s\n" % self._generateConstraintsFramaC(
                'pc_cs', self.__lines['main'])
            # main += "             assume(pc_cs <= %s);\n" % (self.__lines['main'])
        elif self.__backend == 'cbmc':
            main += "             pc_cs = pc[0] + guess_pc();\n"
            main += "             assume((pc_cs >= pc[0]) && (pc_cs <= %s));\n" % (
                self.__lines['main'])
            # main += "             assume((pc_cs >= pc[0]) & (pc_cs <= %s));\n" % (self.__lines['main'])
        else:
            main += "             pc_cs = pc[0] + guess_pc();\n"
            main += "             assume((pc_cs >= pc[0]) && (pc_cs <= %s));\n" % (
                self.__lines['main'])
            # main += "             assume((pc_cs >= pc[0]) & (pc_cs <= %s));\n" % (self.__lines['main'])
        main += "             main_thread();\n"
        main += "          }\n\n"
        if self.__backend == 'framac':
            main += "   _Bool __FRAMAC_spec = __FRAMAC_error;\n"
        main += "   return 0;\n"

        main += "}\n\n"
        return main

    # Checks whether variable  v  from function  f  is an array.
    #
    def __isArray(self, f, v):
        if v in self.Parser.varNames[f] and self.Parser.varArity[f, v] > 0:
            return True
        elif v in self.Parser.varNames[''] and self.Parser.varArity['', v] > 0:
            return True
        else:
            return False

    def __isVariable(self, f, v):
        if v in self.Parser.varNames[f] and f not in self.Parser.structName:
            return True
        else:
            return False

    def __isScalar(self, f, v):
        if v in self.Parser.varNames[f] and '*' not in self.Parser.varType[f, v] and self.Parser.varArity[f, v] == 0:
            return True
        elif v in self.Parser.varNames[''] and '*' not in self.Parser.varType['', v] and self.Parser.varArity['', v] == 0:
            return True
        else:
            return False

    # Checks whether variable  v  from function  f  is a pointer.
    #
    def __isPointer(self, f, v):
        if v in self.Parser.varNames[f] and self.Parser.varType[f, v].endswith('*'):
            return True
        elif v in self.Parser.varNames[''] and self.Parser.varType['', v].endswith('*'):
            return True
        else:
            return False

    # Checks whether variable  v  from function  f  is global.
    #
    def __isGlobal(self, f, v):
        if (v in self.Parser.varNames[''] and v not in self.Parser.varNames[f]):
            return True
        else:
            return False

    # Check whether the given AST node accesses global memory or uses a pointer.
    def __globalAccess(self, stmt):
        oldStmtCount = self.__stmtCount             # backup counters
        oldMaxInCompound = self.__maxInCompound
        oldGlobalMemoryAccessed = self.__globalMemoryAccessed

        globalAccess = False
        self.__globalMemoryAccessed = False

        if type(stmt) not in (pycparser.c_ast.If, ):
            tmp = self.visit(stmt)
        else:
            tmp = self.visit(stmt.cond)

        globalAccess = self.__globalMemoryAccessed

        self.__stmtCount = oldStmtCount             # restore counters
        self.__maxInCompound = oldMaxInCompound
        self.__globalMemoryAccessed = oldGlobalMemoryAccessed

        return globalAccess

    def _generateConstraintsFramaC(self, varname, threadline):
        cond = ' || '.join(['%s==%s' % (varname, i)
                            for i in range(0, threadline + 1)])
        cond = '/*@ assert ' + cond + ';*/'
        return cond

    def loadfromstring(self, string, env):
        threads = env.threads
        rounds = env.rounds
        unwind = env.unwind
        backend = env.format
        self.__backend = backend
        self.__threadbound = threads
        self.__argc = env.argc
        self.__argv = env.argv

        # super(LabelerRC, self).loadfromstring(string, env)
        self.cseqenv = env
        self.input = string

        # save current time
        timeBefore = time.time()
        if self.cseqenv.debug:
            print "/* " + self.cseqenv.currentmodule,

        self.Parser.reset()  # resets all the parser datastructs
        self.Parser.loadfromstring(string)
        self.ast = self.Parser.ast
        self.output = self.visit(self.ast)

        # Attach main thread in the end
        self.output += self.__main_thread

        fileno = str(self.cseqenv.transforms + 1).zfill(2)

        # # Truc's modification
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_raw__%s.c' % (self.cseqenv.debugpath,
                                                      fileno, self.cseqenv.currentmodule), self.output)

        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_symbol__%s.c' % (self.cseqenv.debugpath,
                                                         fileno, self.cseqenv.currentmodule), str(self.Parser.printsymbols()))

        # Truc's modification, fix indentation
        self.output = self.fixIndentation(self.output)

        # save debug information if needed
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_input___%s.c' % (self.cseqenv.debugpath,
                                                         fileno, self.cseqenv.currentmodule), self.input)

        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_marked__%s.c' % (self.cseqenv.debugpath,
                                                         fileno, self.cseqenv.currentmodule), self.output)

        # this will generate the line map and remove line markers from
        # self.output
        self.generatelinenumbers()

        #
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_output__%s.c' % (self.cseqenv.debugpath,
                                                         fileno, self.cseqenv.currentmodule), self.output)
            print "[%s] ok %0.2fs */" % (fileno, int(time.time()) - int(timeBefore))
            self.savelinenumbers()

        # Add the new main().
        #
        self.output += self.__createMain(rounds)

        # Thread sizes (i.e. number of visible statements) comma separated.
        #
        lines = ''
        i = 0
        maxsize = 0
        for t in self.__threadName:
            if i <= self.__threadbound:
                if i > 0:
                    lines += ', '
                lines += str(self.__lines[t])
                maxsize = max(int(maxsize), int(self.__lines[t]))
            i += 1

        # Generate the header.
        #
        import os.path
        # Find current directory of this file
        dirname = os.path.dirname(sys.argv[0]) + '/cseqinclude/'

        if self.__backend == 'cbmc':
            newseqincludefile = dirname + 'newseq-includeRC-cbmc.c'
        elif self.__backend == 'esbmc':
            newseqincludefile = dirname + 'newseq-includeRC-esbmc.c'
        elif self.__backend == 'llbmc':
            newseqincludefile = dirname + 'newseq-includeRC-llbmc.c'
        elif self.__backend == '2ls':
            newseqincludefile = dirname + 'newseq-includeRC-cbmc.c'
        elif self.__backend == 'klee':
            newseqincludefile = dirname + 'newseq-includeRC-klee.c'
        elif self.__backend == 'framac':
            newseqincludefile = dirname + 'newseq-includeRC-framac.c'
        elif self.__backend == 'cpachecker':
            newseqincludefile = dirname + 'newseq-includeRC-cpa.c'
        elif self.__backend == 'pagai':
            newseqincludefile = dirname + 'newseq-includeRC-pagai.c'
        else:
            newseqincludefile = dirname + 'newseq-includeRC-general.c'

        # Modifying header date
        header = core.utils.printFile(newseqincludefile)
        header = header.replace('<insert-version-here>', '0.0.0.0.0.0')
        header = header.replace('<insert-maxthreads-here>', str(threads))
        header = header.replace('<insert-maxrounds-here>', str(rounds))
        header = header.replace('<insert-unwind-here>', str(unwind))
        header = header.replace('<insert-backend-here>', str(backend))
        header = header.replace('<insert-date-here>',
                                strftime("%Y-%m-%d %H:%M:%S", gmtime()))
        # other thread parameters, they are transformed to global vars...
        header = header.replace('<insert-threads-params-here>', '')

        # Additional header from the preprocessor
        if self.cseqenv.header != '':
            if not os.path.isfile(self.cseqenv.header):
                print "Header file (%s) is missing!\n" % self.cseqenv.header
                sys.exit(2)
            listheader = core.utils.printFileRows(self.cseqenv.header)
            header = header.replace('<insert-headers-here>\n', listheader)
        else:
            header = header.replace('<insert-headers-here>\n', '')

        # Instrumentation.
        #
        if backend == 'cbmc' or backend == '2ls':
            if self.cseqenv.preprocessed:
                header = header.replace('<insert-extraheaders-here>', '')
                header = header.replace(
                    '<insert-size_t-decl-here>', 'typedef unsigned int size_t;')
            else:
                header = header.replace(
                    '<insert-extraheaders-here>', '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>')
                header = header.replace('<insert-size_t-decl-here>', '')
            k = int(math.floor(math.log(maxsize, 2))) + 1
            header = header.replace(
                '<insert-active_thread-type-here>', '__CPROVER_bitvector[1]')
            header = header.replace(
                '<insert-pc-type-here>', '__CPROVER_bitvector[%s]' % (k))
            # pc_cs stores the sum of two bitvectors of size(k, needs to be one
            # bit wider (k+1)
            header = header.replace(
                '<insert-pc_cs-type-here>', '__CPROVER_bitvector[%s]' % (k + 1))
            header = header.replace('<insert-thread_index-type-here>', 'int')
            header = header.replace(
                '<insert-thread_lines-type-here>', '__CPROVER_bitvector[%s]' % (k))
            header = header.replace(
                '<insert-guesspc-type-here>', '__CPROVER_bitvector[%s]' % (k))
            # header = header.replace('<insert-guesspc-type-here>', 'int')    #
            # Why changing back?
        elif backend == 'esbmc':
            if self.cseqenv.preprocessed:
                header = header.replace('<insert-extraheaders-here>', '')
                header = header.replace(
                    '<insert-size_t-decl-here>', 'typedef unsigned int size_t;')
            else:
                header = header.replace(
                    '<insert-extraheaders-here>', '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>')
                header = header.replace('<insert-size_t-decl-here>', '')
        elif backend == 'llbmc':
            if self.cseqenv.preprocessed:
                header = header.replace('<insert-extraheaders-here>', '')
                header = header.replace(
                    '<insert-size_t-decl-here>', 'typedef unsigned int size_t;')
            else:
                header = header.replace(
                    '<insert-extraheaders-here>', '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <stdint.h>')
                header = header.replace('<insert-size_t-decl-here>', '')
        elif backend == 'smack':
            header = header.replace('<insert-extraheaders-here>\n',
                                    '#define SMACK_H_\n#define __VERIFIER_assume __SMACK_assume\n#define __VERIFIER_assert __SMACK_assert\n#define assert __SMACK_assert\nvoid __SMACK_assert(int val);\nvoid __SMACK_assume(int val);\nint __SMACK_nondet(void);\n\n')
            header = header.replace('<insert-assume-here>', '__SMACK_assume')
            header = header.replace('<insert-assert-here>', 'assert')
            header = header.replace(
                '<insert-nondet_int-here>', '__SMACK_nondet')
            header = header.replace(
                '<insert-nondet_uint-here>', '__SMACK_nondet')
            header = header.replace(
                '<insert-nondet_int-body-here>', 'int nondet_int();')
            header = header.replace(
                '<insert-nondet_uint-body-here>', 'unsigned int nondet_uint();')
        elif backend == 'satabs':
            header = header.replace('<insert-extraheaders-here>\n', '')
            header = header.replace('<insert-assume-here>', '__CPROVER_assume')
            header = header.replace('<insert-assert-here>', 'assert')
            header = header.replace('<insert-nondet_int-here>', 'nondet_int')
            header = header.replace('<insert-nondet_uint-here>', 'nondet_uint')
            header = header.replace(
                '<insert-nondet_int-body-here>', 'int nondet_int();')
            header = header.replace(
                '<insert-nondet_uint-body-here>', 'unsigned int nondet_uint();')
        elif backend == 'klee':
            header = header.replace('<insert-extraheaders-here>', '')
        elif backend == 'cpachecker':
            header = header.replace('<insert-extraheaders-here>\n', '')
            header = header.replace(
                '<insert-assume-here>', '__VERIFIER_assume')
            header = header.replace(
                '<insert-assert-here>', '__VERIFIER_assert')
            header = header.replace('<insert-nondet_int-here>', 'nondet_int')
            header = header.replace('<insert-nondet_uint-here>', 'nondet_uint')
            header = header.replace(
                '<insert-nondet_int-body-here>', 'int nondet_int();')
            header = header.replace(
                '<insert-nondet_uint-body-here>', 'unsigned int nondet_uint();')
            # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            header = header.replace(
                '<insert-active_thread-type-here>', '_Bool')
            header = header.replace('<insert-pc-type-here>', 'int')
            header = header.replace('<insert-pc_cs-type-here>', 'int')
            header = header.replace('<insert-thread_index-type-here>', 'int')
            header = header.replace('<insert-thread_lines-type-here>', 'int')
        elif backend == 'framac':
            header = header.replace('<insert-extraheaders-here>\n', '')
        elif backend == 'pagai':
            if self.cseqenv.preprocessed:
                header = header.replace('<insert-extraheaders-here>', '')
                header = header.replace(
                    '<insert-size_t-decl-here>', 'typedef unsigned int size_t;')
            else:
                header = header.replace(
                    '<insert-extraheaders-here>', '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>')
                header = header.replace('<insert-size_t-decl-here>', '')

        header = header.replace('<insert-threadsizes-here>', lines)

        # Fix gotos by inserting ASS_GOTO(..) blocks before each goto,
        # excluding gotos which destination is the line below.
        #
        for (a, b) in self.__labelLine:
            if (a, b) in self.__gotoLine and (self.__labelLine[a, b] == self.__gotoLine[a, b] + 1):
                self.output = self.output.replace('<%s,%s>' % (a, b), '')
            else:
                self.output = self.output.replace('<%s,%s>' % (
                    a, b), 'ASS_GOTO(%s)' % self.__labelLine[a, b])

        # Fix indentation.
        #
        new = ''

        for line in self.output.splitlines():
            if len(line) > 0 and (line[0] == self.__startChar or line.startswith('IF(')):
                new += line + '\n'
            else:
                new += self.__labelLength * ' ' + line + '\n'

        # Truc's modification
        # Fix linemapping.
        #
        faketypedefsfile = dirname + '_fake_typedefs.h'
        len_faketypedefs = core.utils.countlineFile(faketypedefsfile) - 3
        len_newseqinclude = core.utils.countlineFile(newseqincludefile)
        offset = len_faketypedefs - len_newseqinclude  # TODO: calculate

        self.newoutputtoinput = {}

        for j in self.outputtoinput:
            if j > len_faketypedefs:
                self.newoutputtoinput[j - offset] = self.outputtoinput[j]
                # print "%s <- %s" % (i, i-offset)

        self.outputtoinput = self.newoutputtoinput
        if env.debug:
            self.savelinenumbers()  # overwrite map with correct values

        header2 = self.__create_mem_header()

        self.output = header + header2 + '\n\n' + new
