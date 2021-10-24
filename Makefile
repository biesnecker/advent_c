CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes
CFLAGS=-std=c17 -g --debug -O0 -march=native $(ERRFLAGS)
BINARY_NAME=advent_c

OBJS = \
	src/utils/bitset.o \
	src/utils/hashset.o \
	src/utils/heap.o \
	src/utils/md5.o \
	src/utils/minmax.o \
	src/utils/rand.o \
	src/utils/ringbuffer.o \
	src/utils/strings.o \
	src/utils/vector.c \
	src/utils/xxhash.o \
	src/main.o

HEADERS = $(wildcard src/*.h)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY_NAME): $(OBJS) lib2015.a lib2016.a
	$(CC) -o $@ $^ $(CFLAGS)

lib2015.a: $(patsubst %.c, %.o, $(wildcard src/2015/*.c))
	ar -rv $@ $^

lib2016.a: $(patsubst %.c, %.o, $(wildcard src/2016/*.c))
	ar -rv $@ $^

.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)
