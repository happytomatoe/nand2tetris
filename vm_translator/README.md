# VM translator 
VM translator that translates jack into hack assembly. 
Solution for https://www.nand2tetris.org/project07. 

# Specification
## Memory segments

| Segment | Role | Memory Range | Description | 
|----------|----------|--------------|------------------|
| Local | Function's local variables | RAM[16-2047] | Stores the function's local variables, 2032 slots |
| Argument | Function's arguments | RAM[4-12] | Stores the function's arguments, 9 slots |
| This | Caller's 'this' | RAM[3] | Stores the 'this' of the caller |
| That | Caller's 'that' | RAM[4] | Stores the 'that' of the caller |
| Static | Static variables | RAM[16-2047] | Stores the static variables, 2032 slots |
| Constant | Constant values | N/A | Stores the constant values 0-32767 |
| Temp | Temporary storage | RAM[5-12] | Mapped directly to RAM locations 5-12, 8 slots |
| Pointer | Manipulate this & that | RAM[3-4] | Used to manipulate the 'this' and 'that' pointers |
| Temp | Temporary storage | RAM[5-12] | Mapped directly to RAM locations 5-12, 8 slots |

![alt text](doc/memory_seg.png)
![alt text](doc/memory_seg_2.png)

## Semantics
`push` segment index: Pushes the value of segment[index] onto the stack, where segment is argument, local, static, this, that, pointer, or temp and index is a nonnegative integer.
``
push local 0
``
`pop` segment index: Pops the top stack value and stores it in segment[index], where segment is argument, local, static, this, that, pointer, or temp and index is a nonnegative integer.
``
pop local 0
``
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

```text
push constant 21
push constant 22
add
pop constant 21
```
Operations operate on top 2 elements in the stack