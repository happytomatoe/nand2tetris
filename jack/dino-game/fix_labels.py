# l='/home/babkamen/git/nand2tetris/nand2tetris/jack/dino-game/build/CactusQueue.vm'
from functools import reduce
import re
import os
from pathlib import Path
import sys
import sqlite3
if __name__ == '__main__':
    l = sys.argv[1]

    # print("File",l)
    filename = Path(l).stem
    with open(l) as f:
        text = f.read()
    labels = [a.replace("label ","").strip() for a in text.split("\n") if a.startswith("label")]
    tranformed_labels = {" "+l:" "+filename+"_"+ l  for l in labels }
    new_text = reduce(lambda acc, l: acc.replace(l, tranformed_labels[l]), tranformed_labels, text)
    with open(l, "w") as f:
        f.write(new_text)
    
    
   