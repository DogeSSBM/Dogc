#!/bin/sh
printf "\033c"
flags="-Wall -Wextra -O3"
gcc main.c $flags -o main.out
