/*
Author: Cornelius Marx
Date: April 12th 2019
Description:
	Defines values used as constants throughout rendering
*/
#ifndef RAY_SETTINGS_H
#define RAY_SETTINGS_H

/***** settings *****/

#define RAY_SAMPLE_FRACTION		10000 /* how many possible samples per pixel along one axis */
#define RAY_NUM_BOUNCES			4 /* number of ray bounces calculated per sample */
#define RAY_DIFFUSE_SAMPLES		32	

/* after sampling one pixel a color ramp is applied to the final color, remapping the non-normalized color space */
#define RAY_COLOR_RAMP_BLACK	0.f /* color channels of this value and below remapped to a value of 0 in normalized color space*/ 
#define RAY_COLOR_RAMP_WHITE	1.f /* color channels of this value and above are mapped to a value of 1 in normalized color space */

#define RAY_NOISE_SEED	3141596

/********************/
#endif
