# DasOS Virtual Devices

> darklink:
> HDD: 1GB
> GPU: Kann spriting, tiling und palletierung.
> CPU: Hat ne FPU
> FPU: 32 und 64-bit floats; letztere können nicht im stack
>      gespeichert werden, sondern nur im RAM

## FPU

Floating Point Unit that has its
own stack and set of instructions.
Each stack entry has 64 bit and contains
an IEEE floating point number.
pushf:
	Push 32bit float to floating stack
popf:
	Pop 32bit float from floating stack
addf, subf, divf, mulf, modf:
	Basic arithmetic operation, pops two, pushes result