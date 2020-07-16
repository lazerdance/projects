/*
Author: Cornelius Marx
Date: April 10th 2019
*/
#include "ray_math.h"

void f_vec3_print(const f_vec3 * v)
{
	printf("%fx %fy %fz\n", v->x, v->y, v->z);
}

void f_mat4_print(const f_mat4 * m)
{
	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
			m->e[0], m->e[4], m->e[8], m->e[12],
			m->e[1], m->e[5], m->e[9], m->e[13],
			m->e[2], m->e[6], m->e[10], m->e[14],
			m->e[3], m->e[7], m->e[11], m->e[15]
		);
}

int f_imax(int a, int b)
{
	if(a > b)
		return a;

	return b;
}

int f_imin(int a, int b)
{
	if(a < b)
		return a;

	return b;
}

float f_fmax(float a, float b)
{
	if(a > b)
		return a;

	return b;
}

float f_fmin(float a, float b)
{
	if(a < b)
		return a;

	return b;
}


int f_ilimit(int a, int max, int min)
{
	if(a < min)
		return min;

	if(a > max)
		return max;

	return a;
}

float f_flimit(float a, float max, float min)
{
	if(a < min)
		return min;

	if(a > max)
		return max;

	return a;
}

float f_round(float a)
{
	if(a >= 0)
		return (float)((int)(a+0.5f));
	
	return (float)((int)(a-0.5f));
}

float f_fsign(float a)
{
	if(a > 0.f)
		return 1.f;

	if(a < 0.f)
		return -1.f;

	return 0.f;
}

int f_isign(int a)
{
	if(a > 0)
		return 1;

	if(a < 0)
		return -1;

	return 0;
}

float f_rad(float deg)
{
	return F_PI*(deg/180.f);
}

float f_deg(float rad)
{
	return 180.f*(rad/F_PI);
}

float f_quadricInterpolate(float t)
{
	if(t <= 0.5f)
		return 2*t*t;

	float t_ = t-1;
	return 1 - 2*t_*t_;
}

/* Vector2D */
void f_vec2_set(f_vec2 * v, float x, float y)
{
	v->x = x;
	v->y = y;
}

float f_vec2_getLength(const f_vec2 * v)
{
	return sqrt(v->x*v->x + v->y*v->y);
}

void f_vec2_normalize(f_vec2 * v)
{
	float l = f_vec2_getLength(v);
	v->x /= l;
	v->y /= l;
}

void f_vec2_add(const f_vec2 * a, const f_vec2 * b, f_vec2 * result)
{
	result->x = a->x + b->x;
	result->y = a->y + b->y;
}

void f_vec2_mult(const f_vec2 * v, float s, f_vec2 * result)
{
	result->x = v->x*s;
	result->y = v->y*s;
}

void f_vec2_multv(const f_vec2 *  a, const f_vec2 * b, f_vec2 * result)
{
	result->x = a->x*b->x;
	result->y = a->y*b->y;
}

float f_vec2_dot(const f_vec2 * a, const f_vec2 * b)
{
	return a->x*b->x + a->y*b->y;
}

float f_vec2_getAngle(const f_vec2 * a, const f_vec2 * b)//get angle (radiants) between vector a and 
{
	return acos((f_vec2_dot(a, b))/(f_vec2_getLength(a)*f_vec2_getLength(b)));
}

