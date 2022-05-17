# Variables declaration
# every variable can potentially also be an array of strings
# by seperating the strings with space e.g. LIBS
TARGET_NAME = zipper.out
IDIR = ./header
CC = gcc
CFLAGS = -I$(IDIR)
LIBS = -lzip

# ? means that  vallue will be set only if ODIR was not set for example from command line: make ODIR=native
ODIR ?= bin

# SRC will hold an array of all the .c files
# it is possible to run shell scripts as part of the makefile and recieve it's output
# "subst" is a built-in of make that removes substitutes strings
# Note: the order of command processing is from right to left
# the output passes from one to another: shell(find) -> subst -> SRCS
SRCS := $(subst ./,,$(shell find . -name "*.c"))

# OBJS will hold an array of the corresponding .o to the .c files
OBJS := $(addprefix $(ODIR)/,$(patsubst %.c,%.o,$(SRCS)))

# Left side of ":" is the target, right side is the dependecy/prerequisite
# % is pattern matching rule(wildcard) which expands in the target and dependency together
# for every file that matches the pattern %.c(which is every file whos name ends with .c)
# run the commands below it
# Note: expansion using ‘%’ in pattern rules occurs after variable and function expansions
# which take place when the makefile is read
$(ODIR)/%.o: %.c
	mkdir -p $(ODIR)/$(dir $<)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(ODIR)/$(TARGET_NAME): $(OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

# make treats input arguments as "target" usually the name of the output file
# .PHONY tells make to treat it as rules and not files
.PHONY: clean
clean:
	rm -rf $(ODIR)
