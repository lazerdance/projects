all: bruteforcer.c libs/sha256ref/sha256ref.c libs/sha256.c helper/pwgen.c helper/hashloader.c
	gcc -std=c99 -Wall -o bruteforcer bruteforcer.c libs/sha256ref/sha256ref.c libs/sha256.c helper/pwgen.c helper/hashloader.c -g

clean:
	rm -rf bruteforcer bruteforcer.dSYM
