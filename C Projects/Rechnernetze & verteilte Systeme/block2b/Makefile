# makefile RNVS Block 2

.PHONY: clean

all: client clientudp clienthttp

client: client.c
	gcc -g -Wall -Wextra -pedantic-errors -std=gnu11 client.c -o client

clientudp: clientudp.c
	gcc -g -Wall -Wextra -pedantic-errors -std=gnu11 clientudp.c -o clientudp

clienthttp: clienthttp.c
	gcc -g -Wall -Wextra -pedantic-errors -std=gnu11 clienthttp.c -o clienthttp

clean:
	\rm -rf client clientudp clienthttp
