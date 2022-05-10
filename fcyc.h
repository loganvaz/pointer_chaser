/* Fcyc measures the speed of any "test function."  Such a function
   is passed a list of integer parameters, which it may interpret
   in any way it chooses.
*/

// typedef void (*test_funct2)(int, int);
typedef void (*test_funct2)(unsigned long int *);

/* Compute number of cycles used by function f on given set of parameters */
// double fcyc2(test_funct2 f, int size, int stride);
double fcyc2(test_funct2 f, unsigned long int *mem);