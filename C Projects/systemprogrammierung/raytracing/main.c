#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "simple_ray/ray_default_scene.h"
#include <sys/types.h>

// general variables
int p_num, res, samples, children, returnStatus, t,interval;
char title[50] = "";


void marshal(int pid, char *title){
	char file[50] = "file";
	char child_num[50];
	sprintf(child_num, "%d", pid); 
	strcat(file,child_num);
	strcat(file,".bmp");
	strcpy(title,file);
}

void handle_input(int argc, char** argv){
if(argc != 4){
		printf("der Aufruf muss so aussehen : ./raytracer <Anzahl der Prozesse> <Aufl¨osung> <Anzahl der Samples>\n");
		exit(0);
	}
	p_num = strtol(argv[1], NULL, 10);
	
	if(p_num < 1 || p_num > 128){
		printf("Anzahl der Prozessen muss zwischen 1 und 128 sein\n");
		exit(0);
	}
	
	res = strtol(argv[2], NULL, 10);
	if(res < 1 || res > 10000){
		printf("Auflösung muss zwischen 1 und 10000 sein\n");
		exit(0);
	}
	
	samples = strtol(argv[3], NULL, 10);
	if(samples < 1){
		printf("Minimum ein Sample\n");
		exit(0);
	}
	printf("Anzahl der Prozesse: %d\n", p_num);
	printf("Auflösung: %d\n", res);
	printf("Samples: %d\n", samples);
}

int main(int argc, char** argv)
{
	/* TODO: check and parse command line arguments */
	handle_input(argc, argv);
	/* initialize scene */
	ray_Scene* scene = ray_createDefaultScene();
	/* TODO: divide work load and create n child p_num */

	int count = p_num;
	pid_t child = 1;
	
	bmp_Rect bmp;
	bmp.h = res/p_num;
	bmp.w = res;
	bmp.x = 0;
	bmp.y = 0;

	bmp_Rect* p = &bmp;
	int rest=res%p_num;
	printf("rest is %d\n ", rest);
	while (count-- > 0 && child>0){
		// if ((child = fork()) > 0){
		// 	waitpid(child, &returnStatus, 0);  // Parent process waits here for child to terminate.
		if(rest!= 0 && count == 0){
			bmp.h = bmp.h + rest;
		}
		bmp.y = t;
		marshal(t,title);
		if((child = fork()) > 0){
			t = t + res/p_num;
			children++;
		}
		// }
		interval = children;
	}
	if(child != 0){
		sleep(1);
        printf("Parent %d spawned %d/%d children\n", getpid(), children, p_num);
		while(children-->0)
			waitpid(child, &returnStatus, 0);  // Parent process waits here for child to terminate.
	/* TODO: parent process loads all files and merges them into a single image */
	/* TODO: save final image to file "final.bmp" */
		if (returnStatus == 0){  // Verify child process terminated without error.  
			printf("Time to pack pieces together.\n");
			bmp_Image *final = bmp_loadFromData(res,res,NULL);
			for(int i=0; i<=res-(res/p_num); i=i+(res/p_num)){
				interval--;
				printf("**************************************** children %d", interval);
				printf("--------------------------------i is %d------------------", i);
				printf("reassembling the photo %d and p_num %d\n",i,p_num);
				marshal(i,title);
				bmp_Image *part = bmp_loadFromFile(title);
				bmp_stamp(part, final, 0, i);
				bmp_saveToFile(final, "final.bmp");			
				bmp_free(part);
				if (interval == 0) break;

			}
	/* free memory */
	/* TODO: make sure to free all allocated memory */
			bmp_free(final);
			ray_freeScene(scene);
		}
    }
	/* TODO: each child process renders it's part of the image and saves it to an individual file;
			 the parent process meanwhile waits for children to finish their work
	*/
	else{
			printf("Child %d, sired by %d\n", getpid(), getppid());
			bmp_Image *bi = ray_renderScene(p,res,res,samples, scene, "first image");
			bmp_saveToFile(bi, title);	
	/* free memory */
	/* TODO: make sure to free all allocated memory */			
			ray_freeScene(scene);
			bmp_free(bi);
	}	
	return 0;
}
