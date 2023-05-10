#ifndef DEFICIENT_H
#define DEFICIENT_H
#include <stdbool.h>

/* This function uses a for loop to iterate through the numbers 1 to n-1 and 
checks if each number is a divisor of n. If it is, it adds the divisor to a 
sum. After the loop finishes, the function checks if the sum of the divisors is
less than n. If it is, the function returns true (1), indicating that n is a 
deficient number. If the sum is not less than n, the function returns false 
(0), indicating that n is not a deficient number.*/

bool isDeficientNumber(int n);
#endif
