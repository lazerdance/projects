/*
Author: Cornelius Marx
Date: April 12th 2019
*/
#include "ray_default_scene.h"

ray_Scene* ray_createDefaultScene()
{
	/* instanciate colors */
    static const float sphere_color[3] = SCENE_SPHERE_COLOR;
	static const float cube_color[3] = SCENE_CUBE_COLOR;
	static const float top_plane_color[3] = SCENE_TOP_PLANE_COLOR;
	static const float bottom_plane_color[3] = SCENE_BOTTOM_PLANE_COLOR;
	static const float left_plane_color[3] = SCENE_LEFT_PLANE_COLOR;
	static const float right_plane_color[3] = SCENE_RIGHT_PLANE_COLOR;
	static const float back_plane_color[3] = SCENE_BACK_PLANE_COLOR;
	static const float front_plane_color[3] = SCENE_FRONT_PLANE_COLOR;
	static const float light_color[3] = SCENE_LIGHT_COLOR;
	static const float horizon_color[3] = SCENE_AMBIENT_COLOR; /* used as ambient color */

	ray_Scene * s = ray_createScene();
	s->horizon_color = *((f_vec3*)horizon_color);
	/* setting projection and its inverse */
	f_mat4 proj;
	f_mat4_setPerspectiveY(&proj, f_rad(SCENE_FOV_ANGLE), 1.0f, 0.1f, 10.0f);
	f_mat4_translate(&proj, 0.0f, 0.0f, -SCENE_CAMERA_DISTANCE);
	f_mat4 proj_inv;
	f_mat4_invert(&proj, &proj_inv);
	ray_setProjection(s, &proj, &proj_inv);

	/* adding objects */
	f_mat4 m;
	f_vec3 dim;
	ray_Material material;

	/* bottom plane */
	f_mat4_setTranslation(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE);
	f_vec3_set(&dim, SCENE_BOX_SIZE, 0.f, SCENE_BOX_SIZE);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)bottom_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_XZ, &dim);

	/* top plane */
	f_mat4_setIdentity(&m);
	f_mat4_scale(&m,1.f, -1.f, 1.f);
	f_mat4_translate(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE);
	f_vec3_set(&dim, SCENE_BOX_SIZE, 0.f, SCENE_BOX_SIZE);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)top_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_XZ, &dim);

	/* back plane */
	f_mat4_setTranslation(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE);
	f_vec3_set(&dim, SCENE_BOX_SIZE, SCENE_BOX_SIZE, 0.f);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)back_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_XY, &dim);

	/* front plane */
	f_mat4_setIdentity(&m);
	f_mat4_scale(&m, 1.f, 1.f, -1.f);
	f_mat4_translate(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, 0.f);
	f_vec3_set(&dim, SCENE_BOX_SIZE, SCENE_BOX_SIZE, 0.f);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)front_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_XY, &dim);

	/* left plane */
	f_mat4_setTranslation(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE);
	f_vec3_set(&dim, 0.f, SCENE_BOX_SIZE, SCENE_BOX_SIZE);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)left_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_YZ, &dim);

	/* right plane */
	f_mat4_setIdentity(&m);
	f_mat4_scale(&m, -1.f, 1.f, 1.f);
	f_mat4_translate(&m, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE/2.f, -SCENE_BOX_SIZE);
	f_vec3_set(&dim, 0.f, SCENE_BOX_SIZE, SCENE_BOX_SIZE);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)right_plane_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_PLANE_YZ, &dim);

	/* light */
	f_vec3 l_pos;
	l_pos.x = 0.f;
	l_pos.y = -SCENE_LIGHT_OFFSET+SCENE_BOX_SIZE/2.f;
	l_pos.z = -SCENE_BOX_SIZE/2.f;
	ray_addLight(s, &l_pos, (f_vec3*)light_color,
		SCENE_LIGHT_CONSTANT_ATTEN, SCENE_LIGHT_LINEAR_ATTEN, SCENE_LIGHT_QUADRIC_ATTEN);	

	/* sphere */
	f_mat4_setTranslation(&m, SCENE_SPHERE_X, SCENE_SPHERE_RADIUS-SCENE_BOX_SIZE/2.0f, -SCENE_BOX_SIZE/2.0f + SCENE_SPHERE_Z);
	f_vec3_set(&dim, SCENE_SPHERE_RADIUS, 0.f, 0.f);
	material.type = RAY_MATERIAL_DIFFUSE;
    material.color = *((f_vec3*)sphere_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_SPHERE, &dim);

	/* box */
	f_mat4_setTranslation(&m, -SCENE_CUBE_SIZE/2.0f + SCENE_CUBE_X, -SCENE_BOX_SIZE/2.0f, -SCENE_CUBE_SIZE/2.0f-SCENE_BOX_SIZE/2.0f + SCENE_CUBE_Z);
	f_vec3_set(&dim, SCENE_CUBE_SIZE, SCENE_CUBE_SIZE, SCENE_CUBE_SIZE);
	material.type = RAY_MATERIAL_DIFFUSE;
	material.color = *((f_vec3*)cube_color);
	ray_addObject(s, &m, &material, RAY_OBJECT_BOX, &dim);

	return s;
}
