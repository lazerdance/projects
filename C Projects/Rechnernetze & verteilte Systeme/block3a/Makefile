# makefile RNVS Block 3a

STD = -std=gnu11
CFLAGS = -g -Wall -Wextra -pedantic-errors $(STD)
COMPILE = gcc $(CFLAGS) $^ -o $@

BINARIES = server

.PHONY: clean

all: clean $(BINARIES)

server: server.c hashmap.c protokoll.c
	$(COMPILE)

test_hashmap: test_hashmap.c hashmap.c
	$(COMPILE)

clean:
	\rm -rf $(BINARIES)
