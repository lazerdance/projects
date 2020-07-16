# makefile RNVS Block 4

STD = -std=gnu11
CFLAGS = -g -Wall -Wextra -pedantic-errors $(STD)
COMPILE = gcc $(CFLAGS) $^ -o $@

BINARIES = peer

.PHONY: clean

all: $(BINARIES)

peer: peer.c hashmap.c protokoll.c connection.c list.c
	$(COMPILE) -DHASHSIZE=128

clean:
	\rm -rf $(BINARIES)
