/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    /* the following conditional statement is designed to instruct the user how to implement the program... 
        it both requests and requires the user to enter either one or two additional 
        command line arguments... without which the program ends and generates an error code and the message... */
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    /* the first additional command line argument is converted to type int ... (integer value)... 
        and is assigned to the variable n  ... note: command line args are originally received as string values...   */
    int n = atoi(argv[1]);

    /* if the second optional command line argument is entered by the user ...
        it is converted to type long ...(long integer value )... 
        and becomes the argument to a function call that places a "seed" value into a buffer for the random number generator to access... 
        ...otherwise...the time function (from #include <time.h>) is used to produce the argument... */
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    /*  the user's first command line input (n) is used to provide a quantity of pseudorandom values...
         which are created by a pseudorandom number generator function (from #include <stdlib.h>) ...
         the default output values of this function (positive double-precision floats) are converted to type int... 
         their range limited by the #define value listed in the header  ...and displayed...  */
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
