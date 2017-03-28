CC = gcc
CFLAGS = -g -std=c99 -Wall
CFLAGS_VERBOSE = -Wextra -pedantic

DIRECTIVES =

TARGETS = $(wildcard *.c)
OUTPUT  = emulator

all: build

verbose:
	$(eval CFLAGS += $(CFLAGS_VERBOSE))

debug:
	$(eval DIRECTIVES += -DDEBUG)

log:
	$(eval DIRECTIVES += -DDOLOG)

withtest:
	$(eval OUTPUT := test)
	$(eval DIRECTIVES += -DTEST)

build:
	$(CC) $(CFLAGS) $(DIRECTIVES) -o $(OUTPUT) $(TARGETS)

run: build
	./peppercorn $(ARGS)

test: withtest build
	./test

runtestval: withtest build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test

clean:
	rm wildfire test
