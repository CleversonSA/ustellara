#! /bin/bash

rm -f err.log 

(

gcc ./src/libdigit.c -o ./build/libdigit.so -lncurses
gcc ./src/main.c -o ./build/ustellara

) 2>err.log

vim err.log

exit 0
