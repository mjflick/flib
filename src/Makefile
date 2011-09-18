CC = gcc
AR = ar
INCLUDES = ../include/

CFLAGS = -ggdb  -I$(INCLUDES)

OBJS = strstring.o fstring.o

all: shared

shared: $(OBJS)
	rm -f $@.a
	$(AR) q $@.a $(OBJS)

clean:
	rm -f *.o *~ \#* shared.a


