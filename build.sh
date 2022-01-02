#! /bin/bash

rm -f err.log 

(
 gcc -o ./build/ustellara \
   ./src/libreceivemode.c \
   ./src/libdigit.c \
   ./src/frontend.c \
   ./src/main.c \
   -lncurses
) 2>err.log

vim err.log

exit 0
