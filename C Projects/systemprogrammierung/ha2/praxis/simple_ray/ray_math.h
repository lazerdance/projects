/*
Author: Cornelius Marx
Date: April 10th 2019
Description:
	Math functions and structs needed for raytracing algorithms.
*/
#ifndef RAY_MATH_H
#define RAY_MATH_H

#include <math.h>
#include <string.h>
#include <stdio.h>
/* defining own PI constant for as M_PI might not be defined on some systems */
#define F_PI 3.14159265358979323846

//2D vector
typedef struct
{
	float x, y;
}f_vec2;
void f_vec2_set(f_vec2 * v, float x, float y);
float f_vec2_getLength(const f_vec2 * v);
void f_vec2_normalize(f_vec2 * v);
void f_vec2_add(const f_vec2 * a, const f_vec2 * b, f_vec2 * result);
void f_vec2_mult(const f_vec2 *  v, float s, f_vec2 * result);
void f_vec2_multv(const f_vec2 *  a, const f_vec2 * b, f_vec2 * result);//component-wise multiplication
float f_vec2_dot(const f_vec2 * a, const f_vec2 * b);
float f_vec2_getAngle(const f_vec2 * a, const f_vec2 * b);//get angle (radiants) between vector a and b

//3D vector
typedef struct
{
	float x, y, z;
}f_vec3;
void f_vec3_set(f_vec3 * v, float x, float y, float z);
float f_vec3_getLength(const f_vec3 * v);
void f_vec3_normalize(f_vec3 * v);
void f_vec3_add(const f_vec3 * a, const f_vec3 * b, f_vec3 * result);
void f_vec3_sub(const f_vec3 * a, const f_vec3 * b, f_vec3 * result);
void f_vec3_mult(const f_vec3 *  v, float s, f_vec3 * result);
void f_vec3_multv(const f_vec3 *  a, const f_vec3 * b, f_vec3 * result);//component-wise multiplication
void f_vec3_div(const f_vec3 *  v, float s, f_vec3 * result);
float f_vec3_dot(const f_vec3 * a, const f_vec3 * b);
float f_vec3_getAngle(const f_vec3 * a, const f_vec3 * b);//get angle (radiants) between vector a and b
void f_vec3_cross(const f_vec3 * a, const f_vec3 * b, f_vec3 * result);
void f_vec3_reflect(const f_vec3 * incoming, const f_vec3 * n, f_vec3 * outgoing);/* reflecting along normal n (all vectors need to be normalized) */
void f_vec3_print(const f_vec3 * v);

