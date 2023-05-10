CC=clang
INCLUDE_DIRS=-I.
OPTIMIZATION_LEVEL=-O0
CFLAGS=-Wall -Wextra -g $(INCLUDE_DIRS) $(OPTIMIZATION_LEVEL)

SOURCE_FILE_PATHS=$(wildcard src/*.c)
HEADER_FILE_PATHS=$(wildcard src/*.h)
OBJECT_FILE_PATHS=$(subst %.c,%.o, $(SOURCE_FILE_PATHS))

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECT_FILE_PATHS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean

clean:
	rm -rf $(BINARY) *.o
