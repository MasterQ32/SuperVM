# Assembler Mnemonics

## nop
Does nothing.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |       copy(0) |
| Result    |       discard |
| Set Flags |            no |

## push $arg
Pushes $arg on the stack.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       copy(0) |
| Result    |          push |
| Set Flags |            no |

## drop
Removes the top value from the stack.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       copy(0) |
| Result    |       discard |
| Set Flags |            no |

## dup
Duplicates the top value of the stack.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     peek,zero |
| Command   |       copy(0) |
| Result    |          push |
| Set Flags |            no |

## jmp $arg
Jumps to the command at index $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       copy(0) |
| Result    |          jump |
| Set Flags |            no |

## jmpi
Pops an index from the stack and jumps to it.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       copy(0) |
| Result    |          jump |
| Set Flags |            no |

## ret
Returns from a function. Is exactly the same as `jmpi`, but is ment for returning from a function instead of an indirected jump.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       copy(0) |
| Result    |          jump |
| Set Flags |            no |

## load8 $arg
Loads a byte (8 bit) from $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       load(0) |
| Result    |          push |
| Set Flags |            no |

## load16 $arg
Loads a half world (16 bit) from $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       load(1) |
| Result    |          push |
| Set Flags |            no |

## load32 $arg
Loads a word (32 bit) from $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       load(2) |
| Result    |          push |
| Set Flags |            no |

## load8i
Pops an address from the stack and loads the byte (8 bit) located at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       load(0) |
| Result    |          push |
| Set Flags |            no |

## load16i
Pops an address from the stack and loads the half world (16 bit) located at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       load(1) |
| Result    |          push |
| Set Flags |            no |

## load32i
Pops an address from the stack and loads the word (32 bit) located at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       load(2) |
| Result    |          push |
| Set Flags |            no |

## store8 $arg
Pops a value and stores it as a byte (8 bit) at $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      store(0) |
| Result    |       discard |
| Set Flags |            no |

## store16 $arg
Pops a value and stores it as a half word (16 bit) at $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      store(1) |
| Result    |       discard |
| Set Flags |            no |

## store32 $arg
Pops a value and stores the word (32 bit) at $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      store(2) |
| Result    |       discard |
| Set Flags |            no |

## store8i
First pops an address from the stack, then a value. Then stores the value as a byte (8 bit) at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |      store(0) |
| Result    |       discard |
| Set Flags |            no |

## store16i
First pops an address from the stack, then a value. Then stores the value as a half word (16 bit) at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |      store(1) |
| Result    |       discard |
| Set Flags |            no |

## store32i
First pops an address from the stack, then a value. Then stores the value (32 bit) at the popped address.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |      store(2) |
| Result    |       discard |
| Set Flags |            no |

## get $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |        get(0) |
| Result    |          push |
| Set Flags |            no |

## geti


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |        get(0) |
| Result    |          push |
| Set Flags |            no |

## set $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |        set(0) |
| Result    |       discard |
| Set Flags |            no |

## seti


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |        set(0) |
| Result    |       discard |
| Set Flags |            no |

## bpget
Pushes the current base pointer to the stack.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |      bpget(0) |
| Result    |          push |
| Set Flags |            no |

## bpset
Pops a value into the base pointer.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |      bpset(0) |
| Result    |       discard |
| Set Flags |            no |

## spget
Pushes the current stack pointer to the stack.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |      spget(0) |
| Result    |          push |
| Set Flags |            no |

## spset
Pops a value into the stack pointer.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |      spset(0) |
| Result    |       discard |
| Set Flags |            no |

## cpget
Pushes the current code pointer incremented by one. This allows pushing return values for function calls.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |      cpget(1) |
| Result    |          push |
| Set Flags |            no |

## add


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(0) |
| Result    |          push |
| Set Flags |            no |

## sub


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(1) |
| Result    |          push |
| Set Flags |            no |

## cmp $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |       math(1) |
| Result    |       discard |
| Set Flags |           yes |

## cmpi


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(1) |
| Result    |       discard |
| Set Flags |           yes |

## mul


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(2) |
| Result    |          push |
| Set Flags |            no |

## div


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(3) |
| Result    |          push |
| Set Flags |            no |

## mod


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(4) |
| Result    |          push |
| Set Flags |            no |

## and


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(5) |
| Result    |          push |
| Set Flags |            no |

## or


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(6) |
| Result    |          push |
| Set Flags |            no |

## xor


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |       pop,pop |
| Command   |       math(7) |
| Result    |          push |
| Set Flags |            no |

## not


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |      pop,zero |
| Command   |       math(8) |
| Result    |          push |
| Set Flags |            no |

## rol $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |       math(9) |
| Result    |          push |
| Set Flags |            no |

## ror $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      math(10) |
| Result    |          push |
| Set Flags |            no |

## asl $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      math(11) |
| Result    |          push |
| Set Flags |            no |

## asr $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      math(12) |
| Result    |          push |
| Set Flags |            no |

## shl $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      math(13) |
| Result    |          push |
| Set Flags |            no |

## shr $arg


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |      math(14) |
| Result    |          push |
| Set Flags |            no |

## syscall


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |    syscall(0) |
| Result    |       discard |
| Set Flags |            no |

## hwio


| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |       hwio(0) |
| Result    |       discard |
| Set Flags |            no |

## int $arg
Raises the interrupt $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |        int(0) |
| Result    |       discard |
| Set Flags |            no |

## sei
Sets the interrupt flag and thus enables interrupts.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |        sei(0) |
| Result    |       discard |
| Set Flags |            no |

## cli
Clears the interrupt flag and thus disables interrupts.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |     zero,zero |
| Command   |        cli(0) |
| Result    |       discard |
| Set Flags |            no |

## in $arg
Reads a word from the port $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 | argument,zero |
| Command   |       hwio(0) |
| Result    |          push |
| Set Flags |            no |

## out $arg
Pops a word and writes it to the port $arg.

| Property  | Configuration |
|-----------|---------------|
| Execution |        always |
| Input 0,1 |  argument,pop |
| Command   |       hwio(1) |
| Result    |       discard |
| Set Flags |            no |

