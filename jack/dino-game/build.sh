#!/bin/env bash
set  -e
mkdir build
nand2tetris compile src build 
assemble=true
disable_comments=false



if [ "$disable_comments" = true ] ; then
  disable_comments_flag="--dc"
else
  disable_comments_flag=""
fi

cd build
ls *.vm |xargs -I {} python ../count_functions.py {}
ls *.vm |xargs -I {} python ../fix_labels.py {}
if [ "$assemble" = true ] ; then
  hack-vm-translator $disable_comments_flag .
  wc -l *.asm |sort -rn
fi
echo "Functions count list"
echo ""
sqlite3 ../stats.db "SELECT * FROM functions ORDER BY count desc LIMIT 10"
# echo ""
# rm *.jack
# wc -l *.vm |sort -rn
CPUEmulator.sh
cp Linked.asm ~/