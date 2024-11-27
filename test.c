#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_test(int test_num);

int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        int test_num = atoi(argv[2]);
        run_test(test_num);
    } else {
        for (int i = 1; i <= 5; i++) {
            run_test(i);
        }
    }
    return 0;
}

void run_test(int test_num) {
    memory_init(1024);

    switch (test_num) {
        case 1: {
            printf("Test 1: Simple Allocation\n");
            void* ptr1 = memory_malloc(128);
            if (ptr1) {
                printf("Test 1 Passed\n");
            } else {
                printf("Test 1 Failed\n");
            }
            memory_free(ptr1);
            break;
        }
        case 2: {
            printf("Test 2: Boundary Allocation\n");
            void* ptr = memory_malloc(896);
            if (ptr) {
                printf("Test 2 Passed\n");
            } else {
                printf("Test 2 Failed\n");
            }
            memory_free(ptr);
            break;
        }
        case 3: {
            printf("Test 3: Fragmentation and Coalescing\n");
            void* ptr1 = memory_malloc(128);
            void* ptr2 = memory_malloc(128);
            memory_free(ptr1);
            void* ptr3 = memory_malloc(128);
            if (ptr3) {
                printf("Test 3 Allocation after Free Passed\n");
            } else {
                printf("Test 3 Allocation after Free Failed\n");
            }
            memory_free(ptr2);
            memory_free(ptr3);
            
            void* ptr4 = memory_malloc(256);  // Should be able to allocate a larger block now
            if (ptr4) {
                printf("Test 3 Coalescing Passed\n");
            } else {
                printf("Test 3 Coalescing Failed\n");
            }
            memory_free(ptr4);
            break;
        }
        case 4: {
            printf("Test 4: Reallocation\n");
            void* ptr = memory_malloc(128);
            ptr = memory_realloc(ptr, 256);
            if (ptr) {
                printf("Test 4 Passed\n");
            } else {
                printf("Test 4 Failed\n");
            }
            memory_free(ptr);
            break;
        }
        case 5: {
            printf("Test 5: Stress Test with Repeated Allocations\n");
            int success = 1;
            for (int i = 0; i < 50; i++) {
                void* ptr = memory_malloc(16);
                if (!ptr) {
                    printf("Test 5 Failed at iteration %d\n", i);
                    success = 0;
                    break;
                }
                memory_free(ptr);
            }
            if (success) {
                printf("Test 5 Passed\n");
            }
            break;
        }
        case 6: {
            printf("Test 6: Memory Overrun\n");
            void* ptr = memory_malloc(2048);
            if (ptr == NULL) {
                printf("Test 6 Passed: Properly handled memory overrun\n");
            } else {
                printf("Test 6 Failed: Memory overrun not properly handled\n");
                memory_free(ptr);
            }
            break;
        }
        case 7: {
            printf("Test 7: Memory Alignment\n");
            void* ptr = memory_malloc(100);
            if ((uintptr_t)ptr % 8 == 0) {
                printf("Test 7 Passed: Memory is aligned\n");
            } else {
                printf("Test 7 Failed: Memory is not properly aligned\n");
            }
            memory_free(ptr);
            break;
        }
        default:
            printf("Unknown test number: %d\n", test_num);
            break;
    }

    memory_deinit();
}
