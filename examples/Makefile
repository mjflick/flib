CC = gcc
INCLUDES = ../include/

OBJS = ../src/shared.a
CFLAGS = -ggdb  -I$(INCLUDES)

all: test simplesed

test:
	$(CC) $(CFLAGS) -o test test.c $(OBJS)

simplesed:
	$(CC) $(CFLAGS) -o simplesed simplesed.c $(OBJS)

clean:
	rm -f test
	rm -f simplesed
