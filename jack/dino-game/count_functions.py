# l='/home/babkamen/git/nand2tetris/nand2tetris/jack/dino-game/build/CactusQueue.vm'
import re
import sys
import sqlite3
if __name__ == '__main__':
    l = sys.argv[1]

   
    # print("File",l)
    with open(l) as f:
        lines = f.readlines()
   
    c=0
    arr = []
    for line in lines:
        if(line.startswith("function ")):
            if(f):
                arr.append((f,c))
            f = re.search(r"function ([\w.]+)", line).group(1)
            c=0
        else: 
            c+=1
    if c>0:
        arr.append((f,c))

    if not arr:
        raise Exception("Didn't find functions in a file")

    arr = arr[1:]
    col_w = [len(a[0]) for a in arr ]
    col_w= max(col_w)
    sorted = sorted(arr, key=lambda tup: tup[1], reverse=True)
    # for a in sorted:
    #     print(a[0].ljust(col_w,' '), a[1])
    with  sqlite3.connect("../stats.db") as con:
        cur = con.cursor()
        cur.execute("CREATE TABLE IF NOT EXISTS functions(file, function, count)")
        cur.executemany(f"INSERT INTO functions VALUES('{l}',?, ?)", sorted)
        con.commit()
   