CC = gcc
AR = ar
INCLUDES = ../include/

CFLAGS = -ggdb  -I$(INCLUDES)

all:
	$(CC) $(CFLAGS) -o test test.c ../src/shared.a
	

clean:
	rm -f test

