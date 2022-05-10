// fcyc.c - Run the test function and estimate the ``best'' runtime
// (in clock cycles).

/*
  This file defines the function fcyc2, which runs a test function
  (supplied in mem-chase.c) enough times to estimate the best-case
  runtime. If we only ran it once, we'd be more likely to see a
  slowdown due to the initial state of the cache; we run the function
  repeatedly, keeping track of the K best runs as we go (keeping them
  sorted in order of runtime), and when the top K are close enough
  (i.e. the #1/fastest runtime is CLOSE ENOUGH to the #K runtime), we
  have ``converged,'' and the fastest runtime is returned.
*/

#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>

#include "clock.h"
#include "fcyc.h"

#define K 5                 // We record the top K runs; higher K
                            // means the algorithm takes longer to
                            // converge, but we are more likely to get
                            // a better estimate

#define MAXSAMPLES 20       // Maximum # of times we run the test
                            // function and record its runtime

#define EPSILON 0.005        // Convergence epsilon (see algorithm)

// We use static variables for each of the above #defines; this allows
// us to use the #defines as defaults and allow the caller to change
// them if (s)he wishes.
static int kbest = K;
static int maxsamples = MAXSAMPLES;
static double epsilon = EPSILON;

static double *values = NULL; // array of sample values, sorted in
                              // increasing order
static int samplecount = 0;   // number of samples in values array
                              // (thus far)

// Allocate space for the values array, which will hold the K best
// cycle counts from the test runs.
static void init_sampler(void)
{
    if (values) free(values);

    // Allocate values array & init to 0
    values = calloc(kbest, sizeof(double));

    samplecount = 0; /* Initialize samplecount */
}

// Add new sample to sorted list of values. If val > values[kbest],
// the value gets ignored.
static void add_sample(const double val)
{
    int pos = 0;
    if (samplecount < kbest) {
        pos = samplecount;
        values[pos] = val;
    } else if (val < values[kbest-1]) {
        pos = kbest-1;
        values[pos] = val;
    }

    samplecount++;
    while (pos > 0 && values[pos-1] > values[pos]) {
        double temp = values[pos-1];
        values[pos-1] = values[pos];
        values[pos] = temp;
        pos--;
    }
}

static int has_converged(void)

// This function determines whether the kbest measurements have
// converged within epsilon.  We compare the difference between our
// worst (biggest) measurement with our best (smallest) - if the
// relative error is less than epsilon, we have converged.
{
    return
        (samplecount >= kbest) && /* don't converge unless we've taken
                                     at least kbest samples */
        ((1 + epsilon)*values[0] >= values[kbest-1]);
}

// double fcyc2(test_funct2 f, int arg1, int arg2)
double fcyc2(test_funct2 f, unsigned long int *arg)

// Determine the best possible performance we can obtain with test
// function f by taking multiple samples, keeping kbest samples with
// least value, and halting when all the kbest samples are within a
// certain relative error epsilon.

{
    double result;
    init_sampler();

    do {
        double cyc;
        start_counter();
        // f(arg1, arg2);
        f(arg);
        cyc = get_counter();
        add_sample(cyc);
    } while (!has_converged() && samplecount < maxsamples);

    result = values[0];
    return result;
}
