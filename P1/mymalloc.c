#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h" //header file

//memory object  
#define HEAPLENGTH 512
static double heap[HEAPLENGTH]; 

//header stuct 
typedef struct header {
    size_t size; 
    int isAlloc; 
} header;

//mymalloc definition 
void *mymalloc(size_t size, char *file, int line) {
    //alignment
    size = (size % 8 == 0) ? size : (size + 8 - (size % 8));

    if (size == 0) {
        //alloc req is 0 
        printf("MYMALLOC ERROR in %s, line %d: allocation requested is 0\n", file, line); 
        return NULL; 
    }

    if (size > HEAPLENGTH - 1) {
        //alloc req too large error 
        printf("MYMALLOC ERROR in %s, line %d: allocation request exceeds heap size\n", file, line); 
        return NULL; 
    }
    
    //initial pointer @ heap[0]
    header *p = (header*) heap; 

    //iterate + allocate 
    while (p < (header*) &heap[HEAPLENGTH - 1]) {
        if (p->isAlloc == 0) {//@ unallocated index
            //set metadata
            p->size = size + 8; 
            p->isAlloc = 1; 
            header h = {p->size, p->isAlloc}; //create header instance
            *p = h; //store header instance in heap

            printf("MYMALLOC RETURN: chunk size %zu at %ld\n", p->size, (p - (header*)&heap[0])); 
            p = p + 8; 
            return (void*) p; //return payload pointer
        } else {
            //traverse to next opening
            p = p + (p->size); 
        }
    }

    //no free chunk available 
    printf("MYMALLOC ERROR in %s, line %d: insufficient memory\n", file, line);
    return NULL;
}

//myfree definition 
void myfree(void *ptr, char *file, int line) {
    header *p = (header*) ptr; 
    p = p - 8; 
    if (p->size < 16) {
        //ptr is not header (improper size)
        printf("MYFREE ERROR in %s, line %d: argument pointer not located at the start of a chunk\n", file, line); 
        return; 
    }

    if (p < (header*) &heap[0] && p > (header*) &heap[HEAPLENGTH - 1]) {
        //ptr is not mymalloc ptr 
        printf("MYFREE ERROR in %s, line %d: argument pointer not created by MYMALLOC\n", file, line); 
        return; 
    }

    if (p->isAlloc == 0) { 
        //ptr is already freed 
        printf("MYFREE ERROR in %s, line %d: argument pointer has already been freed\n", file, line); 
        return; 
    }

    p->isAlloc = 0; //free chunk 
    printf("MYFREE RETURN: chunk size %zu freed at %ld\n", p->size, (p - (header*)&heap[0])); 
}

int main() {
        void *p = malloc(25); 
        free(p); 
        malloc(50); 
        malloc(500); 
        return 0; 
}
