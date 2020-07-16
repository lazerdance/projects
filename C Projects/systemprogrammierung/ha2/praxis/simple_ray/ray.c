/*
Author: Cornelius Marx
Date: April 10th 2019
*/
#include "ray.h"

bmp_Image* ray_renderScene(const bmp_Rect * canvas, int resolution_w, int resolution_h,
							int samples, const ray_Scene* scene, const char * process_name)
{
	if(scene == NULL)
		return NULL;
	int x_start = 0;
	int x_end = resolution_w;
	int y_start = 0;
	int y_end = resolution_h;
	if(canvas != NULL)/* canvas specified */
	{
		x_start = (canvas->x < 0) ? 0 : canvas->x;
		y_start = (canvas->y < 0) ? 0 : canvas->y;
		x_end = (canvas->x+canvas->w > resolution_w) ? resolution_w : canvas->x+canvas->w;
		y_end = (canvas->y+canvas->h > resolution_h) ? resolution_h : canvas->y+canvas->h;
	}
	/* calculate image dimensions */
	int image_w = x_end-x_start;
	int image_h = y_end-y_start;
	if(image_w <= 0 || image_h <= 0)/* no intersection with total image */
		return NULL;

	/* create empty image */
	bmp_Image * img = bmp_loadFromData(image_w, image_h, NULL);
	/* 	for each pixel:
			for each sample:
				-> choose random spot that lies in current pixel, send ray along camera frustum
				-> sum up color returned	
			-> average colors of all samples (sum)
	*/

	int percent_done = 0;
	if(process_name != NULL)
		printf("%s: 0%%\n", process_name);
	int img_y = 0;
	for(int y = y_start; y < y_end; y++)
	{
		int img_x = 0;
		for(int x = x_start; x < x_end; x++)
		{
            srand(RAY_NOISE_SEED);
			f_vec3 pixel_color;
			pixel_color.x = 0.f;
			pixel_color.y = 0.f;
			pixel_color.z = 0.f;
			for(int i = 0; i < samples; i++)
			{
				/* choose random sample in image coordinates and transform to normalized projection */
				f_ray sample_ray;
				sample_ray.s.x = (rand()%(RAY_SAMPLE_FRACTION+1))/((float)RAY_SAMPLE_FRACTION) - 1.0f;
				sample_ray.s.y = (rand()%(RAY_SAMPLE_FRACTION+1))/((float)RAY_SAMPLE_FRACTION) - 1.0f;

				sample_ray.s.x += x+0.5f;
				sample_ray.s.x = 2.0f*(sample_ray.s.x/resolution_w) - 1.0f;
				sample_ray.s.y += y+0.5f;	
				sample_ray.s.y = 2.0f*(-sample_ray.s.y/resolution_h) + 1.0f;

				sample_ray.s.z = -1.0f;/* ray starts on near-plane */
				sample_ray.v.x = sample_ray.s.x;
				sample_ray.v.y = sample_ray.s.y;
				sample_ray.v.z = 1.0f;/* ray ends on far-plane */
				/* transform sample ray in world space */
				float s_w = f_mat4_multv(&scene->projection_mat_inv, &sample_ray.s, 1.0f, &sample_ray.s);
				float v_w = f_mat4_multv(&scene->projection_mat_inv, &sample_ray.v, 1.0f, &sample_ray.v);
				f_vec3_div(&sample_ray.s, s_w, &sample_ray.s);
				f_vec3_div(&sample_ray.v, v_w, &sample_ray.v);
				f_vec3 sample_color = pixel_color;
				/* raycast */
				ray_raySampleDirect(sample_ray, scene, &sample_color);
				/***********/
				if(sample_color.x > 1.0f)
					sample_color.x = 1.0f;
				if(sample_color.y > 1.0f)
					sample_color.y = 1.0f;
				if(sample_color.z > 1.0f)
					sample_color.z = 1.0f;

				/* sum up sample color */
				f_vec3_add(&pixel_color, &sample_color, &pixel_color);
				/* */
			}
			/* averaging */
			f_vec3_div(&pixel_color, samples, &pixel_color);
			/* applying color ramp */
			float color_width = RAY_COLOR_RAMP_WHITE-RAY_COLOR_RAMP_BLACK;
			pixel_color.x = (pixel_color.x-RAY_COLOR_RAMP_BLACK)/color_width;
			pixel_color.y = (pixel_color.y-RAY_COLOR_RAMP_BLACK)/color_width;
			pixel_color.z = (pixel_color.z-RAY_COLOR_RAMP_BLACK)/color_width;
			
			/* setting final color in image */	
			Color final_color = bmp_getRGBColorf(pixel_color.x, pixel_color.y, pixel_color.z);
			int index = bmp_getIndex(img_x, img_y, img);
			img->pixels[index] = final_color;

			if(process_name != NULL)
			{
				int new_percent = (100*(img_y*image_w + img_x))/(image_h*image_w-1);
				if(new_percent != percent_done)
				{
					printf("%s: %d%%\n", process_name, new_percent);
					percent_done = new_percent;
				}
			}
			img_x++;
		}
		img_y++;
	}
	return img;
}

