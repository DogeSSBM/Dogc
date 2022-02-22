#!/bin/sh
printf "\033c"
flags="-Wall -O3"
gcc main.c $flags -o main.out
