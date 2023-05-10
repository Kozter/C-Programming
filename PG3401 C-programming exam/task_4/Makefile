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

task4: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lpthread -lrt

run:
	./task4 eksamen_v23_oppgave4_pg2265.txt
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

