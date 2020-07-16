# makefile RNVS Block 6a

STD = -std=gnu11
CFLAGS = -g -Wall -Wextra -pedantic-errors $(STD)
COMPILE = gcc $(CFLAGS) $^ -o $@

BINARIES = ntpclient

.PHONY: clean

all: $(BINARIES)

ntpclient: ntpclient.c ntprotocol.c
	$(COMPILE)

clean:
	\rm -rf $(BINARIES)
