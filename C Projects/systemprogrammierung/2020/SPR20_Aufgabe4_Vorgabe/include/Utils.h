/**
 * @file Utils.h
 * @brief Helper functions and data structs for distancing simulator.
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct Vec2f Vec2f;

/**
 * @brief 2D float vector
 */
struct Vec2f
{
	float x;
	float y;
};

/**
 * Check if points a and b are in distance dist. Should be called by
 * Scenario_CheckInfections().
 * @param a point a
 * @param b point b
 * @return true when a and b in distance dist, else false
 */
bool Vec2f_dist_lt(const Vec2f *a, const Vec2f *b, float dist);

/**
 * Generate random float in a range.
 * @param min lower range bound
 * @param max upper range bound
 * @return random float
 */
float rand_float(float min, float max);

/**
 * Generate random -1 or 1.
 * @return -1 or 1 at random
 */
int rand_sign(void);

#endif