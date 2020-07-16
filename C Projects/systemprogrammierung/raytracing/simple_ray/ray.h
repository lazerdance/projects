/*
Author: Cornelius Marx
Date: April 10th 2019
Description:
	Implements functions for rendering (ray tracing) geometric objects with different materials bundled in scenes
*/
#ifndef RAY_H
#define RAY_H

#include "ray_settings.h"
#include "ray_math.h"
#include "bmp.h"


/* object types */
#define RAY_OBJECT_PLANE_XY	0
#define RAY_OBJECT_PLANE_XZ	1
#define RAY_OBJECT_PLANE_YZ	2
#define RAY_OBJECT_BOX		3 
#define RAY_OBJECT_SPHERE	4

/* material types */
#define RAY_MATERIAL_DIFFUSE	0
#define RAY_MATERIAL_GLOSSY		1
#define RAY_MATERIAL_EMISSIVE	2

/* material */
typedef struct{
	int type; /* material type */
	f_vec3 color; /* material color, ignored for specular type,
					 strength of emission (for emissive type) is encoded in the length of the vector
					*/
}ray_Material;

typedef struct ray_Object ray_Object;
struct ray_Object{
	f_mat4 transform; /* world transform of the object */
	f_mat4 transform_inv; /* inverse world transform (from world to object space), needed for ray-test */
	ray_Material material; /* material of the object */
	int type; /* object type */
	f_vec3 dim; /* dimensions of the object, depending on type:
					PLANE: by setting one dimension to 0, object is either a XY, XZ or YZ plane
					BOX: dim describes the width, height and depth of the box 
					SPHERE: dim.x is the radius of the sphere
				*/
	ray_Object * next; /* next object in linked list */
};

/* point light source */
typedef struct ray_Light ray_Light;
struct ray_Light{
	f_vec3 pos; /* light position */
	f_vec3 color; /* light color */
	/* attenuation */
	float constant_atten;
	float linear_atten;
	float quadric_atten;
	ray_Light * next;
};

/* ray_Scene describing projection and objects in scene */
typedef struct{
	ray_Object * objects; /* single linked list of objects */
	int num_objects;
	ray_Light * lights;
	int num_lights;
	f_mat4 projection_mat; /* projection matrix determines how scene is projected onto image */ 
	f_mat4 projection_mat_inv; /* inverse projection matrix */
	f_vec3 horizon_color;
}ray_Scene;

typedef struct{
	ray_Object * object;
	f_vec3 hit_pos;	
	f_vec3 hit_normal;
}ray_bounceStage;

/* render part of a given scene to an image with given resolution and number of samples
	canvas: determines what part of the complete image (resolution_w, resolution_h) should be rendered;
			if the rect specified by canvas lies completely in the total image's area, the image returned has the dimensions canvas->w, canvas->h;
			when passing NULL, the complete image (resolution_w, resolution_h) will be rendered
	resolution_w, resolution_h: resolution of the complete image
	samples: number of samples rendered per pixel;
			 a higher value corresponds to more computing time but results in a cleaner image
	scene: the scene to render
	return: the rendered image with dimensions canvas->w, canvas->h (or resolution_w, resolution_h when no canvas specified); NULL is returned if the specified rect (canvas) has no intersection with the total image
	process_name: name of the rendering process, used for status print. If set to NULL, no status is print
		NOTE: use bmp_free(bmp_Image*) to delete the image returned
*/
bmp_Image* ray_renderScene(const bmp_Rect * canvas, int resolution_w, int resolution_h,
							int samples, const ray_Scene* scene, const char * process_name);

/* render one sample with indirect lighting (slow and noisy but accurate), not used anymore (too noisy)
*/
int ray_raySample(f_ray ray, const ray_Scene * scene, f_vec3 * color);

/* render one sample with direct lighting (fast and clean but does not calculate global illumination) */
void ray_raySampleDirect(f_ray ray, const ray_Scene * scene, f_vec3 * color);

/* casting ray through scene, detecting nearest object,
	tabu_object will be excluded from testing
 */
ray_Object* ray_rayCast(const f_ray *ray, const ray_Scene * scene, f_vec3 * hit_pos, f_vec3 * hit_normal, const ray_Object * tabu_object);

/* create a new scene
	return: scene that was created; use ray_freeScene(ray_Scene*) to delete that scene
*/
ray_Scene* ray_createScene();

/* adding an object to a given scene;
	the object is constructed from the given parameters;
	object will be inserted at the beginning of the list
*/
void ray_addObject(ray_Scene * scene, const f_mat4* transform, const ray_Material* material, 
					int type, const f_vec3* dim);

/* adding light source to a given scene;
	light will be inserted at the beginning of the list
*/
void ray_addLight(ray_Scene * scene, const f_vec3 * pos, const f_vec3 * color,
					float const_atten, float linear_atten, float quadric_atten);

/* set the projection and its matrix inverse of a given scene */
void ray_setProjection(ray_Scene * scene, const f_mat4 * proj, const f_mat4 * proj_inv);

/* freeing scene and all of its objects */
void ray_freeScene(ray_Scene * scene);

#endif
