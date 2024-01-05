#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

//malloc() and immediately free() a 1-byte obejct, 120 times
void test1() {
    for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);  // Allocate 1 byte of memory
        free(ptr);  // Release the memory
    }
    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

//Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
void test2() {
    char *ptrArray[120];  // Array to store 120 pointers

    for(int i = 0; i < 120; i++) {
        ptrArray[i] = malloc(1);  // Allocate 1 byte of memory and store the address
    }

    for(int i = 0; i < 120; i++) {
        free(ptrArray[i]);  // Release the memory
    }

    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}


void test3() {
    char *ptrArray[120];  // Array to store 120 pointers
    int allocated[120] = {0};  // Initialize the memory allocation status array
    int loc = 0;  // Current location

    for(int i = 0; i < 120; i++) {
        if(loc == 0 || (rand() % 2 == 0 && loc < 120)) {
            // Allocate 1 byte of memory and store the address
            //printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
        } else {
            // Release the memory
            loc--;
            //printf("free loc=%d\n", loc);
            free(ptrArray[loc]);
            allocated[loc] = 0;
        }
    }

    //printf("Process is done.\n");

    // Clean up any unreleased memory
    for(int i = 0; i < 120; i++) {
        if(allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }

    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}


void test4() {
    char *ptrArray[120];  // Array to store 120 pointers
    int allocated[120] = {0};  // Initialize the memory allocation status array
    int loc = 0;  // Current location


    for (int i = 0; i < 120; i++) {
        if (loc == 0 || (rand() % 2 == 0 && loc < 120)) {
            // Generate a random size for memory allocation (between 1 and 10 bytes)
            int size = (rand() % 10) + 1;
            
            // Allocate memory of the random size and store the address
            //printf("alloc loc=%d, size=%d\n", loc, size);
            ptrArray[loc] = malloc(size);
            allocated[loc] = 1;
            loc++;
        } else {
            // Release the memory
            loc--;
            //printf("free loc=%d\n", loc);
            free(ptrArray[loc]);
            allocated[loc] = 0;
        }
    }

    // Clean up any unreleased memory
    for (int i = 0; i < 120; i++) {
        if (allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }
    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

void test5(){
    char *ptrArray[120];  // Array to store 120 pointers

    for(int i = 0; i < 120; i++) {
        ptrArray[i] = malloc(1);  // Allocate 1 byte of memory and store the address
    }
    for(int i = 0; i<120; i=i+2){
        free(ptrArray[i]);
    }
    for(int j = 1; j<120; j=j+2){
        free(ptrArray[j]);
    }
    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared

}

long timedTest1(int numRuns){
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    for(int i = 0 ; i<50; i++){
        test1();
    }
    gettimeofday(&tv_end, NULL);
    long diff = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
    return diff;
}

long timedTest2(int numRuns){
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    for(int i = 0 ; i<50; i++){
        test2();
    }
    gettimeofday(&tv_end, NULL);
    long diff = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
    return diff;
}

long timedTest3(int numRuns){
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    for(int i = 0 ; i<50; i++){
        test3();
    }
    gettimeofday(&tv_end, NULL);
    long diff = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
    return diff;
}

long timedTest4(int numRuns){
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    for(int i = 0 ; i<50; i++){
        test4();
    }
    gettimeofday(&tv_end, NULL);
    long diff = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
    return diff;
}

long timedTest5(int numRuns){
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    for(int i = 0 ; i<50; i++){
        test5();
    }
    gettimeofday(&tv_end, NULL);
    long diff = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
    return diff;

}

int main(int argc, char **argv){
    int iterationNum = 50;

    printf("Average time to run test1: %ld\n",timedTest1(iterationNum)/50);
    printf("Average time to run test2: %ld\n",timedTest2(iterationNum)/50);
    printf("Average time to run test3: %ld\n",timedTest3(iterationNum)/50);
    printf("Average time to run test4: %ld\n",timedTest4(iterationNum)/50);
    printf("Average time to run test5: %ld\n",timedTest5(iterationNum)/50);

    return EXIT_SUCCESS;
}