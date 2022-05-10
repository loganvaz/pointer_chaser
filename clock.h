/* Routines for using cycle counter */

/* Start the counter */
void start_counter();

/* Get # cycles since counter started */
double get_counter();

/* Determine clock rate of processor, having more control over accuracy */
double mhz(int verbose, int sleeptime);

