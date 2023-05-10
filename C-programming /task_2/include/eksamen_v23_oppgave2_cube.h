#ifndef CUBE_H
#define CUBE_H
#include <math.h>
#include <stdbool.h>

/* This function first computes the cubic root of n using the cbrt function 
from the math.h library. It then checks if the cbrt of the cbrt root of n 
is equal to n. If it is, the function returns true (1), otherwise, it returns 
false (0)..*/

bool isCubeNumber(int n);

#endif
