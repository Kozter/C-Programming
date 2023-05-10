# Makefile template

INCLDIR = ./include
SRCDIR = ./src
CC = gcc
CFLAGS = -O2
CFLAGS += -I $(INCLDIR)

OBJDIR = ./obj

_DEPS = source.h
DEPS = $(patsubst %, $(INCLDIR)/%,$(_DEPS))

_SRCS = source.c
SRCS = $(patsubst %, $(SRCDIR)/%,$(_SRCS))

_OBJS = source.o
OBJS = $(patsubst %, $(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task5: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

runclient:
	./task5 -server 127.0.0.1 -port 42042
runserver:
	./task5 -listen -port 42042
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

