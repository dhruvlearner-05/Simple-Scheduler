#!/bin/bash

gcc -w Codes/Simple_Scheduler.c Codes/Queue.c -o Out/Simple_Scheduler
gcc -w Codes/Simple_Shell.c -o Out/Simple_Shell
gcc -w Jobs/fib.c -o Out/fib
gcc -w Jobs/fib2.c -o Out/fib2
gcc -w Jobs/hello.c -o Out/hello
gcc -w Jobs/sum.c -o Out/sum

echo "Compilation complete!"

