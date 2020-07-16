#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "simple_ray/ray_default_scene.h"

int main(int argc, char** argv)
{
	/* check and parse command line arguments */
	if (argc != 4) {
		printf("Syntax: ./raytracer <Anzahl der Prozesse> <Auflösung> <Anzahl der Samples>\n");
		exit(0);
	}

	int process_amount = strtol(argv[1], NULL, 10);
	if (process_amount < 1 || process_amount > 128) {
		printf("Die Anzahl der Prozesse muss zwischen 1 und 128 liegen.\n");
		exit(0);
	}

	int resolution = strtol(argv[2], NULL, 10);
	if (resolution < 1 || resolution > 10000) {
		printf("Die Auflösung muss zwischen 1 und 10000 liegen.\n");
		exit(0);
	}

	int sample_amount = strtol(argv[3], NULL, 10);
	if (sample_amount < 1) {
		printf("Die Anzahl an Samples muss mindestens 1 betragen.\n");
		exit(0);
	}

	printf("Anzahl der Prozesse: %d\n", process_amount);
	printf("Auflösung: %dx%d\n", resolution, resolution);
	printf("Samples: %d\n", sample_amount);

	/* initialize scene */
	ray_Scene* scene = ray_createDefaultScene();

	/* create n child processes */
	int process_id = 0;
	int forked[process_amount];
	for (process_id = 0; process_id < process_amount; process_id++) {
		forked[process_id] = fork();
		if (forked[process_id] == 0) {
			break; // we are a child process, we don't want to fork
		}
	}
	printf("Initialized process number %d.\n", process_id);

	/* each child process renders its part of the image and saves it to an individual file;
	   the parent process meanwhile waits for children to finish their work */
	if (process_id < process_amount) { // every child
		bmp_Rect *toRender = malloc(sizeof(bmp_Rect));
		toRender->x = 0;
		toRender->y = resolution * process_id / process_amount;
		toRender->w = resolution;
		toRender->h = resolution / process_amount;
		if ((resolution * (process_id + 1) / process_amount) > (toRender->y + toRender->h)) {
			// This is to handle cases where the resolution can't be equally devided on all processes.
			toRender->h++;
		}
		if (toRender->h == 0) {
			// An image with a height of 0 could lead to issues, so we'll skip that one and end this child's process.
			free(toRender);
			ray_freeScene(scene);
			exit(0);
		}
		// printf("toRender->x: %d\n", toRender->x);
		// printf("toRender->y: %d\n", toRender->y);
		// printf("toRender->w: %d\n", toRender->w);
		// printf("toRender->h: %d\n", toRender->h);
		char process_id_string[12];
		snprintf(process_id_string, sizeof(process_id_string), "Process %03d", process_id);
		bmp_Image *rendered_image = ray_renderScene(toRender, resolution, resolution, sample_amount, scene, process_id_string);
		free(toRender);
		char image_filename[14];
		snprintf(image_filename, sizeof(image_filename), "image_%d.bmp", process_id);
		if (bmp_saveToFile(rendered_image, image_filename) == 0) {
			printf("WARNING: FILE WRITE FAILED!\n\n");
			exit(1);
		}
		free(rendered_image->pixels);
		free(rendered_image);
	} else { // parent process merges all images into one final image
		for (int i = 0; i < process_amount; i++) {
			waitpid(forked[i], NULL, 0); // make sure all child processes have terminated
		}
		printf("All child processes have terminated, image packing started...\n");
		bmp_Image *final_image = bmp_loadFromData(resolution, resolution, NULL);
		int position_y = 0;
		char filename[14];
		bmp_Image *current;
		for (int i = 0; i < process_amount; i++) {
			if ((resolution * (i + 1) / process_amount) == position_y) {
				continue; // There is no file for this i, so we'll skip an iteration
			}
			snprintf(filename, sizeof(filename), "image_%d.bmp", i);
			current = bmp_loadFromFile(filename);
			if (current == NULL) { // shouldn't happen
				printf("is halt passiert \n\n\n");
			}
			bmp_stamp(current, final_image, 0, position_y);
			position_y = resolution * (i + 1) / process_amount;
			free(current->pixels); // probably not optimal doing it here, inside the loop
			free(current);

		}

		if (bmp_saveToFile(final_image, "final.bmp") == 0) {
			printf("WARNING: FINAL FILE WRITE FAILED!\n\n");
			exit(1);
		}
		free(final_image->pixels);
		free(final_image);
	}

	/* free memory */
	ray_freeScene(scene);

	return 0;
}
