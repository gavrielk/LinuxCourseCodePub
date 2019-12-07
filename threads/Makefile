# Simple makefile create executable(.out) from every .c file
CC = gcc
LIBS = -lpthread

# SRC will hold an array of all the .c files
SRCS := $(subst ./,,$(shell find . -name "*.c"))

# OBJS will hold an array of the corresponding .out to the .c files
OBJS := $(patsubst %.c,%.out,$(SRCS))

# all will be the default TARGET and will create all the object files
all: $(OBJS)

# This rule will be executed for every .out file present in the target "all"
%.out: %.c
	$(CC) $^ -o $@ $(LIBS)

# Easy way to remove all binaries
clean:
	find . -name "*.out" -exec rm -rf {} \;
