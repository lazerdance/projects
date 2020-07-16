# makefile RNVS Block 3b

STD = -std=gnu11
CFLAGS = -g -Wall -Wextra -pedantic-errors $(STD)
COMPILE = gcc $(CFLAGS) $^ -o $@

BINARIES = client

.PHONY: clean

all: clean $(BINARIES)

client: client.c connection.c protokoll.c
	$(COMPILE)

clean:
	\rm -rf $(BINARIES)
