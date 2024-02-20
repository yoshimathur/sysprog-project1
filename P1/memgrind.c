#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// Include the header file for mymalloc
#include "mymalloc.h"

// Define constants for testing
#define ITERATIONS 50
#define OBJECTS 120
#define BYTE_SIZE 1

int main() {
    struct timeval start, end;
    double time_taken;

    // Task 1: malloc() and free() a 1-byte object, 120 times
    gettimeofday(&start, NULL);
    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < OBJECTS; j++) {
            char *ptr = (char *)malloc(BYTE_SIZE);
            free(ptr);
        }
    }
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    printf("Task 1 took %f microseconds on average\n", time_taken / ITERATIONS);

    // Task 2: Allocate and deallocate 120 1-byte objects
    gettimeofday(&start, NULL);
    for (int i = 0; i < ITERATIONS; i++) {
        char *ptrs[OBJECTS];
        for (int j = 0; j < OBJECTS; j++) {
            ptrs[j] = (char *)malloc(BYTE_SIZE);
        }
        for (int j = 0; j < OBJECTS; j++) {
            free(ptrs[j]);
        }
    }
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    printf("Task 2 took %f microseconds on average\n", time_taken / ITERATIONS);

    // Task 3: Randomly allocate and deallocate 1-byte objects
    gettimeofday(&start, NULL);
    for (int i = 0; i < ITERATIONS; i++) {
        char *ptrs[OBJECTS];
        int allocated = 0;
        for (int j = 0; j < OBJECTS; j++) {
            if (rand() % 2 == 0 && allocated < OBJECTS) {
                ptrs[allocated++] = (char *)malloc(BYTE_SIZE);
            } else if (allocated > 0) {
                int index = rand() % allocated;
                free(ptrs[index]);
                ptrs[index] = ptrs[--allocated];
            }
        }
        for (int j = 0; j < allocated; j++) {
            free(ptrs[j]);
        }
    }
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    printf("Task 3 took %f microseconds on average\n", time_taken / ITERATIONS);

    // Task 4: Add your own stress tests here

    return 0;
}

