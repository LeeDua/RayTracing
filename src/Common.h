#ifndef COMMON_H
#define COMMON_H

// #define DEBUG
#ifdef DEBUG
#define ASSERT( x ) assert(x)
#else
#define ASSERT( x )  
#endif

#include "types.h"
#include <limits>

#define PI 3.1415926

inline dtype degree_to_radius(dtype degree){
    return degree * PI / 180.0;
}


#endif