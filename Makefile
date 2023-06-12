all: clean main

CC = clang
CFLAGS = -g -Wno-everything -pthread -lm
FILES = main.c chess.c
OUTPUT = main

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main:
	$(CC) $(CFLAGS) $(FILES) -o $(OUTPUT)

clean:
	rm -f main