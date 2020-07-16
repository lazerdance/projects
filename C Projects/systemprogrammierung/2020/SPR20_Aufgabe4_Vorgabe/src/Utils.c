/**
 * @file Utils.c
 * @brief Helper functions for distancing simulator. You dont have to read this file :)
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#include <stdlib.h>
#include <math.h>

#include "Utils.h"

/////////////////////////////////
// definition global functions //
/////////////////////////////////

bool Vec2f_dist_lt(const Vec2f *a, const Vec2f *b, float dist)
{
	return (powf(a->x - b->x, 2) + powf(a->y - b->y, 2) <= powf(dist, 2));
}

float rand_float(float min, float max)
{
	return min + rand() / (float) RAND_MAX * (max - min);
}

int rand_sign(void)
{
	return (((rand() % 2) * 2) - 1);
}
