PRJ=ifj19
#
PROGS=$(PRJ)
FILES=main.c errors.h lex_anal.c lex_anal.h generator.c generator.h
CC =gcc
CFLAGS= -Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
