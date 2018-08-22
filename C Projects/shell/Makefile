TARGET_SHELL = shell
TARGET_SPAM = spam

BIN = \
	$(TARGET_SHELL) \
	$(TARGET_SPAM)

SRC_SHELL = \
	shell.c \
	shell.h

SRC_SPAM = \
	spam.c

CC	     = gcc
CFLAGS   = -std=c99 -g -Wall -Wextra -O2

default: all

all: shell spam

shell: $(SRC_SHELL)
	$(CC) $(CFLAGS) shell.c -o $(TARGET_SHELL)

spam: $(SRC_SPAM)
	$(CC) $(CFLAGS) spam.c -o $(TARGET_SPAM)

run: $(BIN)
	./$(TARGET_SHELL)

debug: $(BIN)
	gdb ./$(TARGET_SHELL)

clean: $(BIN)
	rm $(TARGET_SHELL) $(TARGET_SPAM)
