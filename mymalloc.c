/*CS 214 Fall 2023: Project I: My little malloc() 
Mia Wu: cw984
Sri Akshara Kollu: sk2342*/
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 512
static double memory[MEMLENGTH];
char *heapstart = (char *)memory;
#define ROUNDUP8(x) (((x) + 7) & (-8))


/* Structure:
MetaData 8 bytes:
int chunkSize: 4 bytes, stores the size of the chunk including the metadata. 
int isMalloced: 4 bytes, a boolean that stores 1 if it is malloced (or not free) and 0 if it is
not malloced (or free)*/
unsigned int isMalloced(char* ptr){
    return *(int*)(ptr+4);
}

unsigned int getChunkSize(char* ptr){
    return *(int*)ptr;
}

void setChunkSize(char* ptr, int size){
    *(int*)ptr = size;
}

void markedAsMalloced(char* ptr){
    *(int*)(ptr+4) = 1;
}

char* getNextChunk(char* ptr){
    return ptr+(int)getChunkSize(ptr);
}

unsigned int nextChunkUninitailized(char* ptr){
    char* next = getNextChunk(ptr);

    if(isMalloced(next)==0 && getChunkSize(next)==0){
        return 1;
    }
    else{
        return 0;
    }
}

void setNextChunkSize(char* ptr, int size){
    //char* next = getNextChunk(ptr);
    setChunkSize(getNextChunk(ptr),size);
}

//free helpers
static char* getMetadataStart(char* ptr) {
    return ptr - 8;
}

static int isAdjacentAndFree(char* block1, char* block2) {
    // Calculate the end address of block1
    char* endOfBlock1 = block1 + getChunkSize(block1);
    char* medataBlock2 = getMetadataStart(block2);

    // Check if the end of block1 is exactly at the beginning of block2
    if (endOfBlock1 == medataBlock2) {
        // Check if both blocks are marked as free
        if (isMalloced(block1) == 0 && isMalloced(medataBlock2) == 0) {
            return 1;
        }
    }
    return 0;
}
static char* nextBlock(char* block) {
    // return the start of the payload of the next chunk
    return block+(int)getChunkSize(getMetadataStart(block));
}

static void markAsFree(char* ptr) {
    // Mark the block as free by updating the metadata
    char* metadata = getMetadataStart(ptr);
    *(int*)(metadata+4) = 0;
}

static void invalidatePointer(void* ptr) {
    // Invalidate the pointer by setting it to NULL
    ptr = NULL;
}

static void mergeBlocks(char* block1, char* block2){
    int size1 = getChunkSize(block1);
    int size2 = getChunkSize(getMetadataStart(block2));
    setChunkSize(block1, size1+size2);
}

int memCleared() {
    char *start = heapstart;
    int chunkSize = getChunkSize(start);
    int free = isMalloced(start);

    // Case: Memory pool is uninitialized or fully cleared
    if ((chunkSize == 0 && free == 0) || (chunkSize == 8 * MEMLENGTH && free == 0)) {
        return 1;
    }

    return 0;
}

void printHeap(){
    char* ptr = heapstart;
    printf("Heapstart - ");
    int i = 0;
    while(i<4096){
        if(ptr == NULL){
            return;
        }
        int chunkSize = getChunkSize(ptr);
        int free = isMalloced(ptr);
        printf("size: %d, used:%d |", chunkSize, free);
        if(i + chunkSize<4096){
            ptr = getNextChunk(ptr);
        }
        i=i+getChunkSize(ptr);
    }
    printf("\n");
}

void printMem(char* ptr){
    int size = getChunkSize(ptr);
    int free = isMalloced(ptr);
    printf("Size: %d, Malloced: %d| ", size, free);
}

