#include <stdio.h>
#include "dummy_main.h"

int dummy_main(int argc, char **argv) {
  int sum = 0;
  for (int i = 1; i <= 10000; i++) {
    sum += i;
  }

  printf("The sum of the first 10000 numbers is: %d\n", sum);

  return 0;
}
