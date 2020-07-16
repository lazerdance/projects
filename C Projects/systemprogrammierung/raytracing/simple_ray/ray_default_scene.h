/*
Author: Cornelius Marx
Date: April 12th 2019
Description:
	Function for initializing default scene consisting of a simple Cornell-box with a glossy sphere and a diffuse cube
*/
#ifndef RAY_DEFAULT_SCENE_H
#define RAY_DEFAULT_SCENE_H

#include "ray.h"

/* scene settings */
#define SCENE_FOV_ANGLE 			15.0f
#define SCENE_CAMERA_DISTANCE 		2.7f
#define SCENE_BOX_SIZE				2.0f
#define SCENE_SPHERE_RADIUS			0.5f
#define SCENE_SPHERE_X				-0.3f
#define SCENE_SPHERE_Z				-0.1f
#define SCENE_SPHERE_COLOR          {0.1f, 1.0f, 0.3f}
#define SCENE_CUBE_SIZE				0.6f
#define SCENE_CUBE_X				0.6f
#define SCENE_CUBE_Z				0.3f
#define SCENE_CUBE_COLOR			{0.1f, 0.5f, 1.0f}
#define SCENE_BOTTOM_PLANE_COLOR	{0.8f, 0.8f, 0.8f}	
#define SCENE_TOP_PLANE_COLOR		{0.2f, 0.8f, 0.8f}
#define SCENE_LEFT_PLANE_COLOR		{0.8f, 0.75f, 0.2f}
#define SCENE_RIGHT_PLANE_COLOR		{0.9f, 0.3f, 0.3f}	
#define SCENE_BACK_PLANE_COLOR		{0.8f, 0.2f, 0.8f}
#define SCENE_FRONT_PLANE_COLOR		{0.3f, 0.7f, 0.3f}
#define SCENE_LIGHT_COLOR			{1.0f, 1.0f, 1.0f}
#define SCENE_LIGHT_OFFSET			0.1f
#define SCENE_LIGHT_CONSTANT_ATTEN	0.9f
#define SCENE_LIGHT_LINEAR_ATTEN	0.1f
#define SCENE_LIGHT_QUADRIC_ATTEN	0.3f
#define SCENE_AMBIENT_COLOR			{0.2f, 0.2f, 0.2f}	

/* initialize default scene */
ray_Scene* ray_createDefaultScene();

#endif
