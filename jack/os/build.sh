#!/bin/env bash
set  -e
rm -rf build
mkdir build
nand2tetris compile src build 



if [ "$disable_comments" = true ] ; then
  disable_comments_flag="--dc"
else
  disable_comments_flag=""
fi

cd build
cat Main.vm > Linked.vm
ls -I Main.vm -I Linked.vm | xargs -I {} cat {} >> Linked.vm
cat Linked.vm | xclip -sel clip