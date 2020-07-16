# makefile RNVS Block 2

.PHONY: clean

all: client server

client: client.c
	gcc -g -Wall -Wextra -pedantic-errors -std=gnu11 client.c -o client

server: server.c
	gcc -g -Wall -Wextra -pedantic-errors -std=gnu11 server.c -o server

clean:
	\rm -rf client server
