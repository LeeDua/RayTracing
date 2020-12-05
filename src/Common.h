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
#include <cstdlib>

#define PI 3.1415926535897932385
#define MAX_RECURSIVE_DEPTH 50
#define SAMPLE_PER_PIXEL 200


inline dtype degree_to_radius(dtype degree){
    return degree * PI / 180.0;
}

inline dtype rand_double(dtype min=0.0, dtype max=1.0){
    return min + (max-min)* rand() / (RAND_MAX + 1.0);
}


#endif