/* Vector3D */
void f_vec3_set(f_vec3 * v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

float f_vec3_getLength(const f_vec3 * v)
{
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

void f_vec3_normalize(f_vec3 * v)
{
	float l = f_vec3_getLength(v);
	v->x /= l;
	v->y /= l;
	v->z /= l;
}

void f_vec3_add(const f_vec3 * a, const f_vec3 * b, f_vec3 * result)
{
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	result->z = a->z + b->z;
}
void f_vec3_sub(const f_vec3 * a, const f_vec3 * b, f_vec3 * result)
{
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
}

void f_vec3_mult(const f_vec3 * v, float s, f_vec3 * result)
{
	result->x = v->x*s;
	result->y = v->y*s;
	result->z = v->z*s;
}

void f_vec3_multv(const f_vec3 *  a, const f_vec3 * b, f_vec3 * result)
{
	result->x = a->x*b->x;
	result->y = a->y*b->y;
	result->z = a->z*b->z;
}

void f_vec3_div(const f_vec3 *  v, float s, f_vec3 * result)
{
	result->x = v->x/s;
	result->y = v->y/s;
	result->z = v->z/s;
}

float f_vec3_dot(const f_vec3 * a, const f_vec3 * b)
{
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

float f_vec3_getAngle(const f_vec3 * a, const f_vec3 * b)//get angle (radiants) between vector a and 
{
	return acos((f_vec3_dot(a, b))/(f_vec3_getLength(a)*f_vec3_getLength(b)));
}

void f_vec3_cross(const f_vec3 * a, const f_vec3 * b, f_vec3 * result)
{
	//write to tmp-vars first to prevent miscalculation if operant is passed as result-pointer
	float x = a->y*b->z - a->z*b->y;
	float y = a->z*b->x - a->x*b->z;
	result->z = a->x*b->y - a->y*b->x;
	result->x = x;
	result->y = y;
}

void f_vec3_reflect(const f_vec3 * incoming, const f_vec3 * n, f_vec3 * outgoing)
{
	f_vec3 t;
	float s = f_vec3_dot(incoming, n);
	f_vec3_mult(n, -2.0f*s, &t); 
	f_vec3_add(&t, incoming, outgoing);
}

/* Matrix4x4 */
void f_mat4_setIdentity(f_mat4 * m)
{
	m->e[0] = 1; m->e[4] = 0; m->e[8]  = 0; m->e[12] = 0;
	m->e[1] = 0; m->e[5] = 1; m->e[9]  = 0; m->e[13] = 0;
	m->e[2] = 0; m->e[6] = 0; m->e[10] = 1; m->e[14] = 0;
	m->e[3] = 0; m->e[7] = 0; m->e[11] = 0; m->e[15] = 1;
}

void f_mat4_setTranslation(f_mat4 * m, float x, float y, float z)
{
	m->e[0] = 1; m->e[4] = 0; m->e[8]  = 0; m->e[12] = x;
	m->e[1] = 0; m->e[5] = 1; m->e[9]  = 0; m->e[13] = y;
	m->e[2] = 0; m->e[6] = 0; m->e[10] = 1; m->e[14] = z;
	m->e[3] = 0; m->e[7] = 0; m->e[11] = 0; m->e[15] = 1;
}

void f_mat4_setTranslationv(f_mat4 * m, const f_vec3 * v)
{
	f_mat4_setTranslation(m, v->x, v->y, v->z);
}

void f_mat4_translate(f_mat4 * m, float x, float y, float z)
{
	m->e[12] = m->e[0]*x + m->e[4]*y + m->e[8]*z + m->e[12];
	m->e[13] = m->e[1]*x + m->e[5]*y + m->e[9]*z + m->e[13];
	m->e[14] = m->e[2]*x + m->e[6]*y + m->e[10]*z+ m->e[14];
	m->e[15] = m->e[3]*x + m->e[7]*y + m->e[11]*z+ m->e[15];
}

void f_mat4_translatev(f_mat4 * m, const f_vec3 * v)
{
	f_mat4_translate(m, v->x, v->y, v->z);
}

void f_mat4_setScale(f_mat4 * m, float x, float y, float z)
{
	m->e[0] = x; m->e[4] = 0; m->e[8]  = 0; m->e[12] = 0;
	m->e[1] = 0; m->e[5] = y; m->e[9]  = 0; m->e[13] = 0;
	m->e[2] = 0; m->e[6] = 0; m->e[10] = z; m->e[14] = 0;
	m->e[3] = 0; m->e[7] = 0; m->e[11] = 0; m->e[15] = 1;
}

void f_mat4_setScalev(f_mat4 * m, const f_vec3 * v)
{
	f_mat4_setScale(m, v->x, v->y, v->z);
}

void f_mat4_scale(f_mat4 * m, float x, float y, float z)
{
	m->e[0] *= x;	
	m->e[1] *= x;
	m->e[2] *= x;
	m->e[3] *= x;

	m->e[5] *= y;
	m->e[6] *= y;
	m->e[7] *= y;
	m->e[8] *= y;

	m->e[9]  *= z;
	m->e[10] *= z;
	m->e[11] *= z;
	m->e[12] *= z;
}

void f_mat4_scalev(f_mat4 * m, const f_vec3 * v)
{
	f_mat4_scale(m, v->x, v->y, v->z);
}

void f_mat4_setRotation(f_mat4 * m, float angle_rad, float x, float y, float z)
{
	float c = (float)cos((double)angle_rad);
	float s = (float)sin((double)angle_rad);
	m->e[0] = c+(1-c)*x*x; 		m->e[4] = (1-c)*x*y-s*z;	m->e[8]  = (1-c)*x*z+s*y; 	m->e[12] = 0;
	m->e[1] = (1-c)*x*y+s*z; 	m->e[5] = c+(1-c)*y*y; 		m->e[9]  = (1-c)*y*z-s*x; 	m->e[13] = 0;
	m->e[2] = (1-c)*x*z-s*y; 	m->e[6] = (1-c)*y*z+s*x; 	m->e[10] = c+(1-c)*z*z;		m->e[14] = 0;
	m->e[3] = 0; 				m->e[7] = 0; 				m->e[11] = 0; 				m->e[15] = 1;
}

void f_mat4_setRotationv(f_mat4 * m, float angle_rad, const f_vec3 * v)
{
	f_mat4_setRotation(m, angle_rad, v->x, v->y, v->z);
}

void f_mat4_rotate(f_mat4 * m, float angle_rad, float x, float y, float z)
{
	f_mat4 r;
	f_mat4_setRotation(&r, angle_rad, x, y, z);
	float m_tmp[12];
	m_tmp[0]  = m->e[0]*r.e[0] + m->e[4]*r.e[1] + m->e[8]*r.e[2]; 
	m_tmp[1]  = m->e[1]*r.e[0] + m->e[5]*r.e[1] + m->e[9]*r.e[2]; 
	m_tmp[2]  = m->e[2]*r.e[0] + m->e[6]*r.e[1] + m->e[10]*r.e[2]; 
	m_tmp[3]  = m->e[3]*r.e[0] + m->e[7]*r.e[1] + m->e[11]*r.e[2]; 

	m_tmp[4]  = m->e[0]*r.e[4] + m->e[4]*r.e[5] + m->e[8]*r.e[6]; 
	m_tmp[5]  = m->e[1]*r.e[4] + m->e[5]*r.e[5] + m->e[9]*r.e[6]; 
	m_tmp[6]  = m->e[2]*r.e[4] + m->e[6]*r.e[5] + m->e[10]*r.e[6]; 
	m_tmp[7]  = m->e[3]*r.e[4] + m->e[7]*r.e[5] + m->e[11]*r.e[6]; 

	m_tmp[8]  = m->e[0]*r.e[8] + m->e[4]*r.e[9] + m->e[8]*r.e[10]; 
	m_tmp[9]  = m->e[1]*r.e[8] + m->e[5]*r.e[9] + m->e[9]*r.e[10]; 
	m_tmp[10] = m->e[2]*r.e[8] + m->e[6]*r.e[9] + m->e[10]*r.e[10]; 
	m_tmp[11] = m->e[3]*r.e[8] + m->e[7]*r.e[9] + m->e[11]*r.e[10]; 

	memcpy(m->e, m_tmp, sizeof(float)*12);
}

void f_mat4_rotatev(f_mat4 * m, float angle_rad, const f_vec3 * v)
{
	f_mat4_rotate(m, angle_rad, v->x, v->y, v->z);
}

void f_mat4_setRotationX(f_mat4 * m, float x)
{
	float cos_x = (float)cos((double)x);
	float sin_x = (float)sin((double)x);
	m->e[0] = 1; m->e[4] = 0; 		m->e[8]  = 0; 		m->e[12] = 0;
	m->e[1] = 0; m->e[5] = cos_x; 	m->e[9]  = -sin_x; 	m->e[13] = 0;
	m->e[2] = 0; m->e[6] = sin_x; 	m->e[10] = cos_x;	m->e[14] = 0;
	m->e[3] = 0; m->e[7] = 0; 		m->e[11] = 0; 		m->e[15] = 1;
}

void f_mat4_rotateX(f_mat4 * m, float x)
{
	float cos_x = (float)cos((double)x);
	float sin_x = (float)sin((double)x);
	float m_tmp[8];
	m_tmp[0] = cos_x*m->e[4] + sin_x*m->e[8];
	m_tmp[1] = cos_x*m->e[5] + sin_x*m->e[9];
	m_tmp[2] = cos_x*m->e[6] + sin_x*m->e[10];
	m_tmp[3] = cos_x*m->e[7] + sin_x*m->e[11];

	m_tmp[4] = -sin_x*m->e[4] + cos_x*m->e[8];
	m_tmp[5] = -sin_x*m->e[5] + cos_x*m->e[9];
	m_tmp[6] = -sin_x*m->e[6] + cos_x*m->e[10];
	m_tmp[7] = -sin_x*m->e[7] + cos_x*m->e[11];

	memcpy(&m->e[4], m_tmp, sizeof(float)*8);
}

void f_mat4_setRotationY(f_mat4 * m, float y)
{
	float cos_y = (float)cos((double)y);
	float sin_y = (float)sin((double)y);
	m->e[0] = cos_y; 	m->e[4] = 0; m->e[8]  = sin_y; 	m->e[12] = 0;
	m->e[1] = 0; 		m->e[5] = 1; m->e[9]  = 0; 		m->e[13] = 0;
	m->e[2] = -sin_y;	m->e[6] = 0; m->e[10] = cos_y;	m->e[14] = 0;
	m->e[3] = 0; 		m->e[7] = 0; m->e[11] = 0; 		m->e[15] = 1;
}

void f_mat4_rotateY(f_mat4 * m, float y)
{
	float cos_y = (float)cos((double)y);
	float sin_y = (float)sin((double)y);
	float m_tmp[8];
	m_tmp[0] = cos_y*m->e[0] - sin_y*m->e[8];
	m_tmp[1] = cos_y*m->e[1] - sin_y*m->e[9];
	m_tmp[2] = cos_y*m->e[2] - sin_y*m->e[10];
	m_tmp[3] = cos_y*m->e[3] - sin_y*m->e[11];

	m_tmp[4] = sin_y*m->e[0] + cos_y*m->e[8];
	m_tmp[5] = sin_y*m->e[1] + cos_y*m->e[9];
	m_tmp[6] = sin_y*m->e[2] + cos_y*m->e[10];
	m_tmp[7] = sin_y*m->e[3] + cos_y*m->e[11];

	memcpy(m->e, m_tmp, sizeof(float)*4);
	memcpy(&m->e[8], &m_tmp[4], sizeof(float)*4);
}

void f_mat4_setRotationZ(f_mat4 * m, float z)
{
	float cos_z = (float)cos((double)z);
	float sin_z = (float)sin((double)z);
	m->e[0] = cos_z; 	m->e[4] = -sin_z;	m->e[8]  = 0; m->e[12] = 0;
	m->e[1] = sin_z; 	m->e[5] = cos_z; 	m->e[9]  = 0; m->e[13] = 0;
	m->e[2] = 0; 		m->e[6] = 0; 		m->e[10] = 1; m->e[14] = 0;
	m->e[3] = 0; 		m->e[7] = 0; 		m->e[11] = 0; m->e[15] = 1;
}

void f_mat4_rotateZ(f_mat4 * m, float z)
{
	float cos_z = (float)cos((double)z);
	float sin_z = (float)sin((double)z);
	float m_tmp[8];
	m_tmp[0] = cos_z*m->e[0] + sin_z*m->e[4];
	m_tmp[1] = cos_z*m->e[1] + sin_z*m->e[5];
	m_tmp[2] = cos_z*m->e[2] + sin_z*m->e[6];
	m_tmp[3] = cos_z*m->e[3] + sin_z*m->e[7];

	m_tmp[4] = -sin_z*m->e[0] + cos_z*m->e[4];
	m_tmp[5] = -sin_z*m->e[1] + cos_z*m->e[5];
	m_tmp[6] = -sin_z*m->e[2] + cos_z*m->e[6];
	m_tmp[7] = -sin_z*m->e[3] + cos_z*m->e[7];

	memcpy(m->e, m_tmp, sizeof(float)*8);
}

void f_mat4_multm(const f_mat4 * a, const f_mat4 * b, f_mat4 * result)
{
	//write to tmp-var to prevent misscalculation if operant (a,b) is passed as result pointer
	f_mat4 r;
	r.e[0]  = a->e[0]*b->e[0] + a->e[4]*b->e[1] + a->e[8]*b->e[2]  + a->e[12]*b->e[3]; 
	r.e[1]  = a->e[1]*b->e[0] + a->e[5]*b->e[1] + a->e[9]*b->e[2]  + a->e[13]*b->e[3]; 
	r.e[2]  = a->e[2]*b->e[0] + a->e[6]*b->e[1] + a->e[10]*b->e[2] + a->e[14]*b->e[3]; 
	r.e[3]  = a->e[3]*b->e[0] + a->e[7]*b->e[1] + a->e[11]*b->e[2] + a->e[15]*b->e[3]; 

	r.e[4]  = a->e[0]*b->e[4] + a->e[4]*b->e[5] + a->e[8]*b->e[6]  + a->e[12]*b->e[7]; 
	r.e[5]  = a->e[1]*b->e[4] + a->e[5]*b->e[5] + a->e[9]*b->e[6]  + a->e[13]*b->e[7]; 
	r.e[6]  = a->e[2]*b->e[4] + a->e[6]*b->e[5] + a->e[10]*b->e[6] + a->e[14]*b->e[7]; 
	r.e[7]  = a->e[3]*b->e[4] + a->e[7]*b->e[5] + a->e[11]*b->e[6] + a->e[15]*b->e[7]; 

	r.e[8]  = a->e[0]*b->e[8] + a->e[4]*b->e[9] + a->e[8]*b->e[10]  + a->e[12]*b->e[11]; 
	r.e[9]  = a->e[1]*b->e[8] + a->e[5]*b->e[9] + a->e[9]*b->e[10]  + a->e[13]*b->e[11]; 
	r.e[10] = a->e[2]*b->e[8] + a->e[6]*b->e[9] + a->e[10]*b->e[10] + a->e[14]*b->e[11]; 
	r.e[11] = a->e[3]*b->e[8] + a->e[7]*b->e[9] + a->e[11]*b->e[10] + a->e[15]*b->e[11]; 

	r.e[12] = a->e[0]*b->e[12] + a->e[4]*b->e[13] + a->e[8]*b->e[14]  + a->e[12]*b->e[15]; 
	r.e[13] = a->e[1]*b->e[12] + a->e[5]*b->e[13] + a->e[9]*b->e[14]  + a->e[13]*b->e[15]; 
	r.e[14] = a->e[2]*b->e[12] + a->e[6]*b->e[13] + a->e[10]*b->e[14] + a->e[14]*b->e[15]; 
	r.e[15] = a->e[3]*b->e[12] + a->e[7]*b->e[13] + a->e[11]*b->e[14] + a->e[15]*b->e[15]; 

	*result = r;
}

float f_mat4_multv(const f_mat4 * m, const f_vec3 * v, float w, f_vec3 * result)
{
	f_vec3 r;
	r.x = m->e[0]*v->x + m->e[4]*v->y + m->e[8]*v->z + m->e[12]*w;
	r.y = m->e[1]*v->x + m->e[5]*v->y + m->e[9]*v->z + m->e[13]*w;
	r.z = m->e[2]*v->x + m->e[6]*v->y + m->e[10]*v->z+ m->e[14]*w;
	
	float _w = m->e[3]*v->x + m->e[7]*v->y + m->e[11]*v->z + m->e[15]*w;
	*result = r;
	return _w;
}

void f_mat4_mult(const f_mat4 * m, float s, f_mat4 * result)
{
	for(int i = 0; i < 16; i++)
		result->e[i] = m->e[i]*s;
}

void f_mat4_add(const f_mat4 * a, const f_mat4 * b, f_mat4 * result)
{
	for(int i = 0; i < 16; i++)
		result->e[i] = a->e[i] + b->e[i];
}

void f_mat4_setOrtho(f_mat4 * m, float left, float right, float bottom, float top, float near, float far)
{
	m->e[0]  = 2.0f/(right-left);
    m->e[1]  = 0.f;
    m->e[2]  = 0.f;
    m->e[3]  = 0.f;
    m->e[4]  = 0.f;
	m->e[5]  = 2.0f/(top-bottom);
    m->e[6]  = 0.f;
	m->e[7]  = 0.f;
    m->e[8]  = 0.f;
    m->e[9]  = 0.f;
    m->e[10] =-2.f/(far-near);
    m->e[11] = 0.f;
	m->e[12] =-(right+left)/(right-left);
	m->e[13] =-(top+bottom)/(top-bottom);
	m->e[14] =-(far+near)/(far-near);
    m->e[15] = 1.f;
}

void f_mat4_setPerspective(f_mat4 * m, float left, float right, float bottom, float top, float near, float far)
{
	m->e[0]  = (2*near)/(right-left);
    m->e[1]  = 0.f;
    m->e[2]  = 0.f;
    m->e[3]  = 0.f;
    m->e[4]  = 0.f;
	m->e[5]  = (2*near)/(top-bottom);
    m->e[6]  = 0.f;
    m->e[7]  = 0.f;
	m->e[8]  = (right+left)/(right-left);
	m->e[9]  = (top+bottom)/(top-bottom);
    m->e[10] =-(far+near)/(far-near);
	m->e[11] =-1.f;
    m->e[12] = 0.f;
    m->e[13] = 0.f;
    m->e[14] =-(2*far*near)/(far-near);
    m->e[15] = 0.f;
}

void f_mat4_setPerspectiveX(f_mat4 * m, float fovx, float aspect, float near, float far)
{
    float deltaZ = near - far;
	float a =1.f/tan(fovx);
	m->e[0] = a;
	m->e[1] = 0.f;
	m->e[2] = 0.f;
	m->e[3] = 0.f;

	m->e[4] = 0;
	m->e[5] = a/aspect;
	m->e[6] = 0.f;
	m->e[7] = 0.f;

	m->e[8] = 0.f;	
	m->e[9] = 0.f;	
	m->e[10] = (near + far)/deltaZ;	
	m->e[11] = -1.f;	

	m->e[12]  = 0.f;
	m->e[13]  = 0.f;
	m->e[14]  = (2*near*far)/deltaZ;
	m->e[15]  = 0.f;
}

void f_mat4_setPerspectiveY(f_mat4 * m, float fovy, float aspect, float near, float far)
{
	float deltaZ = near - far;
	float a= 1.f/tan(fovy);
	
	m->e[0] = a/aspect;
	m->e[1] = 0.f;
	m->e[2] = 0.f;
	m->e[3] = 0.f;
	
	m->e[4] = 0.f;
	m->e[5] = a;
	m->e[6] = 0.f;
	m->e[7] = 0.f;
	
	m->e[8] = 0.f;	
	m->e[9] = 0.f;	
	m->e[10] = (near + far)/deltaZ;	
	m->e[11] = -1.f;	

	m->e[12]  = 0.f;
	m->e[13]  = 0.f;
	m->e[14]  = (2*near*far)/deltaZ;
	m->e[15]  = 0.f;
}

void f_mat4_affineInvertTranspose(const f_mat4 * m, f_mat4 * result)
{
	f_mat4 tmp;
	tmp.e[0] = m->e[0];
	tmp.e[1] = m->e[1];
	tmp.e[2] = m->e[2];
	tmp.e[3] = -(m->e[0]*m->e[12]+m->e[1]*m->e[13]+m->e[2]*m->e[14]);

	tmp.e[4] = m->e[4];
	tmp.e[5] = m->e[5];
	tmp.e[6] = m->e[6];
	tmp.e[7] = -(m->e[4]*m->e[12]+m->e[5]*m->e[13]+m->e[6]*m->e[14]);

	tmp.e[8] = m->e[8];
	tmp.e[9] = m->e[9];
	tmp.e[10] = m->e[10];
	tmp.e[11] = -(m->e[8]*m->e[12]+m->e[9]*m->e[13]+m->e[10]*m->e[14]);

	tmp.e[12] = 0.f;
	tmp.e[13] = 0.f;
	tmp.e[14] = 0.f,
	tmp.e[15] = 1.f;
	*result = tmp;
}

void f_mat4_affineInvert(const f_mat4 * m, f_mat4 * result)
{
	f_mat4 tmp;
	tmp.e[0] = m->e[0];
	tmp.e[1] = m->e[4];
	tmp.e[2] = m->e[8];
	tmp.e[3] = 0.f;

	tmp.e[4] = m->e[1];
	tmp.e[5] = m->e[5];
	tmp.e[6] = m->e[9];
	tmp.e[7] = 0.f;

	tmp.e[8] = m->e[2];
	tmp.e[9] = m->e[6];
	tmp.e[10] = m->e[10];
	tmp.e[11] = 0.f;

	tmp.e[12] = -(m->e[0]*m->e[12]+m->e[1]*m->e[13]+m->e[2]*m->e[14]);
	tmp.e[13] = -(m->e[4]*m->e[12]+m->e[5]*m->e[13]+m->e[6]*m->e[14]);
	tmp.e[14] = -(m->e[8]*m->e[12]+m->e[9]*m->e[13]+m->e[10]*m->e[14]),
	tmp.e[15] = 1.f;
	*result = tmp;
}

int f_mat4_invertTranspose(const f_mat4 * m, f_mat4 * result)
{
	float tmp[12];												//temporary pair storage
	float res[16];
	float det;													//determinant

	//calculate pairs for first 8 elements (cofactors
	tmp[0] = m->e[10] * m->e[15];
	tmp[1] = m->e[11] * m->e[14];
	tmp[2] = m->e[9] * m->e[15];
	tmp[3] = m->e[11] * m->e[13];
	tmp[4] = m->e[9] * m->e[14];
	tmp[5] = m->e[10] * m->e[13];
	tmp[6] = m->e[8] * m->e[15];
	tmp[7] = m->e[11] * m->e[12];
	tmp[8] = m->e[8] * m->e[14];
	tmp[9] = m->e[10] * m->e[12];
	tmp[10] = m->e[8] * m->e[13];
	tmp[11] = m->e[9] * m->e[12];

	//calculate first 8 elements (cofactors)
	res[0]=		tmp[0]*m->e[5] + tmp[3]*m->e[6] + tmp[4]*m->e[7]
					-	tmp[1]*m->e[5] - tmp[2]*m->e[6] - tmp[5]*m->e[7];

	res[1]=		tmp[1]*m->e[4] + tmp[6]*m->e[6] + tmp[9]*m->e[7]
					-	tmp[0]*m->e[4] - tmp[7]*m->e[6] - tmp[8]*m->e[7];

	res[2]=		tmp[2]*m->e[4] + tmp[7]*m->e[5] + tmp[10]*m->e[7]
					-	tmp[3]*m->e[4] - tmp[6]*m->e[5] - tmp[11]*m->e[7];

	res[3]=		tmp[5]*m->e[4] + tmp[8]*m->e[5] + tmp[11]*m->e[6]
					-	tmp[4]*m->e[4] - tmp[9]*m->e[5] - tmp[10]*m->e[6];

	res[4]=		tmp[1]*m->e[1] + tmp[2]*m->e[2] + tmp[5]*m->e[3]
					-	tmp[0]*m->e[1] - tmp[3]*m->e[2] - tmp[4]*m->e[3];

	res[5]=		tmp[0]*m->e[0] + tmp[7]*m->e[2] + tmp[8]*m->e[3]
					-	tmp[1]*m->e[0] - tmp[6]*m->e[2] - tmp[9]*m->e[3];

	res[6]=		tmp[3]*m->e[0] + tmp[6]*m->e[1] + tmp[11]*m->e[3]
					-	tmp[2]*m->e[0] - tmp[7]*m->e[1] - tmp[10]*m->e[3];

	res[7]=		tmp[4]*m->e[0] + tmp[9]*m->e[1] + tmp[10]*m->e[2]
					-	tmp[5]*m->e[0] - tmp[8]*m->e[1] - tmp[11]*m->e[2];

	//calculate pairs for second 8 elements (cofactors)
	tmp[0] = m->e[2]*m->e[7];
	tmp[1] = m->e[3]*m->e[6];
	tmp[2] = m->e[1]*m->e[7];
	tmp[3] = m->e[3]*m->e[5];
	tmp[4] = m->e[1]*m->e[6];
	tmp[5] = m->e[2]*m->e[5];
	tmp[6] = m->e[0]*m->e[7];
	tmp[7] = m->e[3]*m->e[4];
	tmp[8] = m->e[0]*m->e[6];
	tmp[9] = m->e[2]*m->e[4];
	tmp[10] = m->e[0]*m->e[5];
	tmp[11] = m->e[1]*m->e[4];

	//calculate second 8 elements (cofactors)
	res[8]=		tmp[0]*m->e[13] + tmp[3]*m->e[14] + tmp[4]*m->e[15]
					-	tmp[1]*m->e[13] - tmp[2]*m->e[14] - tmp[5]*m->e[15];

	res[9]=		tmp[1]*m->e[12] + tmp[6]*m->e[14] + tmp[9]*m->e[15]
					-	tmp[0]*m->e[12] - tmp[7]*m->e[14] - tmp[8]*m->e[15];

	res[10]=		tmp[2]*m->e[12] + tmp[7]*m->e[13] + tmp[10]*m->e[15]
					-	tmp[3]*m->e[12] - tmp[6]*m->e[13] - tmp[11]*m->e[15];

	res[11]=		tmp[5]*m->e[12] + tmp[8]*m->e[13] + tmp[11]*m->e[14]
					-	tmp[4]*m->e[12] - tmp[9]*m->e[13] - tmp[10]*m->e[14];

	res[12]=		tmp[2]*m->e[10] + tmp[5]*m->e[11] + tmp[1]*m->e[9]
					-	tmp[4]*m->e[11] - tmp[0]*m->e[9] - tmp[3]*m->e[10];

	res[13]=		tmp[8]*m->e[11] + tmp[0]*m->e[8] + tmp[7]*m->e[10]
					-	tmp[6]*m->e[10] - tmp[9]*m->e[11] - tmp[1]*m->e[8];

	res[14]=		tmp[6]*m->e[9] + tmp[11]*m->e[11] + tmp[3]*m->e[8]
					-	tmp[10]*m->e[11] - tmp[2]*m->e[8] - tmp[7]*m->e[9];

	res[15]=		tmp[10]*m->e[10] + tmp[4]*m->e[8] + tmp[9]*m->e[9]
					-	tmp[8]*m->e[9] - tmp[11]*m->e[10] - tmp[5]*m->e[8];

	// calculate determinant
	det	=	 m->e[0]*res[0]
			+m->e[1]*res[1]
			+m->e[2]*res[2]
			+m->e[3]*res[3];

	if(det==0.0f)//Matrix is not invertible
	{
		return 0;
	}
	
	result->e[0] = res[0]/det;
	result->e[1] = res[1]/det;
	result->e[2] = res[2]/det;
	result->e[3] = res[3]/det;
	result->e[4] = res[4]/det;
	result->e[5] = res[5]/det;
	result->e[6] = res[6]/det;
	result->e[7] = res[7]/det;
	result->e[8] = res[8]/det;
	result->e[9] = res[9]/det;
	result->e[10] = res[10]/det;
	result->e[11] = res[11]/det;
	result->e[12] = res[12]/det;
	result->e[13] = res[13]/det;
	result->e[14] = res[14]/det;
	result->e[15] = res[15]/det;

	return 1;
}

int f_mat4_invert(const f_mat4 * m, f_mat4 * result)
{
	float tmp[12];												//temporary pair storage
	float res[16];
	float det;													//determinant

	//calculate pairs for first 8 elements (cofactors
	tmp[0] = m->e[10] * m->e[15];
	tmp[1] = m->e[11] * m->e[14];
	tmp[2] = m->e[9] * m->e[15];
	tmp[3] = m->e[11] * m->e[13];
	tmp[4] = m->e[9] * m->e[14];
	tmp[5] = m->e[10] * m->e[13];
	tmp[6] = m->e[8] * m->e[15];
	tmp[7] = m->e[11] * m->e[12];
	tmp[8] = m->e[8] * m->e[14];
	tmp[9] = m->e[10] * m->e[12];
	tmp[10] = m->e[8] * m->e[13];
	tmp[11] = m->e[9] * m->e[12];

	//calculate first 8 elements (cofactors)
	res[0]=		tmp[0]*m->e[5] + tmp[3]*m->e[6] + tmp[4]*m->e[7]
					-	tmp[1]*m->e[5] - tmp[2]*m->e[6] - tmp[5]*m->e[7];

	res[1]=		tmp[1]*m->e[4] + tmp[6]*m->e[6] + tmp[9]*m->e[7]
					-	tmp[0]*m->e[4] - tmp[7]*m->e[6] - tmp[8]*m->e[7];

	res[2]=		tmp[2]*m->e[4] + tmp[7]*m->e[5] + tmp[10]*m->e[7]
					-	tmp[3]*m->e[4] - tmp[6]*m->e[5] - tmp[11]*m->e[7];

	res[3]=		tmp[5]*m->e[4] + tmp[8]*m->e[5] + tmp[11]*m->e[6]
					-	tmp[4]*m->e[4] - tmp[9]*m->e[5] - tmp[10]*m->e[6];

	res[4]=		tmp[1]*m->e[1] + tmp[2]*m->e[2] + tmp[5]*m->e[3]
					-	tmp[0]*m->e[1] - tmp[3]*m->e[2] - tmp[4]*m->e[3];

	res[5]=		tmp[0]*m->e[0] + tmp[7]*m->e[2] + tmp[8]*m->e[3]
					-	tmp[1]*m->e[0] - tmp[6]*m->e[2] - tmp[9]*m->e[3];

	res[6]=		tmp[3]*m->e[0] + tmp[6]*m->e[1] + tmp[11]*m->e[3]
					-	tmp[2]*m->e[0] - tmp[7]*m->e[1] - tmp[10]*m->e[3];

	res[7]=		tmp[4]*m->e[0] + tmp[9]*m->e[1] + tmp[10]*m->e[2]
					-	tmp[5]*m->e[0] - tmp[8]*m->e[1] - tmp[11]*m->e[2];

	//calculate pairs for second 8 elements (cofactors)
	tmp[0] = m->e[2]*m->e[7];
	tmp[1] = m->e[3]*m->e[6];
	tmp[2] = m->e[1]*m->e[7];
	tmp[3] = m->e[3]*m->e[5];
	tmp[4] = m->e[1]*m->e[6];
	tmp[5] = m->e[2]*m->e[5];
	tmp[6] = m->e[0]*m->e[7];
	tmp[7] = m->e[3]*m->e[4];
	tmp[8] = m->e[0]*m->e[6];
	tmp[9] = m->e[2]*m->e[4];
	tmp[10] = m->e[0]*m->e[5];
	tmp[11] = m->e[1]*m->e[4];

	//calculate second 8 elements (cofactors)
	res[8]=		tmp[0]*m->e[13] + tmp[3]*m->e[14] + tmp[4]*m->e[15]
					-	tmp[1]*m->e[13] - tmp[2]*m->e[14] - tmp[5]*m->e[15];

	res[9]=		tmp[1]*m->e[12] + tmp[6]*m->e[14] + tmp[9]*m->e[15]
					-	tmp[0]*m->e[12] - tmp[7]*m->e[14] - tmp[8]*m->e[15];

	res[10]=		tmp[2]*m->e[12] + tmp[7]*m->e[13] + tmp[10]*m->e[15]
					-	tmp[3]*m->e[12] - tmp[6]*m->e[13] - tmp[11]*m->e[15];

	res[11]=		tmp[5]*m->e[12] + tmp[8]*m->e[13] + tmp[11]*m->e[14]
					-	tmp[4]*m->e[12] - tmp[9]*m->e[13] - tmp[10]*m->e[14];

	res[12]=		tmp[2]*m->e[10] + tmp[5]*m->e[11] + tmp[1]*m->e[9]
					-	tmp[4]*m->e[11] - tmp[0]*m->e[9] - tmp[3]*m->e[10];

	res[13]=		tmp[8]*m->e[11] + tmp[0]*m->e[8] + tmp[7]*m->e[10]
					-	tmp[6]*m->e[10] - tmp[9]*m->e[11] - tmp[1]*m->e[8];

	res[14]=		tmp[6]*m->e[9] + tmp[11]*m->e[11] + tmp[3]*m->e[8]
					-	tmp[10]*m->e[11] - tmp[2]*m->e[8] - tmp[7]*m->e[9];

	res[15]=		tmp[10]*m->e[10] + tmp[4]*m->e[8] + tmp[9]*m->e[9]
					-	tmp[8]*m->e[9] - tmp[11]*m->e[10] - tmp[5]*m->e[8];

	// calculate determinant
	det	=	 m->e[0]*res[0]
			+m->e[1]*res[1]
			+m->e[2]*res[2]
			+m->e[3]*res[3];

	if(det==0.0f)//Matrix is not invertible
	{
		return 0;
	}
	
	result->e[0] = res[0]/det;
	result->e[1] = res[4]/det;
	result->e[2] = res[8]/det;
	result->e[3] = res[12]/det;
	result->e[4] = res[1]/det;
	result->e[5] = res[5]/det;
	result->e[6] = res[9]/det;
	result->e[7] = res[13]/det;
	result->e[8] = res[2]/det;
	result->e[9] = res[6]/det;
	result->e[10] = res[10]/det;
	result->e[11] = res[14]/det;
	result->e[12] = res[3]/det;
	result->e[13] = res[7]/det;
	result->e[14] = res[11]/det;
	result->e[15] = res[15]/det;

	return 1;
}

void f_mat4_transpose(const f_mat4 * m, f_mat4 * result)
{
	f_mat4 tmp = *m;
	//m->e[0] = tmp.e[0]; 
	tmp.e[4] = m->e[1];
	tmp.e[8] = m->e[2]; 
	tmp.e[12] = m->e[3]; 

	tmp.e[1] = m->e[4]; 
	//m->e[5] = tmp.e[5]; 
	tmp.e[9] = m->e[6]; 
	tmp.e[13] = m->e[7]; 

	tmp.e[2] = m->e[8]; 
	tmp.e[6] = m->e[9]; 
	//m->e[10] = tmp.e[10]; 
	tmp.e[14] =m->e[11]; 

	tmp.e[3] = m->e[12]; 
	tmp.e[7] = m->e[13]; 
	tmp.e[11] = m->e[14]; 
	//m->e[15] = tmp.e[15]; 

	*result = tmp;
}

/* ray casting */
int f_ray_sphere(const f_ray* ray, float sphere_radius,
					f_vec3 * hit_pos, f_vec3 * hit_normal)
{
	/* calculate base coefficents */
	float a = f_vec3_dot(&ray->v, &ray->v);
	float b = 2*f_vec3_dot(&ray->v, &ray->s);
	float c = f_vec3_dot(&ray->s, &ray->s) - sphere_radius*sphere_radius;	

	/* calculate discriminant */
	float D = b*b - 4*a*c;
	if(D < 0)/* no intersection */
		return 0;
	
	/* calculate t */
	float t = (-b-sqrt(D))/(2*a);

	/* calculate nearest point of intersection */
	f_vec3 r;
	f_vec3_mult(&ray->v, t, &r);
	f_vec3_add(&ray->s, &r, hit_pos);
	
	/* calculate normal */
	*hit_normal = *hit_pos;
	f_vec3_normalize(hit_normal);

	return 1;
}

void f_ray_transform(const f_ray * ray, const f_mat4 * mat, f_ray * ray_trans)
{
	f_mat4_multv(mat, &ray->s, 1.0f, &ray_trans->s);
	f_mat4_multv(mat, &ray->v, 0.0f, &ray_trans->v);
}

int f_ray_planeXY(const f_ray* ray, float r_x, float r_y, f_vec3 * hit_pos)
{
	if(ray->v.z < 0 && ray->s.z >= 0)/* collision only possible if ray comes from above */
	{
		float t = ray->s.z/(-ray->v.z);
		f_vec3_mult(&ray->v, t, hit_pos);
		f_vec3_add(&ray->s, hit_pos, hit_pos);
		/* check wether point lies in plane */	
		if(hit_pos->x >= 0 && hit_pos->x <= r_x &&
			hit_pos->y >= 0 && hit_pos->y <= r_y)
			return 1;
	}
	return 0;
}

int f_ray_planeXZ(const f_ray* ray, float r_x, float r_z, f_vec3 * hit_pos)
{
	if(ray->v.y < 0 && ray->s.y >= 0)/* collision only possible if ray comes from above */
	{
		float t = ray->s.y/(-ray->v.y);
		f_vec3_mult(&ray->v, t, hit_pos);
		f_vec3_add(&ray->s, hit_pos, hit_pos);
		/* check wether point lies in plane */	
		if(hit_pos->x >= 0 && hit_pos->x <= r_x &&
			hit_pos->z >= 0 && hit_pos->z <= r_z)
			return 1;
	}
	return 0;
}

int f_ray_planeYZ(const f_ray* ray, float r_y, float r_z, f_vec3 * hit_pos)
{
	if(ray->v.x < 0 && ray->s.x >= 0)/* collision only possible if ray comes from above */
	{
		float t = ray->s.x/(-ray->v.x);
		f_vec3_mult(&ray->v, t, hit_pos);
		f_vec3_add(&ray->s, hit_pos, hit_pos);
		/* check wether point lies in plane */	
		if(hit_pos->y >= 0 && hit_pos->y <= r_y &&
			hit_pos->z >= 0 && hit_pos->z <= r_z)
			return 1;
	}
	return 0;
}

int f_ray_box(const f_ray* ray, const f_vec3 * dim, f_vec3 * hit_pos, f_vec3 * hit_normal)
{
	f_ray ray_t = *ray;

	/* x- side */
	ray_t.s.x = -ray->s.x;
	ray_t.v.x = -ray->v.x;
	if(f_ray_planeYZ(&ray_t, dim->y, dim->z, hit_pos))
	{
		hit_normal->x = -1.f;
		hit_normal->y = 0.f;
		hit_normal->z = 0.f;
		return 1;
	}
	ray_t.s.x = ray->s.x;
	ray_t.v.x = ray->v.x;
	/* y- side */
	ray_t.s.y = -ray->s.y;
	ray_t.v.y = -ray->v.y;
	if(f_ray_planeXZ(&ray_t, dim->x, dim->z, hit_pos))
	{
		hit_normal->x = 0.f;
		hit_normal->y = -1.f;
		hit_normal->z = 0.f;
		return 1;
	}
	ray_t.s.y = ray->s.y;
	ray_t.v.y = ray->v.y;

	/* z- side */
	ray_t.s.z = -ray->s.z;
	ray_t.v.z = -ray->v.z;
	if(f_ray_planeXY(&ray_t, dim->x, dim->y, hit_pos))
	{
		hit_normal->x = 0.f;
		hit_normal->y = 0.f;
		hit_normal->z = -1.f;
		return 1;
	}
	ray_t.s.z = ray->s.z;
	ray_t.v.z = ray->v.z;

	/* x+ side */
	ray_t.s.x = ray->s.x-dim->x;
	if(f_ray_planeYZ(&ray_t, dim->y, dim->z, hit_pos))
	{
		hit_normal->x = 1.f;
		hit_normal->y = 0.f;
		hit_normal->z = 0.f;
		hit_pos->x = dim->x;
		return 1;
	}
	ray_t.s.x = ray->s.x;

	/* y+ side */
	ray_t.s.y = ray->s.y-dim->y;
	if(f_ray_planeXZ(&ray_t, dim->x, dim->z, hit_pos))
	{
		hit_normal->x = 0.f;
		hit_normal->y = 1.f;
		hit_normal->z = 0.f;
		hit_pos->y = dim->y;
		return 1;
	}
	ray_t.s.y = ray->s.y;

	/* z+ side */
	ray_t.s.z = ray->s.z-dim->z;
	if(f_ray_planeXY(&ray_t, dim->x, dim->y, hit_pos))
	{
		hit_normal->x = 0.f;
		hit_normal->y = 0.f;
		hit_normal->z = 1.f;
		hit_pos->z = dim->z;
		return 1;
	}

	return 0;
}

void f_ray_diffuseReflect(const f_vec3 * n, f_vec3 * out, int rand_value1, int rand_value2, int rand_max)
{
	/* calculate 2 perpendicular vectors on n */
	f_vec3 perp1;
	f_vec3 perp2;
	if(n->x != 0.f || n->y != 0.f)
	{
		perp1.x = n->y;
		perp1.y = -n->x;
		perp1.z = 0.f;
	}
	else
	{
		perp1.x = 1.f;
		perp1.y = 0.f;
		perp1.z = 0.f;
	}
	f_vec3_cross(n, &perp1, &perp2);

	/* a higher radius is more likely, angle is equally distributed */
	float r1 = rand_value1/((float)rand_max);
	float r2 = rand_value2/((float)rand_max);
	float z = r1; 
	float theta = 2*F_PI*r2;
	float z_pow_2 = z*z;
	float perp1_ratio = sqrt(1-z_pow_2)*cos(theta);
	float perp2_ratio = sqrt(1-z_pow_2)*sin(theta);
	float n_ratio = z;
	
	/* scaling orthonormal system */
	f_vec3_mult(&perp1, perp1_ratio, &perp1);
	f_vec3_mult(&perp2, perp2_ratio, &perp2);
	f_vec3_mult(n, n_ratio, out);

	/* adding it all together */
	f_vec3_add(out, &perp1, out);
	f_vec3_add(out, &perp2, out);
	f_vec3_normalize(out);
}
