CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes
CFLAGS=-std=c17 -O2 -march=native $(ERRFLAGS)
BINARY_NAME=advent_c

OBJS = \
	src/2015/day_one.o \
	src/2015/day_two.o \
	src/2015/day_three.o \
	src/2015/day_four.o \
	src/2015/day_five.o \
	src/2015/day_six.o \
	src/2015/day_seven.o \
	src/2015/day_eight.o \
	src/2015/day_nine.o \
	src/2015/day_ten.o \
	src/2015/day_eleven.o \
	src/2015/day_twelve.o \
	src/2015/day_thirteen.o \
	src/utils/fnv.o \
	src/utils/md5.o \
	src/utils/minmax.o \
	src/utils/strings.o \
	src/main.o

HEADERS = \
	src/utils/fnv.h \
	src/utils/md5.h \
	src/utils/minmax.h \
	src/utils/string.h \
	src/macros.h

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY_NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)
