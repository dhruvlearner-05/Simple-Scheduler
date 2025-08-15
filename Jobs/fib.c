
#include <stdlib.h>
#include <stdio.h>
#include "dummy_main.h"

int fib(int n)
{
  if (n < 2)
    return n;
  else
    return fib(n - 1) + fib(n - 2);
}

int dummy_main(int argc, char **argv)
{
  int val = fib(35);
  printf("fib value = %d \n", val);
  return 0;
}