int ray_raySample(f_ray ray, const ray_Scene * scene, f_vec3 * color)
{
	*color = scene->horizon_color;
	ray_bounceStage stages[RAY_NUM_BOUNCES];
	memset(stages, 0, sizeof(ray_bounceStage)*RAY_NUM_BOUNCES);
	ray_Object * tabu_object = NULL; /* tabu object for each bounce, to prevent self-reflection */
	int i = 0;
	int color_set = 0;
	while(i < RAY_NUM_BOUNCES)
	{
		/* check collision with every object, take nearest */
		ray_Object * o = scene->objects;
		float nearest = INFINITY;
		ray_Object * hit_object = NULL;
		f_vec3 hit_pos;
		f_vec3 hit_normal;
		f_ray ray_trans;
		while(o != NULL)
		{
			if(o == tabu_object)
			{
				o = o->next;
				continue;
			}
			f_vec3 o_hit_pos;
			f_vec3 o_hit_normal;
			/* transform ray to current object space */
			f_ray_transform(&ray, &o->transform_inv, &ray_trans);
			int o_hit = 0;
			switch(o->type)
			{
			case RAY_OBJECT_SPHERE:
			{
				o_hit = f_ray_sphere(&ray_trans, o->dim.x, &o_hit_pos, &o_hit_normal);
			}break;
			case RAY_OBJECT_BOX:
			{
				o_hit = f_ray_box(&ray_trans, &o->dim, &o_hit_pos, &o_hit_normal);
			}break;
			case RAY_OBJECT_PLANE_XY:
			{
				o_hit = f_ray_planeXY(&ray_trans, o->dim.x, o->dim.y, &o_hit_pos);
				o_hit_normal.x = 0.f;
				o_hit_normal.y = 0.f;
				o_hit_normal.z = 1.f;
			}break;
			case RAY_OBJECT_PLANE_XZ:
			{
				o_hit = f_ray_planeXZ(&ray_trans, o->dim.x, o->dim.z, &o_hit_pos);
				o_hit_normal.x = 0.f;
				o_hit_normal.y = 1.f;
				o_hit_normal.z = 0.f;
			}break;
			case RAY_OBJECT_PLANE_YZ:
			{
				o_hit = f_ray_planeYZ(&ray_trans, o->dim.y, o->dim.z, &o_hit_pos);
				o_hit_normal.x = 1.f;
				o_hit_normal.y = 0.f;
				o_hit_normal.z = 0.f;
			}break;
			default:
			{
				/* unknown object type */
			}
			}
			if(o_hit)
			{
				f_vec3 ray_l;
				/* transform back into world space */
				f_mat4_multv(&o->transform, &o_hit_pos, 1.0f, &o_hit_pos);
				f_vec3_sub(&o_hit_pos, &ray.s, &ray_l);
				float ray_len = f_vec3_getLength(&ray_l);
				if(ray_len < nearest)
				{
					f_mat4_multv(&o->transform, &o_hit_normal, 0.0f, &hit_normal);
					nearest = ray_len;
					hit_pos = o_hit_pos;
					hit_object = o;
				}
			}
			o = o->next;
		}

		if(hit_object != NULL)
		{
			switch(hit_object->material.type)
			{
			case RAY_MATERIAL_EMISSIVE:
			{
				if(color_set)
					f_vec3_multv(color, &hit_object->material.color, color);
				else
					*color = hit_object->material.color;
				return i;
			}break;
			case RAY_MATERIAL_DIFFUSE:
			{
				/* diffuse reflect */
				int r1 = rand()%(RAY_SAMPLE_FRACTION+1);
				int r2 = rand()%(RAY_SAMPLE_FRACTION+1);
				f_ray_diffuseReflect(&hit_normal, &ray.v, r1, r2, RAY_SAMPLE_FRACTION);
				if(color_set == 0)
				{
					*color = hit_object->material.color;
					color_set = 1;
				}
				else
					f_vec3_multv(color, &hit_object->material.color, color);

				float dot = f_vec3_dot(&ray.v, &hit_normal);
				f_vec3_mult(color, dot, color);

			}break;
			case RAY_MATERIAL_GLOSSY:
			{
				/* hard reflection */
				f_vec3_reflect(&ray.v, &hit_normal, &ray.v);
				/* color stays the same */
			}break;
			default:
			{
			}break;
			}
			ray.s = hit_pos;
			stages[i].object = hit_object;	
			stages[i].hit_pos = hit_pos;
			stages[i].hit_normal = hit_normal;
			tabu_object = hit_object;
		}
		else
			break;

		i++;
	}
	return i;
}

