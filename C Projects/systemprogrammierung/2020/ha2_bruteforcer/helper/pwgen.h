#ifndef PWGEN_H
#define PWGEN_H

#include <stdlib.h>

typedef struct pwd {
	char *buf;
	int buflen;
} pwd;

pwd *new_password(int maxlen);
int next_password(pwd *thepwd);
void free_password(pwd *thepwd);

#endif
