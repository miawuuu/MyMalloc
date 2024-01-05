#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

void check1(){
  char* ptr = malloc(1);
  free(ptr);
  printf("Check1 MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void check2(){
  char* ptr0 = malloc(1);
  char* ptr1 = malloc(1);
  free(ptr1);
  free(ptr0);
  printf("Check2 MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void check3(){
  char* ptr0 = malloc(1);
  char* ptr1 = malloc(1);
  free(ptr0);
  free(ptr1);
  printf("Check3 MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

int main(int argc, char **argv)
{
  check1();
  check2();
  check3();
  return EXIT_SUCCESS;
}