void ray_raySampleDirect(f_ray ray, const ray_Scene * scene, f_vec3 * color)
{
	*color = scene->horizon_color;
	ray_Object * hit_object = NULL; /* tabu object for each bounce, to prevent self-reflection */
	int i = 0;
	f_vec3 hit_pos;
	f_vec3 hit_normal;
	while(1){
		if(i >= RAY_NUM_BOUNCES)
			return;
		hit_object = ray_rayCast(&ray, scene, &hit_pos, &hit_normal, hit_object);
		if(hit_object == NULL)
			return;
		f_vec3_reflect(&ray.v, &hit_normal, &ray.v);
		ray.s = hit_pos;
		if(hit_object->material.type != RAY_MATERIAL_GLOSSY) /* non glossy material found */
			break;
		i++;
	}
	switch(hit_object->material.type)
	{
	case RAY_MATERIAL_EMISSIVE:
	{
		*color = hit_object->material.color;
		return;
	}break;
	case RAY_MATERIAL_DIFFUSE:
	{
		ray_Light * l = scene->lights;
		color->x = 0.f;
		color->y = 0.f;
		color->z = 0.f;
		f_vec3 hit_light;
		float light_dist;
		while(l != NULL)
		{
			f_vec3_sub(&l->pos, &hit_pos, &hit_light);
			light_dist = f_vec3_getLength(&hit_light);
			f_vec3_div(&hit_light, light_dist, &ray.v);
			float dot = f_vec3_dot(&ray.v, &hit_normal);
			if(dot > 0.f)
			{
				f_vec3 light_hit_pos;
				ray_rayCast(&ray, scene, &light_hit_pos, NULL, hit_object);
				f_vec3_sub(&light_hit_pos, &hit_pos, &light_hit_pos);
				float light_strength = 1.0f/(l->constant_atten + l->linear_atten*light_dist + l->quadric_atten*light_dist*light_dist);
				if(light_dist <= f_vec3_getLength(&light_hit_pos))/* sample is in light */
				{
					f_vec3 diff_color;
					f_vec3_mult(&hit_object->material.color, dot*light_strength, &diff_color);
					f_vec3_multv(&diff_color, &l->color, &diff_color);
					f_vec3_add(&diff_color, color, color);
				}
			}
			l = l->next;
		}
		/* applying global horizon color */
		f_vec3 ambient;
		f_vec3_multv(&hit_object->material.color, &scene->horizon_color, &ambient);
		f_vec3_add(&ambient, color, color);
	}break;
	default:
	{
	}break;
	}
}
ray_Object* ray_rayCast(const f_ray *ray, const ray_Scene * scene, f_vec3 * hit_pos, f_vec3 * hit_normal, const ray_Object * tabu_object)
{
	ray_Object * o = scene->objects;
	float nearest = INFINITY;
	ray_Object * hit_object = NULL;
	f_ray ray_trans;
	while(o != NULL)
	{
		if(o == tabu_object)
		{
			o = o->next;
			continue;
		}
		f_vec3 o_hit_pos;
		f_vec3 o_hit_normal;
		/* transform ray to current object space */
		f_ray_transform(ray, &o->transform_inv, &ray_trans);
		int o_hit = 0;
		switch(o->type)
		{
		case RAY_OBJECT_SPHERE:
		{
			o_hit = f_ray_sphere(&ray_trans, o->dim.x, &o_hit_pos, &o_hit_normal);
		}break;
		case RAY_OBJECT_BOX:
		{
			o_hit = f_ray_box(&ray_trans, &o->dim, &o_hit_pos, &o_hit_normal);
		}break;
		case RAY_OBJECT_PLANE_XY:
		{
			o_hit = f_ray_planeXY(&ray_trans, o->dim.x, o->dim.y, &o_hit_pos);
			o_hit_normal.x = 0.f;
			o_hit_normal.y = 0.f;
			o_hit_normal.z = 1.f;
		}break;
		case RAY_OBJECT_PLANE_XZ:
		{
			o_hit = f_ray_planeXZ(&ray_trans, o->dim.x, o->dim.z, &o_hit_pos);
			o_hit_normal.x = 0.f;
			o_hit_normal.y = 1.f;
			o_hit_normal.z = 0.f;
		}break;
		case RAY_OBJECT_PLANE_YZ:
		{
			o_hit = f_ray_planeYZ(&ray_trans, o->dim.y, o->dim.z, &o_hit_pos);
			o_hit_normal.x = 1.f;
			o_hit_normal.y = 0.f;
			o_hit_normal.z = 0.f;
		}break;
		default:
		{
			/* unknown object type */
		}
		}
		if(o_hit)
		{
			f_vec3 ray_l;
			/* transform back into world space */
			f_mat4_multv(&o->transform, &o_hit_pos, 1.0f, &o_hit_pos);
			f_vec3_sub(&o_hit_pos, &ray->s, &ray_l);
			float ray_len = f_vec3_getLength(&ray_l);
			if(ray_len < nearest)
			{
				if(hit_normal != NULL)
					f_mat4_multv(&o->transform, &o_hit_normal, 0.0f, hit_normal);
				nearest = ray_len;
				*hit_pos = o_hit_pos;
				hit_object = o;
			}
		}
		o = o->next;
	}
	return hit_object;
}

