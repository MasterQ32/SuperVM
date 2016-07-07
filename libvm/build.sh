#!/bin/bash

svmas -o $1.bin $1.asm video.asm
svmln -o $1.exp -c $1.bin -d input.dat -E 4096 -d input.ppm -E 1232896
