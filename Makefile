# vim: set filetype=make nowrap noexpandtab tabstop=4:
# Makefile
#
# PROJECT: 'GGBROWSER' (`ggbrowser`)
# AUTHOR: J. A. Corbal (<jacorbal@gmail.com>)

## Directories
PWD   = $(CURDIR)
I_DIR = ${PWD}/include
S_DIR = ${PWD}/src
L_DIR = ${PWD}/lib
O_DIR = ${PWD}/obj
B_DIR = ${PWD}/bin

SHELL=/bin/bash

## Compiler & linker options
CC           = cc   # gcc, clang
CCSTD        = c99  # c11, c17, gnu11, gnu17
CCOPT        = 3    # 0:debug; 1:optimize; 2:optimize more; 3:optimize yet more
CCWARN		 = -pedantic -pedantic-errors -Werror
EXTRA_CFLAGS =
EXTRA_LFLAGS = -lm -lc -lncurses -lmenu
CCFLAGS      = ${CCOPTS} ${CCWARN} -std=${CCSTD} ${CCEXTRA} -I ${I_DIR} ${EXTRA_CFLAGS}
LDFLAGS      = -L ${L_DIR} ${EXTRA_LFLAGS}

# Use `make DEBUG=1` to add debugging information, symbol table, etc.
# Use `make DEBUG=2` to link with the address sanitizer 
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CCFLAGS += -DDEBUG -g3 -ggdb3 -O0
else ifeq ($(DEBUG), 2)
	CCFLAGS += -DDEBUG -g3 -ggdb3 -O0
	LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fPIC
else
	CCFLAGS += -DNDEBUG -O${CCOPT}
endif


## Makefile options
SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .h .c .o


## Files options
TARGET = ${B_DIR}/main
SRCS = $(wildcard ${S_DIR}/*.c) $(wildcard ${S_DIR}/*/*.c)
OBJS = $(patsubst ${S_DIR}/%.c, ${O_DIR}/%.o, $(SRCS))
ARGS ?=


## Linkage
${TARGET}: ${OBJS}
	${CC} -o $@ $^ ${LDFLAGS}

## Compilation
${O_DIR}/%.o: ${S_DIR}/%.c
	${CC} ${CCFLAGS} -c -o $@ $<


## Make options
.PHONY: all ctags clean-obj clean-bin clean git run hard hard-run doxygen help

all:
	make ${TARGET}
	@make ctags

ctags:
ifeq (,$(wildcard "/usr/bin/ctags"))
	@echo "Generating tags..."
	@ctags -R --exclude='doc' --exclude='obj' --exclude='_tmp_'.
endif

clean-obj:
	rm --force ${OBJS}

clean-bin:
	rm --force ${TARGET}

clean:
	@make clean-obj
	@make clean-bin

run:
	${TARGET} ${ARGS}

hard:
	@make clean
	@make all

hard-run:
	@make hard
	@make run

doxygen:
	@[ -f 'Doxyfile' ] && doxygen || echo "'Doxyfile' not found" >&2

git:
	@git status --short

help:
	@echo "Type:"
	@echo "  'make all'......................... Build project"
	@echo "  'make run'................ Run binary (if exists)"
	@echo "  'make clean-obj'.............. Clean object files"
	@echo "  'make clean'....... Clean binary and object files"
	@echo "  'make hard'...................... Clean and build"
	@echo "  'make doxygen'...... Create Doxygen documentation"
	@echo "  'make git'...................... Check git status"
	@echo ""
	@echo " Binary will be placed in '${TARGET}'"
