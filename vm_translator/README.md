# VM translator 
VM translator that translates jack into hack assembly. 
Solution for https://www.nand2tetris.org/project07. 

# Specification
## Memory segments


![alt text](doc/memory_seg.png)
![alt text](doc/memory_seg_2.png)

## Semantics
`push` segment index: Pushes the value of segment[index] onto the stack, where segment is argument, local, static, this, that, pointer, or temp and index is a nonnegative integer.

`pop` segment index: Pops the top stack value and stores it in segment[index], where segment is argument, local, static, this, that, pointer, or temp and index is a nonnegative integer.
## Arrithmetic-logical commands

|cmd  |  op     | comment
|-----|---------|---------------------------------------
|add  | x + y   | integer addition (two's complement)
|sub  | x - y   | integer subtraction (two's complement)
|neg  | -y      | arithmetic negation (two's complement)
|eq   | x == y  | equality
|gt   | x > y   | greater than
|lt   | x < y   | less than
|and  | x And y | bit-wise And
|or   | x Or y  | bit-wise Or
|not  | Not y   | bit-wise Not