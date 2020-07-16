# makefile RNVS Block 4

STD = -std=gnu11
CFLAGS = -g -Wall -Wextra -pedantic-errors $(STD)
COMPILE = gcc $(CFLAGS) $^ -o $@

BINARIES = peer peer128

.PHONY: clean

all: $(BINARIES)

peer: peer.c hashmap.c protokoll.c connection.c fingertable.c
	$(COMPILE)

peer128: peer.c hashmap.c protokoll.c connection.c fingertable.c
	$(COMPILE) -DHASHSIZE=128

clean:
	\rm -rf $(BINARIES)
