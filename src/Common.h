#ifndef COMMON_H
#define COMMON_H

// #define DEBUG
#ifdef DEBUG
#define ASSERT( x ) assert(x)
#else
#define ASSERT( x )  
#endif

#include "types.h"


#endif