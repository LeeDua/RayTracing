#ifndef COMMON_H
#define COMMON_H

#include<assert.h>

// #define DEBUG
#ifdef DEBUG
#define ASSERT( x ) assert(x)
#define DEBUG_PRINT( x ) std::cout << x << std::endl
static int node_count = 0;
static int leaf_count = 0;
static int box_count = 0;
static int sphere_count = 0;
static int aabb_hit_count = 0;
static int rect_hit_count = 0;
static int light_hit_count = 0;
#else
#define ASSERT( x )  
#define DEBUG_PRINT( x )
#endif

// #define COLORED_BG



#include "types.h"
#include <limits>
#include <cstdlib>
#include <memory>
#include <utility>



#define PI 3.1415926535897932385
#define MAX_RECURSIVE_DEPTH 50
#define SAMPLE_PER_PIXEL 5
#define IMG_WIDTH 400
// #define ASPECT_RATIO (3.0 / 2.0)
// #define ASPECT_RATIO (16.0 / 9.0)
#define ASPECT_RATIO 1.0
#define EPSILON_MORE 1e-6
#define EPSILON 1e-8
#define MIN_HIT_DIST 1e-4


inline dtype degree_to_radius(dtype degree){
    return degree * PI / 180.0;
}

inline dtype rand_double(dtype min=0.0, dtype max=1.0){
    return min + (max-min)* rand() / (RAND_MAX + 1.0);
}


#endif