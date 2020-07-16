#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "simple_ray/ray_default_scene.h"

int main(int argc, char** argv)
{
	/* check and parse command line arguments */
	if( argc != 4 ){
		printf("Die Eingabe entspricht nicht der Vorgabe, es müssen 3 Parameter sein.\n");
		return 1;
	} 
	int threadCount = atoi(argv[1]);
	int resolution = atoi(argv[2]);
	int sampleRate = atoi(argv[3]);
	
	if ( threadCount < 1 || threadCount > 128 ){
		printf("Die Anzahl der Prozesse muss zwischen 0 und 129 liegen.\n");
		return 1;
	}else if ( resolution < 1 || resolution > 10000 ){
		printf("Die Auflösung muss zwischen 0 und 10001 liegen.\n");
		return 1;
	}else if ( sampleRate < 1 ){
		printf("Die Samplerate muss größer als 0 sein.");
		return 1;
	}else if ( threadCount > resolution )
	{
		perror("Die Anzahl der Prozesse darf nicht höher als die Auflösung sein.");
		exit(EXIT_FAILURE);
	}
	else{
		printf("Anzahl der Prozesse: %d\n",threadCount);
		printf("Auflösung: %dx%d\n",resolution,resolution);
		printf("Samples: %d\n", sampleRate);
	}

	/* initialize scene */
	ray_Scene* scene = ray_createDefaultScene();

	/* divide work load and create n child processes */
	//Rechnung anzahl der Streifen
	int breiteStreifen = breiteStreifen = resolution / threadCount;

	
	int y = 0;
	int h =	breiteStreifen;
	for( int i=0; i<threadCount; i++)
	{
		fflush(stdout);
		if( fork() == 0 ){ 
			bmp_Rect * teilBild = malloc(sizeof(bmp_Rect));
			teilBild->x = 0;
			teilBild->y = y;
			teilBild->w = resolution;
			teilBild->h = h;

			char processName[17];
			snprintf(processName, 17, "Process_%d", i);
			bmp_Image* image = ray_renderScene( teilBild, resolution, resolution, sampleRate, scene, processName );
			free(teilBild);
			
			strcat(processName, ".bmp");
			bmp_saveToFile( image, processName );
			exit(0);
		}
		y += breiteStreifen;
		if( i == threadCount-2 ){
			h += resolution % threadCount;
		}
	}

	/* each child process renders it's part of the image and saves it to an individual file;
			 the parent process meanwhile waits for children to finish their work
	*/

	/*  parent process loads all files and merges them into a single image */
	while ( wait(NULL)>0 );
	
	bmp_Image* neuesBild = bmp_loadFromData(resolution, resolution, NULL);
	for( int i = 0; i < threadCount; i++)
	{
		char processName[17];
		snprintf(processName, 17, "Process_%d.bmp", i);
		bmp_Image* teilBild = bmp_loadFromFile(processName);
		if(teilBild==NULL){
			printf("Name: %s\n",processName);
			perror("Konnte Teilbild nicht laden");
			exit(1);	
		}
		bmp_stamp(teilBild, neuesBild, 0, i*breiteStreifen);
		bmp_free(teilBild);
	}
	/* save final image to file "final.bmp" */
	bmp_saveToFile( neuesBild, "final.bmp"); 
	bmp_free(neuesBild);



	/* free memory */
	ray_freeScene(scene);
	/* make sure to free all allocated memory */
	
	return 0;
}
