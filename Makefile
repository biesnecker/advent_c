CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes -Wno-unused-function
CFLAGS=-std=c17 -O2
BINARY_NAME=advent_c

MAIN = src/main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(ERRFLAGS)

LIBS = \
	utils.a \
	lib2015.a \
	lib2016.a \
	lib2018.a \
	lib2021.a \
	lib2022.a

$(BINARY_NAME): $(MAIN) $(LIBS)
	$(CC) -o $@ $^ $(CFLAGS) $(ERRFLAGS)

utils.a: $(patsubst %.c, %.o, $(wildcard src/utils/*.c))
	ar -rv $@ $^

lib2015.a: $(patsubst %.c, %.o, $(wildcard src/2015/*.c))
	ar -rv $@ $^

lib2016.a: $(patsubst %.c, %.o, $(wildcard src/2016/*.c))
	ar -rv $@ $^

lib2018.a: $(patsubst %.c, %.o, $(wildcard src/2018/*.c))
	ar -rv $@ $^

lib2021.a: $(patsubst %.c, %.o, $(wildcard src/2021/*.c))
	ar -rv $@ $^

lib2022.a: $(patsubst %.c, %.o, $(wildcard src/2022/*.c))
	ar -rv $@ $^

.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)
