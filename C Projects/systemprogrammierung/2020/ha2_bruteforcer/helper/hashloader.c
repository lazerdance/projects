#include "hashloader.h"
FILE *open_file(char *filename);
void close_file(FILE *fd);

/*
 * Generiert ein hashes-struct, in das die Hashes geladen werden.
 * Das struct wird dynamisch erzeugt und muss später gefreet werden.
 */
hashes *load_hashes(char *filename) {
	hashes *loaded_hashes = malloc(sizeof(hashes));
	loaded_hashes->array = NULL;
	loaded_hashes->len = 0;
	
	char *buffer = NULL;
	size_t len = 0;
	int line_len;
	
	FILE *fd = open_file(filename);
	while ((line_len = getline(&buffer, &len, fd)) >= 0) {
		if (line_len > 0 && buffer[line_len-1] == '\n') {
			buffer[line_len-1] = '\0';
		}
		
		loaded_hashes->len++;
		loaded_hashes->array = realloc(loaded_hashes->array, loaded_hashes->len * sizeof(char *));
		
		loaded_hashes->array[loaded_hashes->len-1] = buffer;
		
		buffer = NULL;
		len = 0;
	}
	free(buffer);
	close_file(fd);
	
	return loaded_hashes;
}

/**
 * Befreit den kompletten Speicher des hashes structs
 */
void free_hashes(hashes *loaded_hashes) {
	for (int i = 0; i < loaded_hashes->len; i++) {
		free(loaded_hashes->array[i]);
	}
	free(loaded_hashes->array);
	free(loaded_hashes);
}

FILE *open_file(char *filename) {
	FILE *fd;
	
	if ((fd = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Konnte Datei %s nicht öffnen!\n", filename);
		exit(1);
	}
	
	return fd;
}

void close_file(FILE *fd) {
	fclose(fd);
}
