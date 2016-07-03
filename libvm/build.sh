#!/bin/bash

svmas -o test.bin test.asm video.asm
svmln -o test.exp -c test.bin -d input.dat -E 4096 -d input.ppm -E 1232896
