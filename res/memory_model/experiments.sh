#!/bin/bash
#
#
# runexec --output mem_ver1_unwind4.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver1.c --unwind 4 --no-unwinding-assertions
# runexec --output mem_ver1_unwind5.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver1.c --unwind 5 --no-unwinding-assertions
# runexec --output mem_ver1_unwind6.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver1.c --unwind 6 --no-unwinding-assertions
# runexec --output mem_ver1_unwind7.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver1.c --unwind 7 --no-unwinding-assertions
# runexec --output mem_ver1_unwind8.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver1.c --unwind 8 --no-unwinding-assertions
runexec --output mem_ver2_unwind4.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver2.c --unwind 4 --no-unwinding-assertions
runexec --output mem_ver2_unwind5.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver2.c --unwind 5 --no-unwinding-assertions
runexec --output mem_ver2_unwind6.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver2.c --unwind 6 --no-unwinding-assertions
runexec --output mem_ver2_unwind7.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver2.c --unwind 7 --no-unwinding-assertions
# runexec --output mem_ver2_unwind8.log -- cbmc-5.2 merge_sort_false-unreach-call_Mem_ver2.c --unwind 8 --no-unwinding-assertions