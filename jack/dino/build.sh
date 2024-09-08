#!/bin/bash

cd src && JackCompiler.sh && rm -f Out.vm  && cat *.vm>Out.vm && mv Out.vm ../build/ 
c=$(wc -l < ../build/Out.vm)
if [[  "$c" -gt 32767 ]]
then
  echo "Output file is too big. Program counter will go into negative"
  rm ../build/Out.vm
  exit 1
fi