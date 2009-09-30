#!/bin/sh
llvm-dis -f ./c-runtime.bc
cat c-runtime.ll | sed 's/available_externally//' > c-runtime-fixed.ll
llvm-as ./c-runtime-fixed.ll -f -o c-runtime.bc
