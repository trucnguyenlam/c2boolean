#!/bin/bash

/home/trucnguyenlam/Tools/clang+llvm-3.3/bin/clang -emit-llvm -c -g $1 -o $1'.bc'
llbmc -no-max-function-call-depth-checks -no-memory-free-checks -no-shift-checks -no-memcpy-disjoint-checks -no-memory-access-checks -no-memory-allocation-checks --max-loop-iterations=1 --no-max-loop-iterations-checks --ignore-missing-function-bodies -no-overflow-checks -no-div-by-zero-checks $1'.bc'