/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n < 1)
    {
        return false;
    }
    /* binary search algorithm ...
      ...assumes values are sorted low to high ... left to right ... */
    int test, index, left = 0, right = n;
    
    do
    {
        index = (right - left) / 2;
        test = index + left;
        if (value == values[test])
        {
            return true;
        }
        else if (values[test] > value)
        {
            right = test;
        }
        else if (values[test] < value)
        {
            left = test + 1;
        }
    }
    while (right > left);
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    const int MAX = 65536;
    // count sort is an O(n) sorting algorithm
    
    int count[MAX];
    int i, val, biggest = 0;
    
    for (i = 0; i < n; i++)
    {
        val = values[i];
        count[val] += 1;
        if (val > biggest)
        {
            biggest = val;
        }
    }
    
    for (i = 0, val = 0; i <= biggest; i++)
    {
        if (count[i] >= 1)
        { 
            values[val] = i;
            val++;
        }
    }
    
    return;
}
