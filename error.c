#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

void error1(){
  malloc(0);
}

void error2(){
  malloc(4096);
}

void error3(){
  char* ptr0 = malloc(1);
  free(ptr0);
  free(ptr0);
}

void error4(){
  char* ptr0 = malloc(1);
  free(ptr0+1);
}

void error5(){
  int x;
  free(&x);
}

int main(int argc, char **argv)
{
  error1();
  //error2();
  //error3();
  //error4();
  //error5();
  return EXIT_SUCCESS;
}
