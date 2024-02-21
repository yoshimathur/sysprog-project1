#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

#define ITERATIONS 50
#define OBJECTS 120

void task1() {
    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < OBJECTS; j++) {
            void *ptr = malloc(1);
            free(ptr);
        }
    }
}

void task2() {
    for (int i = 0; i < ITERATIONS; i++) {
        void *ptrs[OBJECTS];
        for (int j = 0; j < OBJECTS; j++) {
            ptrs[j] = malloc(1);
        }
        for (int j = 0; j < OBJECTS; j++) {
            free(ptrs[j]);
        }
    }
}

void task3() {
    for (int i = 0; i < ITERATIONS; i++) {
        void *ptrs[OBJECTS];
        int count = 0;
        while (count < OBJECTS) {
            int choice = rand() % 2;
            if (choice == 0) {
                ptrs[count] = malloc(1);
                count++;
            } else if (choice == 1 && count > 0) {
                free(ptrs[count - 1]);
            }
        }
        while (count > 0) {
            free(ptrs[count - 1]);
            count--;
        }
    }
}

void task4() {
    // Stress Test 1: simulate a scenario where the program alternates between allocating 
    // and deallocating large chunks of memory repeatedly. This will test the resilience 
    // and efficiency of the memory allocation system under rapid and varying workload conditions.
    
    for (int i = 0; i < ITERATIONS; i++) {
        // Allocate large chunks of memory
        void *largeChunk1 = malloc(4096);
        void *largeChunk2 = malloc(2048);
        void *largeChunk3 = malloc(1024);

        // Freeing the chunks in a different order than they were allocated
        free(largeChunk2);
        free(largeChunk1);
        free(largeChunk3);

        // Allocate smaller chunks
        void *smallChunk1 = malloc(128);
        void *smallChunk2 = malloc(256);
        void *smallChunk3 = malloc(512);

        // Free the smaller chunks in reverse order of allocation
        free(smallChunk3);
        free(smallChunk2);
        free(smallChunk1);
    }

    // Stress Test 2: This stress test will create a situation where the program rapidly allocates and deallocates
    // a mix of small and large memory chunks randomly. It will help evaluate how well the memory allocation 
    // system handles varying allocation patterns and sizes.
    for (int i = 0; i < ITERATIONS; i++) {
        // Allocate small chunks randomly
        for (int j = 0; j < 10; j++) {
            if (rand() % 2 == 0) {
                void *smallChunk = malloc(64);
                free(smallChunk);
            }
        }

        // Allocate a large chunk
        void *largeChunk = malloc(1024);

        // Free small and large chunks randomly
        for (int k = 0; k < 10; k++) {
            int choice = rand() % 2;
            if (choice == 0) {
                free(largeChunk);
            } else {
                void *smallChunk = malloc(128);
                free(smallChunk);
            }
        }
    }
    

}
    
    




int main() {
    srand(time(NULL)); // Seed the random number generator

    // Task 1
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    task1();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Task 1: Average time taken = %f seconds\n", cpu_time_used / ITERATIONS);

    // Task 2
    start = clock();
    task2();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Task 2: Average time taken = %f seconds\n", cpu_time_used / ITERATIONS);

    // Task 3
    start = clock();
    task3();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Task 3: Average time taken = %f seconds\n", cpu_time_used / ITERATIONS);

    // Task 4
    start = clock();
    task4();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Task 4: Average time taken = %f seconds\n", cpu_time_used / ITERATIONS);

    return 0;
}
