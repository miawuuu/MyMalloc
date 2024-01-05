CS 214 Fall 2023: Project I: My little malloc() 
Mia Wu: cw984
Sri Akshara Kollu: sk2342

Objective:
To implement your own versions of the standard library functions malloc() and free() and detect common usage errors and report them

MetaData: 
The structure of our metadata contains our header and the payload. The header consists of 8 bytes: the first 4 bytes have the payload size (chunkSize), and the last four contain whether the payload is being used. The chunkSize also includes the metadata in our implementation. 

Detectable Errors: 
In our malloc implementation, our code will be able to detect when the user attempts to malloc 0 bytes and will print out Error: Unable to malloc 0 bytes and then halt the program using exit(). Our code will also be able to notice when the user attempts to add memory that cannot be contained in 4096 bytes and will then print Error: not enough memory and halt the program using exit(). 

In our free implementation, our code will be able to detect when the user attempts to double free or call free() a second time on the same pointer and will print out Error: Attempted double free and then halt the program using exit(). Our code will also be able to notice when the user calls free()with an address not obtained from malloc() and when calling free()with an address not at the start of a chunk and will print Error: Invalid pointer and halt the program using exit(). 

We decided to print out an error message and stop the program using exit instead of doing nothing and returning. The benefits of printing outweigh the time disadvantage. Printing out why the user got the error would help debug the error, making it easier for the user to find the issue, and halting the program would stop the code from making potentially incorrect results. 

Test Plan: 
After creating mymalloc() and myfree(), we tested the methods using the following checks in the check.c file:
The method check1() checks to make sure that we are properly using mymalloc() and myfree()
The method check2() checks that we can malloc two chunks and remove the second chunk first. This is used to make sure that the chunks are properly coalescing. 
The method check3() checks that we can malloc two chunks and remove the first chunk. This is also used to make sure that the chunks are properly coalescing. 

After creating mymalloc() and myfree(), we decided to test the methods to print the errors and halt the program when necessary using the file error.c:
The method error1() checks to make sure that we are printing and halting when attempting to malloc 0 bytes
The method error2() checks to make sure that we are printing and halting when there is not enough memory
The method error3() checks to make sure that we are printing and halting when attempting to double-free
The method error4() checks to make sure that we are printing and halting when attempting to free with an address not obtained from malloc
The method error5() checks to make sure that we are printing and halting when attempting to  free with an address, not at the start of a chunk 

We also use the file memtest.c to ensure that mymalloc() and myfree() work for the scenario where you allocate 64 objects using all available memory. The program when using the number of objects is a factor of  4096, prints out 0 incorrect bytes and will print out incorrect bytes otherwise. 

Descriptions of Test Program: 
In our file memgrind.c, we are running stress tests to determine the efficiency of mymalloc() and myfree(). 
In test1, the program allocates 1 byte of memory and immediately releases it 120 times to evaluate the performance when memory is allocated and immediately released. 
In test2, the program allocates 1 byte of memory at 120 consecutive locations and then releases each allocation to test the performance with consecutive memory in simple allocations and deallocations.
In test3, the program randomly decides whether to allocate 1 byte of memory or release an allocated memory and repeats 120 times to evaluate the the performance under more complex memory allocation and deallocation scenarios.
In test4, the program generates random sizes for memory allocation using (rand() % 10) + 1, which will allocate memory chunks of sizes between 1 and 10 bytes to test the performance of more complex memory allocation and deallocation scenarios. 
In test5, the program allocates objects of 1 byte until the memory is full and then free every other object. This test is used to evaluate the performance of more complex memory allocation and deallocation scenarios. 

Average Time for Each Test:
Test 1: 10 microseconds
Test 2: 210 microseconds

Test 3: 101 microseconds
Test 4: 102 microseconds
Test 5: 325 microseconds * this test takes more time due to the amount of coalescing the chunks