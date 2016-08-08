import math
import re
import time
from time import gmtime, strftime
import pycparser.c_parser, pycparser.c_ast, pycparser.c_generator
import core.common, core.module, core.parser, core.utils
import sys

"""
    CSeq
    Copyright 2011-2015 Truc Nguyen Lam, University of Southampton.

    >  Labeler Semi-automatic translation to Interproc with precise PC (inline all thread into main function) <

"""
VERSION = 'labeler-1.0-2015.01.11'   # CSeq-1.0 Release

"""

Transformation:
    - inserting labels at the beginning of each relevant line of code
    - instrumentation
    - renames main() to main_thread()

Prerequisites:
    - all functions should have been inlined, except the main(), all thread functions,
    all __VERIFIER_atomic_ functions, and function __VERIFIER_assert
    - all loops should habe been unrolled
    - no two threads refers to the same thread function (use module duplicator.py)

TODO:
    - check the STOP() inserting mechanism
    - calculate max length of added stamps, and change label padding accordingly

Changelog:
    2016.06.08  change following labelerRC (2016.05.23)
    2015.06.18  remove __VERIFIER_atomic_begin and __VERIFIER_atomic_end from code
    2015.03.05  support __VERIFIER_atomic_begin and __VERIFIER_atomic_end (require not nesting atomic call)
    2015.02.22  fix translation to FramaC
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


class LabelerRCInterproc(core.module.Module):
    __lines = {}                     # lines for each thread
    __threadName = ['main']          # name of threads, as they are found in pthread_create(s) - the threads all have different names
    __threadIndex = {}               # index of the thread = value of threadcount when the pthread_create to that thread was discovered
    __threadCount = 0                # pthread create()s found so far

    __labelLine = {}                 # statement number where labels are defined [function, label]
    __gotoLine = {}                  # statement number where goto to labels appear [function, label]
    __maxInCompound = 0              # max label within a compound
    __labelLength = 85               # for labels to have all the same length, adding padding when needed
    __startChar = 't'                # special char to distinguish between labeled and non-labelled lines

    __stmtCount = -1                 # thread statement counter (to build thread labels)

    __currentThread = ''             # name of the current thread (also used to build thread labels)

    __threadbound = 0                # bound on the number of threads

    __firstThreadCreate = False      # set once the first thread creation is met
    __globalMemoryAccessed = False   # used to limit context-switch points (when global memory is not accessed, no need to insert them)

    # Truc's modification
    __visitMain = False
    __atomic_stack = []
    __inAtomicRegion = False

    def loadfromstring(self, string, env):
        self.__threadCopy = {}     # Hold copy of each thread
        self.__threadVars = {}
        self.__inThreadFunc = False
        self.__threadAssignedID = {}
        self.kbits = {}

        threads = env.threads
        rounds = env.rounds
        unwind = env.unwind
        backend = env.format
        self.__backend = 'interproc'
        self.__threadbound = threads

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

        fileno = str(self.cseqenv.transforms + 1).zfill(2)

        # # Truc's modification
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_raw__%s.c' % (self.cseqenv.debugpath, fileno, self.cseqenv.currentmodule), self.output)

        # Truc's modification, fix indentation
        self.output = self.fixIndentation(self.output)

        # save debug information if needed
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_input___%s.c' % (self.cseqenv.debugpath, fileno, self.cseqenv.currentmodule), self.input)

        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_marked__%s.c' % (self.cseqenv.debugpath, fileno, self.cseqenv.currentmodule), self.output)
            # utils.saveFile('_%s_%s.ast.c' % (cseqenv.transforms,moduleName), str(m.Parser.ast.show())) TODO
            # utils.saveFile('_%s_%s.symbols.c' % (cseqenv.transforms,moduleName),
            # str(m.Parser.printsymbols())) TODO

        # this will generate the line map and remove line markers from self.output
        self.generatelinenumbers()

        #
        if self.cseqenv.debug:
            core.utils.saveFile('%s/_%s_output__%s.c' % (self.cseqenv.debugpath, fileno, self.cseqenv.currentmodule), self.output)
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

        if self.__backend == 'interproc':
            newseqincludefile = dirname + 'newseq-includeRC-interproc.c'
        else:
            newseqincludefile = dirname + 'newseq-includeRC-general.c'

        # Modifying header date
        header = core.utils.printFile(newseqincludefile)
        header = header.replace('<insert-version-here>', '0.0.0.0.0.0')
        header = header.replace('<insert-maxthreads-here>', str(threads))
        header = header.replace('<insert-maxrounds-here>', str(rounds))
        header = header.replace('<insert-unwind-here>', str(unwind))
        header = header.replace('<insert-backend-here>', str(backend))
        header = header.replace('<insert-date-here>', strftime("%Y-%m-%d %H:%M:%S", gmtime()))
        header = header.replace('<insert-threads-params-here>', '')  # other thread parameters, they are transformed to global vars...

        # Additional header from the preprocessor
        if self.cseqenv.header != '':
            if not os.path.isfile(self.cseqenv.header):
                print "Header file (%s) is missing!\n" % self.cseqenv.header
                exit(2)
            listheader = core.utils.printFileRows(self.cseqenv.header)
            header = header.replace('<insert-headers-here>\n', listheader)
        else:
            header = header.replace('<insert-headers-here>\n', '')

        # Instrumentation.
        #
        if backend == 'cbmc':
            if self.cseqenv.preprocessed:
                header = header.replace('<insert-extraheaders-here>', '')
                header = header.replace('<insert-size_t-decl-here>', 'typedef unsigned int size_t;')
            else:
                header = header.replace('<insert-extraheaders-here>', '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>')
                header = header.replace('<insert-size_t-decl-here>', '')
            k = int(math.floor(math.log(maxsize, 2))) + 1
            header = header.replace('<insert-active_thread-type-here>', '__CPROVER_bitvector[1]')
            header = header.replace('<insert-pc-type-here>', '__CPROVER_bitvector[%s]' % (k))
            # pc_cs stores the sum of two bitvectors of size(k, needs to be one bit wider (k+1)
            header = header.replace('<insert-pc_cs-type-here>', '__CPROVER_bitvector[%s]' % (k+1))
            header = header.replace('<insert-thread_index-type-here>', 'int')
            header = header.replace('<insert-thread_lines-type-here>', '__CPROVER_bitvector[%s]' % (k))

        header = header.replace('<insert-threadsizes-here>', lines)

        # Fix gotos by inserting ASS_GOTO(..) blocks before each goto,
        # excluding gotos which destination is the line below.
        #
        for (a,b) in self.__labelLine:
            if (a,b) in self.__gotoLine and (self.__labelLine[a,b] == self.__gotoLine[a,b]+1):
                self.output = self.output.replace('<%s,%s>' % (a, b), '')
            else:
                self.output = self.output.replace('<%s,%s>' % (a, b), 'ASS_GOTO(%s)' % self.__labelLine[a,b])

        # Fix indentation.
        #
        new = self.output

        # Truc's modification
        # Fix linemapping.
        #
        faketypedefsfile = dirname + '_fake_typedefs.h'
        len_faketypedefs = core.utils.countlineFile(faketypedefsfile) - 3
        len_newseqinclude = core.utils.countlineFile(newseqincludefile)
        offset = len_faketypedefs-len_newseqinclude  # TODO: calculate

        self.newoutputtoinput = {}

        for j in self.outputtoinput:
            if j > len_faketypedefs:
                self.newoutputtoinput[j-offset] = self.outputtoinput[j]
                # print "%s <- %s" % (i, i-offset)

        self.outputtoinput = self.newoutputtoinput
        if env.debug:
            self.savelinenumbers()  # overwrite map with correct values

        self.output = header + '\n\n' + new

    def visit_Compound(self, n):
        s = ''
        self.indent_level += 1
        threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0

        # Insert the labels at the beginning of each statement,
        # with a few exclusions to reduce context-switch points...
        #
        if n.block_items:
            for stmt in n.block_items:
                # Case 1: last statement in a thread (must correspond to last label)
                if (type(stmt) == pycparser.c_ast.FuncCall and
                        stmt.name.name == core.common.changeID['pthread_exit']):
                    self.__stmtCount += 1
                    self.__maxInCompound = self.__stmtCount
                    stamp = '<round>%s%s_%s$ ' % (self.__startChar, self.__currentThread, str(self.__stmtCount))
                    padding = ' '*(self.__labelLength - len(stamp))
                    outputcode = self.visit(stmt)
                    # Truc's modification
                    marker = outputcode[outputcode.find('#'):]
                    marker = marker[:marker.find('\n')+1]
                    code = outputcode[outputcode.find('\n')+1:]
                    # Truc's modification
                    newStmt = marker + stamp + padding + code
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'

                # Case 1.a: printf family functions will be ignore
                elif (type(stmt) == pycparser.c_ast.FuncCall and
                        stmt.name.name in ('fprintf', 'printf', 'fputs', 'puts')):    # Non exhastive list
                    newStmt = self.visit(stmt)
                    s += 'skip ;\n'

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
                        stamp = 'if ((pc_%s > <uint>%s) or (pc_cs_%s <= <uint>%s)) then goto <round>%s%s_%s; endif;' % (threadIndex, str(self.__stmtCount), threadIndex, str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
                        padding = ' '*(self.__labelLength - len(stamp))
                        newStmt = stamp + padding + self.visit(stmt.stmt)
                    elif (globalAccess or
                            # Suggestion from Lazy-CSeq-SP
                            # (type(stmt) == pycparser.c_ast.FuncCall and
                            # stmt.name.name ==
                            # core.common.changeID['pthread_create']) or
                            (type(stmt.stmt) == pycparser.c_ast.FuncCall and
                                (
                                    stmt.stmt.name.name == core.common.changeID['pthread_join'] or
                                    # stmt.stmt.name.name == core.common.changeID['pthread_create'] or
                                    stmt.stmt.name.name.startswith("__VERIFIER_atomic") or
                                    stmt.stmt.name.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']) or
                                    stmt.stmt.name.name.startswith("__VERIFIER_assume")
                                ) and
                                (
                                   stmt.stmt.name.name != "__VERIFIER_atomic_end" or
                                    stmt.stmt.name.name != "__CS_atomic_end"
                                )
                            )):
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = '<round>%s%s_%s$ if ((pc_%s > <uint>%s) or (pc_cs_%s <= <uint>%s)) then goto <round>%s%s_%s; endif;' % (self.__startChar, self.__currentThread, str(self.__stmtCount), threadIndex, str(self.__stmtCount), threadIndex, str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
                        padding = ' '*(self.__labelLength - len(stamp))
                        newStmt = stamp + padding + self.visit(stmt.stmt)
                    else:
                        newStmt = self.visit(stmt.stmt)

                    newStmt = self._make_indent() + '<round>' + stmt.name + '$ ' + 'assume (pc_cs_%s >= <uint>%s);\n' % (threadIndex, self.__stmtCount+1) + newStmt

                    # Truc's modification, make sure the last semicolon is placed in the right place
                    if newStmt.endswith('\n'):
                        newStmt = newStmt[:-1]
                    if not newStmt.endswith(';'):
                        newStmt += ';'
                    s += newStmt + '\n'

                # Case 3: all the rest....
                elif (type(stmt) not in (pycparser.c_ast.Compound, pycparser.c_ast.Goto, pycparser.c_ast.Decl)
                    ##### and not (type(stmt) == pycparser.c_ast.FuncCall and self._parenthesize_unless_simple(stmt.name) == 'assert') Omar 19.Jan.2014 - missing context-switch points before assert()s
                    #and not (type(stmt) == pycparser.c_ast.FuncCall and self._parenthesize_unless_simple(stmt.name) == 'assume')
                    and not (self.__currentThread=='main' and self.__firstThreadCreate == False) or (self.__currentThread=='main' and self.__stmtCount == -1)) :

                    # --1-- Simulate a visit to the stmt block to see whether it makes any use of pointers or shared memory.
                    #
                    globalAccess = self.__globalAccess(stmt)
                    newStmt = ''

                    self.lines = []   # override core.module marking behaviour,
                                      # otherwise  module.visit()  won't insert any marker

                    # --2-- Now rebuilds the stmt block again,
                    #       this time using the proper formatting
                    #       (now we know if the statement is accessing global memory,
                    #       so to insert the stamp at the beginning when needed)
                    #
                    if self.__stmtCount == -1:   # first statement in a thread
                        self.__stmtCount += 1
                        self.__maxInCompound = self.__stmtCount
                        stamp = 'if ((pc_%s > <uint>%s) or (pc_cs_%s <= <uint>%s)) then goto <round>%s%s_%s; endif;' % (threadIndex, str(self.__stmtCount), threadIndex, str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
                        padding = ' '*(self.__labelLength - len(stamp))
                        outputcode = self.visit(stmt)
                        # Truc's modification
                        # TODO: need proper fix, this is dirty
                        if type(stmt) == pycparser.c_ast.Compound:
                            newStmt = stamp + padding + outputcode
                        else:
                            marker = outputcode[outputcode.find('#'):]
                            marker = marker[:marker.find('\n')+1]
                            code = outputcode[outputcode.find('\n')+1:]
                            newStmt = marker + stamp + padding + code

                    elif (self.__inAtomicRegion and    # New category
                            not ((type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith('__VERIFIER_atomic')) or
                                (type(stmt) == pycparser.c_ast.FuncCall and stmt.name.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic'])))):
                        newStmt = self.visit(stmt)

                    # elif (type(stmt) == pycparser.c_ast.FuncCall and     # Special for yield statement
                    #         stmt.name.name == "__cs_sched_yield"):


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
                        if (type(stmt) == pycparser.c_ast.FuncCall and (
                                stmt.name.name == '__cs_init_scalar' or
                                stmt.name.name == '__CS_atomic_end')):
                            newStmt = self.visit(stmt)
                        else:
                            self.__stmtCount += 1
                            self.__maxInCompound = self.__stmtCount
                            stamp = '<round>%s%s_%s$ if ((pc_%s > <uint>%s) or (pc_cs_%s <= <uint>%s)) then goto <round>%s%s_%s; endif;' % (self.__startChar, self.__currentThread, str(self.__stmtCount), threadIndex, str(self.__stmtCount), threadIndex, str(self.__stmtCount), self.__startChar, self.__currentThread, str(self.__stmtCount+1))
                            padding = ' '*(self.__labelLength - len(stamp))
                            outputcode = self.visit(stmt)
                            # Truc's modification
                            marker = outputcode[outputcode.find('#'):]
                            marker = marker[:marker.find('\n')+1]
                            code = outputcode[outputcode.find('\n')+1:]
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
        s += ''

        return s

    def visit_Decl(self, n, no_type=False):
        # no_type is used when a Decl is part of a DeclList, where the type is
        # explicitly only for the first delaration in a list.
        #
        s = n.name if no_type else self._generate_decl(n)

        if n.bitsize:
            s += ' : ' + self.visit(n.bitsize)
        if n.init:
            if isinstance(n.init, pycparser.c_ast.InitList):
                s += ' = {' + self.visit(n.init) + '}'
            elif isinstance(n.init, pycparser.c_ast.ExprList):
                s += ' = (' + self.visit(n.init) + ')'
            else:
                s += ' = ' + self.visit(n.init)

        if self.__inThreadFunc:
            self.__threadVars[self.__currentThread].append(s.replace('static ', ''))
            return ''

        return s

    def visit_FuncDef(self, n):
        if (n.decl.name.startswith('__VERIFIER_atomic_') or
                n.decl.name.startswith(core.common.funcPrefixChange['__VERIFIER_atomic']) or
                n.decl.name == '__VERIFIER_assert' or
                n.decl.name in self.Parser.funcReferenced):   # <--- functions called through pointers are not inlined yet
            return self.Parser.funcBlock[n.decl.name]

        self.__currentThread = n.decl.name
        # Reset for every thread
        self.__atomic_stack = []
        self.__inAtomicRegion = False
        if self.__currentThread == 'main' and self.__visitMain:
            print 'More than one main functions in the input file'
            exit(2)

        self.__firstThreadCreate = False

        decl = self.visit(n.decl)
        self.indent_level = 0
        self.__threadVars[self.__currentThread] = []    # Variables of current thread
        self.__inThreadFunc = True
        body = self.visit(n.body)
        self.__inThreadFunc = False

        f = ''    # Function
        self.__lines[self.__currentThread] = self.__stmtCount

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
            f = '%s main_thread(void)\n' % self.Parser.funcBlockOut[self.__currentThread]

            main_args = self.Parser.funcBlockIn['main']
            if main_args.find('void') != -1 or main_args == '':
                main_args = ''
            else:
                # Change *argv[] and argv[][] --> **argv
                main_args = re.sub(r'\*(.*)\[\]', r'** \1', main_args)
                main_args = re.sub(r'(.*)\[\]\[\]', r'** \1', main_args)
                # split argument
                main_args = main_args.split(',')
                if len(main_args) != 2:
                    print 'main function may have been defined wrongly'
                    sys.exit(2)
                # main_args = 'static ' + main_args[0] + '; static ' + main_args[1] + ';'
            # body = '{' + main_args + body[body.find('{')+1:]

        self.__threadCopy[self.__currentThread] = self.__fixIndent(body[body.find('{')+1:body.rfind('}')])

        f += body + '\n\n\n'

        # Truc's modification
        if self.__currentThread == 'main':
            self.__visitMain = True

        return ''
        return '\n/* The thread %s has been inlined in main function */\n' % self.__currentThread

    def visit_If(self, n):
        ifStart = self.__maxInCompound   # label where the if stmt begins

        s = 'if ('

        if n.cond:
            condition = self.visit(n.cond)
            s += condition

        s += ') then\n'
        s += self._generate_stmt(n.iftrue, add_indent=True)

        ifEnd = self.__maxInCompound   # label for the last stmt in the if block:  if () { block; }
        nextLabelID = ifEnd+1

        if n.iffalse:
            elseBlock = self._generate_stmt(n.iffalse, add_indent=True)

            elseEnd = self.__maxInCompound   # label for the last stmt in the if_false block if () {...} else { block; }

            if ifStart < ifEnd:
                threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
                elseHeader = 'assume (pc_cs_%s >= <uint>%s);' % (threadIndex, str(ifEnd+1))
            else:
                elseHeader = ''

            nextLabelID = elseEnd+1
            s += self._make_indent() + 'else\n'

            elseBlock = elseHeader + elseBlock
            s += elseBlock

        s += 'endif;'

        header = ''

        if ifStart+1 < nextLabelID:
            threadIndex = self.Parser.threadIndex[self.__currentThread] if self.__currentThread in self.Parser.threadIndex else 0
            footer = 'assume (pc_cs_%s >= <uint>%s);' % (threadIndex, nextLabelID)
        else:
            footer = ''

        return header + s + self._make_indent() + footer

    def visit_Return(self, n):
        if self.__currentThread != '__VERIFIER_assert' and self.__currentThread not in self.Parser.funcReferenced:
            print "error: labeler.py: return statement in thread '%s'.\n" % self.__currentThread
            exit(1)

        s = 'return'
        if n.expr:
            s += ' ' + self.visit(n.expr)
        return s + ';'

    def visit_Label(self, n):
        self.__labelLine[self.__currentThread, n.name] = self.__stmtCount
        return '<round>' + n.name + '$\n' + self._generate_stmt(n.stmt)

    def visit_Goto(self, n):
        self.__gotoLine[self.__currentThread, n.name] = self.__stmtCount
        extra = '<%s,%s>\n' % (self.__currentThread, n.name) + self._make_indent()
        extra = ''
        return extra + 'goto <round>' + n.name + ';'

    def visit_ID(self, n):
        # If this ID corresponds either to a global variable,
        # or to a pointer...
        #
        if ((self.__isGlobal(self.__currentThread, n.name) or self.__isPointer(self.__currentThread, n.name)) and
                not n.name.startswith('__cs_thread_local_')):
            # Truc's modication
            if not self.__inAtomicRegion:
                self.__globalMemoryAccessed = True
        # Rename the IDs of main() arguments
        # if self.__currentThread == 'main' and n.name in self.Parser.varNames['main'] and self.Parser.varKind['main',n.name] == 'p':
        #   return '__main_params_' + n.name

        return n.name

    def visit_FuncCall(self, n):
        fref = self._parenthesize_unless_simple(n.name)
        args = self.visit(n.args)
        # Truc's modification
        if fref == '__CS_atomic_begin':
            self.__atomic_stack.append(self.__currentThread)
            self.__inAtomicRegion = True
            return '//__CS_atomic_begin'
        if fref == '__CS_atomic_end':
            if len(self.__atomic_stack) == 0:  # There must be something wrong in the thread
                self.__inAtomicRegion = False
            elif len(self.__atomic_stack) == 1:   # Correct value
                self.__atomic_stack.pop()
                self.__inAtomicRegion = False
            else:   # Nested atomic region, must be something wrong
                self.__atomic_stack.pop()
                self.__inAtomicRegion = False
            return '//__CS_atomic_end'

        # 2016.01.28 Added to detect call to atomic function.
        # Is it true if all atomic functions must require context switch?
        if (fref.startswith("__VERIFIER_atomic_") or
                fref.startswith("__CS_atomic_")):
            if not self.__inAtomicRegion:
                self.__globalMemoryAccessed = True
            return fref + '(' + args + ')'

        # When a thread is created, extract its function name
        # based on the 3rd parameter in the pthread_create() call:
        #
        # pthread_create(&id, NULL, f, &arg);
        #                          ^^^
        #
        if fref == core.common.changeID['pthread_create']:
            id = args[:args.find(',')].replace('&', '').strip()
            fName = self.visit(n.args.exprs[2])
            fName = fName.replace('&', '').strip()
            if '\n' in fName:   # Some thing goes wrong with the name
                fName = fName[fName.rfind('\n') + 1:]

            ##print "checking fName = %s\n\n" % fName

            if fName not in self.__threadName:
                self.__threadName.append(fName)
                self.__threadCount = self.__threadCount + 1

                args = args + ', %s' % (self.__threadCount)
                self.__threadIndex[fName] = self.__threadCount
            else:
                # when visiting from the 2nd time on (if it happens),
                # reuse the old thread indexS!
                args = args + ', %s' % (self.__threadIndex[fName])

            self.__threadAssignedID[id] = self.__threadIndex[fName]

            self.__firstThreadCreate = True
            return 'active_thread_%s = true' % (self.__threadIndex[fName])

        if fref == core.common.changeID['pthread_join']:
            id = args[:args.find(',')].strip()
            if id not in self.__threadAssignedID:
                print "There is something wrong with pthread_join call args %s" % id
                print self.__threadAssignedID
                sys.exit(2)
            index = self.__threadAssignedID[id]
            return 'assume(pc_%s == <uint_%s><lines_%s>)' % (index, index, index)

        if fref == core.common.changeID['pthread_exit']:
            return 'skip'

        if fref == '__cs_init_scalar':
            return ''

        if fref == '__VERIFIER_assume':
            return 'assume(%s)' % args

        if fref == 'assert' or fref == '__VERIFIER_assert':
            if args == '0':
                return 'error =  true'
            return 'if (not (%s)) then error = true; endif' % args

        return fref + '(' + args + ')'

    def visit_EmptyStatement(self, n):
        return ''

    def visit_UnaryOp(self, n):
        operand = self._parenthesize_unless_simple(n.expr)
        if n.op == 'p++':
            return '%s = %s + 1' % (operand, operand)
        elif n.op == 'p--':
            return '%s = %s - 1' % (operand, operand)
        elif n.op == 'sizeof':
            # Always parenthesize the argument of sizeof since it can be
            # a name.
            return 'sizeof(%s)' % self.visit(n.expr)
        elif n.op == '!':
            if operand == '1':
                return 'false'
            return 'not %s' % operand
        else:
            return '%s%s' % (n.op, operand)

    def visit_Assignment(self, n):
        rval_str = self._parenthesize_if(n.rvalue, lambda n: isinstance(n, pycparser.c_ast.Assignment))
        lval_str = self.visit(n.lvalue)
        if n.op == '+=':
            return '%s = %s + %s' % (lval_str, lval_str, rval_str)
        elif n.op == '-=':
            return '%s = %s - %s' % (lval_str, lval_str, rval_str)
        else:
            return '%s %s %s' % (lval_str, n.op, rval_str)

    def visit_BinaryOp(self, n):
        lval_str = self._parenthesize_if(n.left, lambda d: not self._is_simple_node(d))
        rval_str = self._parenthesize_if(n.right, lambda d: not self._is_simple_node(d))

        if n.op == '||':
            return '%s or %s' % (lval_str, rval_str)
        elif n.op == '&&':
            return '%s and %s' % (lval_str, rval_str)
        else:
            return '%s %s %s' % (lval_str, n.op, rval_str)

    def __fixIndent(self, string):
        s = ''
        string = self.fixIndentation(string)
        # s = string
        for line in string.splitlines():
            if not line.startswith('# '):
                line = re.sub(r';[ \t]+;', r';', line)
                line = re.sub(r'\$[ \t]+;', r'$ skip;', line)
                if not re.match(r';[\n]*', line):
                    s += line + '\n'
        return s

    def __makeThreadCopy(self, round, threadName):
        # threadIndex = self.Parser.threadIndex[threadName] if threadName in self.Parser.threadIndex else 0
        uint = 'uint[%s]' % self.kbits[threadName]
        threadBody = self.__threadCopy[threadName]

        s = threadBody.replace('<round>', 'r%s' % round)
        s = s.replace('<uint>', uint)
        for i, t in enumerate(self.__threadName):
            s = s.replace('<lines_%s>' % i, str(self.__lines[t]))
            s = s.replace('<uint_%s>' % i, 'uint[%s]' % self.kbits[t])

        return s

    def __makeDeclGlobal(self):
        s = 'var\n'
        for i, t in enumerate(self.__threadName):
            s += 'active_thread_%s : bool,\n' % i
            s += 'pc_%s : uint[%s],\n' % (i, self.kbits[t])
            s += 'pc_cs_%s : uint[%s],\n' % (i, self.kbits[t])

        for t in self.__threadVars:
            # s += '/* Declarations of %s */\n' % t
            for d in self.__threadVars[t]:
                s += d + ',\n'

        s += 'error: bool;\n\n'

        return s

    def __makeInitGlobal(self):
        s = 'error = false;\n'
        for i, t in enumerate(self.__threadName):
            if i == 0:
                s += 'active_thread_%s = true;\n' % i
            else:
                s += 'active_thread_%s = false;\n' % i
            s += 'pc_%s = uint[%s]0;\n' % (i, self.kbits[t])

        return s + '\n'

    def __createMain(self, ROUNDS):
        # the new main() is created according to the following sort of scheduling:
        #
        # main_thread    t1 t2    t1 t2   t1 t2    t1 t2     t1 t2    t1 t2      t1 t2    t1 t2    main_thread
        #

        # Calculate thread lines max bits

        for t in self.__lines:
            k = int(math.floor(math.log(self.__lines[t], 2))) + 1
            self.kbits[t] = k

        round = 0

        main = ''

        main += self.__makeDeclGlobal()

        main += "begin\n"

        main += self.__makeInitGlobal()

        main += "          // round %s\n" % round
        # main += "          thread_index = 0;\n"
        main += "          pc_cs_0 = random;\n"
        main += "          assume(pc_cs_0 > pc_0 and pc_cs_0 <= uint[%s]%s);\n" % (self.kbits['main'], self.__lines['main'])
        main += self.__makeThreadCopy(round, 'main')
        main += "          pc_0 = pc_cs_0;\n"
        main += "\n"

        i = 1
        for t in self.__threadName:
            if t == 'main':
                continue
            if i <= self.__threadbound:
                # main += "          thread_index = %s;\n" % i
                main += "          if (active_thread_%s == true) then /** %s **/\n" % (i, t)
                main += "             pc_cs_%s = random;\n" % i
                main += "             assume(pc_cs_%s >= pc_%s and pc_cs_%s <= uint[%s]%s);\n" % (i, i, i, self.kbits[t], self.__lines[t])
                main += self.__makeThreadCopy(round, t)
                main += "             pc_%s = pc_cs_%s;\n" % (i, i)
                main += "          endif;\n\n"
                i += 1

        for round in range(1, ROUNDS):
            main += "          // round %s\n" % round
            # main += "          thread_index = 0;\n"
            main += "          if (active_thread_0 == true) then /** main **/\n"
            main += "              pc_cs_0 = random;\n"
            main += "              assume(pc_cs_0 >= pc_0 and pc_cs_0 <= uint[%s]%s);\n" % (self.kbits['main'], self.__lines['main'])
            main += self.__makeThreadCopy(round, 'main')
            main += "              pc_0 = pc_cs_0;\n"
            main += "          endif;\n\n"

            i = 1
            for t in self.__threadName:
                if t == 'main':
                    continue
                if i <= self.__threadbound:
                    # main += "          thread_index = %s;\n" % i
                    main += "          if (active_thread_%i == true) then /** %s **/\n" % (i, t)
                    main += "             pc_cs_%s = random;\n" % i
                    main += "             assume(pc_cs_%s >= pc_%s and pc_cs_%s <= uint[%s]%s);\n" % (i, i, i, self.kbits[t], self.__lines[t])
                    main += self.__makeThreadCopy(round, t)
                    main += "             pc_%s = pc_cs_%s;\n" % (i, i)
                    main += "          endif;\n\n"
                    i += 1

        # Last call to main()
        round = ROUNDS
        # main += "          thread_index = 0;\n"
        main += "          if (active_thread_0 == true) then\n"
        main += "             pc_cs_0 = random;\n"
        main += "             assume(pc_cs_0 >= pc_0 and pc_cs_0 <= uint[%s]%s);\n" % (self.kbits['main'], self.__lines['main'])
        main += self.__makeThreadCopy(round, 'main')
        main += "          endif;\n\n"

        main += "if error then skip; ERROR$ endif;\n"

        main += "end\n\n"
        return main

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


    # Check whether the given AST node accesses global memory or uses a pointer.
    #
    # TODO: to limit context-switch points,
    #       implement here variable dependency, pointer analysis, etc .....
    #
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