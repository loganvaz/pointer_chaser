//  mem-chase.c

// This file contains a program meant to challenge students to make a
// certain "pointer-chasing" program run both as quickly as possible
// and as slowly as possible.  Students will not be allowed to change
// the program; they only get to change the initialization of the
// array where the "pointer-chasing" values are stored.

// We begin with a very small example.

//    Fast Access              Slower Access

//     +---------+               +---------+
//  0: |    1    |            0: |    3    |
//     +---------+               +---------+
//  1: |    2    |            1: |    2    |
//     +---------+               +---------+
//  2: |    3    |            2: |    0    |
//     +---------+               +---------+
//  3: |    0    |            3: |    1    |
//     +---------+               +---------+

// The look-ahead mechanisms for a cache fill, especially when a
// processor is incrementally moving through memory addresses work
// quite well.  But, in our slower access initialization, we attempt
// to order the contents of the array to cause cache misses of all
// types (e.g., L1, L2, L3, L4, TLB, TLB backing cache, incremental
// page translation caches, the small on-DRAM caches, etc.).

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "fcyc.h"  // K-best measurement timing routines
#include "clock.h" // routines to access the cycle counter

#define MIN_EXPONENT_SIZE  (9)
#define MAX_EXPONENT_SIZE  (27)
#define MAX_ENTRIES        (1 << MAX_EXPONENT_SIZE)

#define PAGE_SIZE          (4096)
#define EXTRA              (PAGE_SIZE)


unsigned long int data[MAX_ENTRIES + EXTRA];  /* The array we'll be traversing */
unsigned long int *mem;

extern void init_data_fast(unsigned long int *, unsigned long int);
extern void init_data_slow(unsigned long int *, unsigned long int);
void chase_indices(unsigned long int *mem);

// External function references
int main(int argc, char *argv[], char *env[])
{
    int size = 1, slow = 0, arg_exp;

    if (argc >= 2) {
        arg_exp = atoi(argv[1]);
        if (arg_exp < MIN_EXPONENT_SIZE || arg_exp > MAX_EXPONENT_SIZE)
          {
            printf("Memory size is (2^exponent) where %d <= exponent <= %d.\n",
                   MIN_EXPONENT_SIZE, MAX_EXPONENT_SIZE );
            exit(1);
            }
        else
          size = 1 << arg_exp;
    }
    if (argc >= 3) {
        slow = atoi(argv[2]);
    }

    // Set pointer mem to the beginning of a (4K-address-aligned) page.
    mem = (unsigned long int *)
      ((uintptr_t) data + PAGE_SIZE - ((uintptr_t) data % PAGE_SIZE));

    // Initialize data array
    if (slow)
        init_data_slow(mem, size);
    else
        init_data_fast(mem, size);

    double d = fcyc2(chase_indices, mem);
    printf("size = 2^%d = %d, slow = %d, time = %g\n", arg_exp, size, slow, d);

    exit(((int) d) % 256);
}

void chase_indices(unsigned long int *mem)
{
    // Chase your pointers.
    unsigned long int val = 0;
    unsigned long int iters = 0;
    // unsigned long int idx, idxv;
    do {
      do {
        iters++;
        // 0
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 4
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 8
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 12
        val = mem[val];
        val = mem[val];
        val = mem[val];
        //W idx = val;
        val = mem[val];
        // 16
        // Set memory value to 0
        //W idxv = val;
        //W mem[ idx ] = 0;
        // 16
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 20
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 24
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 28
        val = mem[val];
        val = mem[val];
        val = mem[val];
        val = mem[val];
        // 32
        // Restore memory value
        //W mem[ idx ] = idxv;
      }
      while (val);
    }
    while (iters < 4000000);

    printf("Iters: %ld, Val: %ld\n", iters, val);
}