void* mymalloc(size_t size, char *file, int line){
    if(size == 0){
        //error
        fprintf(stderr, "Error: Unable to malloc 0 bytes %s at line %d.\n", file, line);
        exit(EXIT_FAILURE);
    }
    int roundedSize = ROUNDUP8(size);
    if(roundedSize+8 > 4096){
        fprintf(stderr, "Error: Not enough memory in %s at line %d.\n", file, line);
        exit(EXIT_FAILURE);
    }
    
    char* res;
    char* start = heapstart;
    int i = 0;

    while (i<4096){
        int chunkSize = getChunkSize(start);
        int isUsed = isMalloced(start);
        
        if(chunkSize == 0 && isUsed == 0){
            setChunkSize(start,roundedSize+8);
            markedAsMalloced(start);
            chunkSize = getChunkSize(start);
            res = start+8;
            isUsed = 1;
            if(i + chunkSize < 4096){
                setNextChunkSize(start, 4096-(i+chunkSize));
            }
            //printHeap();
            //printMem(start);
            return res;
        }else if(isUsed == 0 && chunkSize>=roundedSize+8){
            markedAsMalloced(start);
            setChunkSize(start, roundedSize+8);
            res = start + 8;
            if(i + getChunkSize(start)<4096){
                if(nextChunkUninitailized(start)){
                    setNextChunkSize(start, chunkSize-(roundedSize+8));
                    //printHeap();
                    //printMem(start);
                    return res;
                }
            }
            //printMem(start);
            return res;
            
        }else if(isUsed == 1 || chunkSize<roundedSize+8){
            int size = getChunkSize(start);
            if(i + size<4096){
                start = getNextChunk(start);
            }
            i = i + size;
        }
    }
    fprintf(stderr, "Error: Not enough memory in %s at line %d.\n", file, line);
    exit(EXIT_FAILURE);
    //printHeap();
    return NULL;
}

/*called with a single pointer, which must be a pointer to a chunk created by malloc().
free() will mark the chunk free, making it available for subsequent calls to malloc().*/
void myfree(void *ptr, char *file, int line){

    /*check to see whether the pointer passed in by the user is valid and pointing to the
    begining of the payload*/
    int j = 0;
    int isValid = 0;
    char* s = heapstart;
    while (j<4096){
        if(getMetadataStart(ptr)==s){
            isValid = 1;
        }
        int size = getChunkSize(s);
        if(size == 0){
            fprintf(stderr, "Error: Invalid pointer in %s at line %d.\n", file, line);
            exit(EXIT_FAILURE);
        }
        if(j + size<4096){
            s = getNextChunk(s);
        }
        j = j + size;
    }
    if(!isValid){
        fprintf(stderr, "Error: Invalid pointer in %s at line %d.\n", file, line);
        exit(EXIT_FAILURE);
    }

    //if pointer is valid, continue
    char *start = heapstart;
    int i = 0;
    if(!isMalloced(getMetadataStart(ptr))){
        fprintf(stderr, "Error: Attempted double free in %s at line %d.\n", file, line);
        exit(EXIT_FAILURE);
    }
    markAsFree(ptr);

    while (i < 4096) {
        int startSize = getChunkSize(start);
        char* ptrMetadata = getMetadataStart(ptr);
        int ptrSize = getChunkSize(ptrMetadata);

        //if the chunk before ptr is free
        if (isAdjacentAndFree(start, ptr)) {
            mergeBlocks(start, ptr);
            if((i+startSize+ptrSize)<4096){
                if(!isMalloced(getNextChunk(ptrMetadata))){
                    mergeBlocks(start, nextBlock(ptr));
                }
            }
            invalidatePointer(ptr);
            //printHeap();
            return;
        }

        //if the current chunk is where ptr is pointing
        if (start == getMetadataStart(ptr)) {
            if((i+startSize+ptrSize)<4096){
                if(!isMalloced(getNextChunk(ptrMetadata))){
                    mergeBlocks(start, nextBlock(ptr));
                }
            }
            invalidatePointer(ptr);
            //printHeap();
            return;
        }
        
        i = i+getChunkSize(start);
        if(i<4096){
            start = getNextChunk(start);
        }
    }

    fprintf(stderr, "Error: Pointer not in heap in %s at line %d.\n", file, line);
    exit(EXIT_FAILURE);
    //printHeap();
}

