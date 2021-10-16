CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes
CFLAGS=-std=c17 -O2 -g -march=native $(ERRFLAGS)
BINARY_NAME=advent_c

OBJS = \
	src/utils/bitset.o \
	src/utils/md5.o \
	src/utils/minmax.o \
	src/utils/rand.o \
	src/utils/strings.o \
	src/utils/vector.c \
	src/utils/xxhash.o \
	src/main.o

HEADERS = \
	src/utils/md5.h \
	src/utils/minmax.h \
	src/utils/rand.h \
	src/utils/strings.h \
	src/utils/vector.h \
	src/common.h \
	src/macros.h

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
