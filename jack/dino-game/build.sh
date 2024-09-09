#!/bin/env bash

cp src/*.jack build/
cd build 
rm -f Out.vm
JackCompiler.sh 
rm *.jack
wc -l *.vm |sort -rn
cat *.vm>Out.vm
c=$(wc -l < Out.vm)
if [[  "$c" -gt 32767 ]]
then
  echo "Output file is too big. Program counter will go into negative"
  rm *.vm
  exit 1
fi