//4x4 matrix
typedef struct
{
	float e[16];//entries stored in column row order
}f_mat4;
void f_mat4_setIdentity(f_mat4 * m);
void f_mat4_setTranslation(f_mat4 * m, float x, float y, float z);
void f_mat4_setTranslationv(f_mat4 * m, const f_vec3 * v);
void f_mat4_translate(f_mat4 * m, float x, float y, float z);//multiply by translation matrix
void f_mat4_translatev(f_mat4 * m, const f_vec3 * v);//multiply by translation matrix
void f_mat4_setScale(f_mat4 * m, float x, float y, float z); 
void f_mat4_setScalev(f_mat4 * m, const f_vec3 * v); 
void f_mat4_scale(f_mat4 * m, float x, float y, float z);//multiply by scale matrix
void f_mat4_scalev(f_mat4 * m, const f_vec3 * v);//multiply by scale matrix
void f_mat4_setRotation(f_mat4 * m, float angle_rad, float x, float y, float z);
void f_mat4_setRotationv(f_mat4 * m, float angle_rad, const f_vec3 * v);
void f_mat4_rotatev(f_mat4 * m, float angle_rad, const f_vec3 * v);//multiply by rotation matrix
void f_mat4_rotate(f_mat4 * m, float angle_rad, float x, float y, float z);//multiply by rotation matrix
void f_mat4_setRotationX(f_mat4 * m, float angle_rad_x);
void f_mat4_rotateX(f_mat4 * m, float angle_rad_x);//multiply by rotation matrix
void f_mat4_setRotationY(f_mat4 * m, float angle_rad_y);
void f_mat4_rotateY(f_mat4 * m, float angle_rad_y);//multiply by rotation matrix
void f_mat4_setRotationZ(f_mat4 * m, float angle_rad_z);
void f_mat4_rotateZ(f_mat4 * m, float angle_rad_z);//multiply by rotation matrix
void f_mat4_multm(const f_mat4 * a, const f_mat4 * b, f_mat4 * result);//result = a*b
float f_mat4_multv(const f_mat4 * m, const f_vec3 * v, float w, f_vec3 * result);//returns resulting w-component
void f_mat4_add(const f_mat4 * a, const f_mat4 * b, f_mat4 * result);
void f_mat4_mult(const f_mat4 * m, float s, f_mat4 * result);/* multiply by scalar */
void f_mat4_setOrtho(f_mat4 * m, float left, float right, float bottom, float top, float near, float far);
void f_mat4_setPerspective(f_mat4 * m, float left, float right, float bottom, float top, float near, float far);
void f_mat4_setPerspectiveX(f_mat4 * m, float fov_x_angle_rad, float aspect, float near, float far);
void f_mat4_setPerspectiveY(f_mat4 * m, float fov_y_angle_rad, float aspect, float near, float far);
void f_mat4_affineInvertTranspose(const f_mat4 * m, f_mat4 * result);
void f_mat4_affineInvert(const f_mat4 * m, f_mat4 * result);
int f_mat4_invert(const f_mat4 * m, f_mat4 * result);//returns 0 if matrix is not invertible
int f_mat4_invertTranspose(const f_mat4 * m, f_mat4 * result);//returns 0 if matrix is not invertible
void f_mat4_transpose(const f_mat4 * m, f_mat4 * result);
void f_mat4_print(const f_mat4 * m);

//returning maximum/minimum of two given values
int f_imax(int a, int b);
int f_imin(int a, int b);
float f_fmax(float a, float b);
float f_fmin(float a, float b);

//limiting given value to upper and lower bound
int f_ilimit(int a, int max, int min);
float f_flimit(float a, float max, float min);

//round to next integer
float f_round(float a);

//signum: returns 1 if a is positive, -1 if a is negative or else 0
float f_fsign(float a);
int f_isign(int a);

//converting radiants to degrees and vice versa
float f_rad(float deg);
float f_deg(float rad);

//interpolating quadratic from 0 to 1
float f_quadricInterpolate(float t);

/* raycasting */
typedef struct{
	f_vec3 s;/* starting point */
	f_vec3 v;/* direction */
} f_ray;

void f_ray_transform(const f_ray * ray, const f_mat4 * mat, f_ray * ray_trans);

/* raycasting against sphere, returns 1 if an intersection was detected */
int f_ray_sphere(const f_ray* ray, float sphere_radius,
					f_vec3 * hit_pos, f_vec3 * hit_normal);

/* raycasting against a single plane lying at origin with dimensions r_x and r_y
	return: 1 if an intersection occured
 */
int f_ray_planeXY(const f_ray* ray, float r_x, float r_y, f_vec3 * hit_pos);
int f_ray_planeXZ(const f_ray* ray, float r_x, float r_z, f_vec3 * hit_pos);
int f_ray_planeYZ(const f_ray* ray, float r_y, float r_z, f_vec3 * hit_pos);

/* raycasting against a box lying at the origin with dimensions r_x, r_y and r_z */
int f_ray_box(const f_ray* ray, const f_vec3 * dim, f_vec3 * hit_pos, f_vec3 * hit_normal);

/* generate random direction vector in hemisphere defined by given normal */
void f_ray_diffuseReflect(const f_vec3 * n, f_vec3 * out,
							int rand_value1, int rand_value2, int rand_max);
#endif
