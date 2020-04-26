PRJ=ifj19
#
PROGS=$(PRJ)
FILES=main.c errors.c generator.c expr.c lex_anal.c symtable.c parser.c
CC =gcc
CFLAGS= -Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
