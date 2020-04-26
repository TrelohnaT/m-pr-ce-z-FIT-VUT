PRJ=proj2
#
PROGS=$(PRJ)
FILES=proj2.c proj2.h
CC =gcc
CFLAGS= -Wall -W -Wextra -g -std=gnu99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