ray_Scene* ray_createScene()
{
	ray_Scene * s = (ray_Scene*)malloc(sizeof(ray_Scene));
	f_vec3_set(&s->horizon_color, 0.f, 0.f, 0.f);
	s->objects = NULL;
	s->num_objects = 0;
	s->lights = NULL;
	s->num_lights = 0;
	return s;
}

void ray_addLight(ray_Scene * scene, const f_vec3 * pos, const f_vec3 * color,
					float const_atten, float linear_atten, float quadric_atten)
{
	ray_Light * light = (ray_Light*)malloc(sizeof(ray_Light));
	light->pos = *pos;
	light->color = *color;
	light->constant_atten = const_atten;
	light->linear_atten = linear_atten;
	light->quadric_atten = quadric_atten;
	light->next = scene->lights;
	scene->lights = light;
	scene->num_lights++;
}

void ray_addObject(ray_Scene * scene, const f_mat4* transform, const ray_Material* material, 
					int type, const f_vec3* dim)
{
	ray_Object * obj = (ray_Object*)malloc(sizeof(ray_Object));
	obj->transform = *transform;
	f_mat4_affineInvert(transform, &obj->transform_inv);
	obj->material = *material;
	obj->type = type;
	obj->dim = *dim;
	obj->next = scene->objects;
	scene->objects = obj;
	scene->num_objects++;
}

void ray_setProjection(ray_Scene * scene, const f_mat4 * projection, const f_mat4 * projection_inv)
{
	scene->projection_mat = *projection;
	scene->projection_mat_inv = *projection_inv;
}

void ray_freeScene(ray_Scene * scene)
{
	if(scene == NULL)
		return;
	/* freeing objects in linked list */
	ray_Object * o = scene->objects;
	while(o != NULL)
	{
		ray_Object * o_next = o->next;
		free(o);
		o = o_next;
	}

	/* freeing lights in linked list */
	ray_Light * l = scene->lights;
	while(l != NULL)
	{
		ray_Light * l_next = l->next;
		free(l);
		l = l_next;
	}

	free(scene);
}
