.PHONY: clean

LEX = flex  
YACC = bison
CC = gcc
CC_FLAGS = -Wall -g

all: calc

calc: main.c flex.c bison.c utils.c utils.h bison.h
	${CC} ${CC_FLAGS} -o $@ main.c flex.c bison.c utils.c

flex.c: calc.l bison.h
	${LEX} -o $@ calc.l

bison.c bison.h: calc.y
	${YACC} --defines=bison.h -o bison.c -v calc.y

clean:
	${RM} flex.c bison.* calc

