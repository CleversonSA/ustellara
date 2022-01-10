#! /bin/bash

rm -f err.log 

(
  make
  make clean
) 2>err.log

vim err.log

exit 0
