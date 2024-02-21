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

    //temps for coalescing
    header *temp_p; 
    int coalescing = 0; 

    //iterate + allocate 
    while (p < (header*) &heap[HEAPLENGTH - 1]) {
        printf("%p\n", p); 
        if (p->isAlloc == 1) {
            //chunk is occupied
            coalescing = 0; 
            p = p + (p->size); 

        } else if (p->size == 0) { //initializable chunk 
            size_t fsize; 

            //check for coalescing for proper size check
            if (coalescing > 0) { 
                fsize = size - temp_p->size + 8;
            } else {
                fsize = size + 8; 
            }

            header *fp = p + fsize; 

            //ensure sufficient storage
            if (fp > (header*) &heap[HEAPLENGTH - 1]) {
                printf("MYMALLOC ERROR in %s, line %d: insufficient memory\n", file, line);
                return NULL; 
            }

            header h = {fsize, 1}; //create header instance

            //check for coalescing to properly store
            if (coalescing > 0) { 
                *temp_p = h; 

                temp_p = temp_p + 8; //set pointer to payload
                printf("MYMALLOC RETURN: chunk size %zu at %p\n", h.size, temp_p); 
                return (void*) temp_p;
            } else {
                *p = h; //store header instance in heap

                p = p + 8; //set pointer to payload
                printf("MYMALLOC RETURN: chunk size %zu at %p\n", h.size, p); 
                return (void*) p;
            }

        } else { //chunk was freed --> size check & avoid overlapping memory
            if (p->size >= size + 8) { 
                //adequate chunk found 
                header h = {(size + 8), 1}; 
                *p = h;  

                p = p + 8; 
                printf("MYMALLOC RETURN: chunk size %zu at %p\n", h.size, p); 
                return (void*) p;
            } else {
                //need to find additional storage 
                if (coalescing > 0) {
                    //joining chunks 
                    if (temp_p->size + p->size >= size + 8) {
                        //adequate joined chunk found 
                        header h = {(size + 8), 1}; 
                        *temp_p = h; 

                        temp_p = temp_p + 8; //set pointer to payload
                        printf("MYMALLOC RETURN: chunk size %zu at %p\n", h.size, temp_p); 
                        return (void*) temp_p;
                    } else {
                        temp_p->size = temp_p->size + p->size; 
                        p = p + p->size; 
                    }
                } else {
                    temp_p = p; 
                    p = p + p->size; 
                    coalescing++; 
                }

            }
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

    if (p->isAlloc == 2) { 
        //ptr is already freed 
        printf("MYFREE ERROR in %s, line %d: argument pointer has already been freed\n", file, line); 
        return; 
    }

    p->isAlloc = 2; //free chunk 
    printf("MYFREE RETURN: chunk size %zu freed at %p\n", p->size, p); 
}

int main() {
        header *hp = (header*) &heap[0]; 
        void *p = malloc(25); 
        malloc(50); 
        void *p2 = malloc(50); 
        free(p);  
        malloc(100); 
        free(p2); 
        return 0; 
}
