#!/bin/env bash
mkdir build
nand2tetris compile src build 
cd build
ls *.vm |xargs -I {} python ../count_functions.py {}
ls *.vm |xargs -I {} python ../fix_labels.py {}
hack-vm-translator .


# echo "Functions count list"
# echo ""
# sqlite3 ../stats.db "SELECT * FROM functions ORDER BY count desc LIMIT 10"
# echo ""
# rm *.jack
# wc -l *.vm |sort -rn
