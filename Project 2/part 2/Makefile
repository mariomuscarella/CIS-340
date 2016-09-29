#
#	GROUP F: Yi Dong(CSUID), Mario Muscarella(2478702), Bri O'Neil(2583119)
#	CIS 340: Project 2
#

CC=gcc
CFLAGS=-g -Wall -c
#DEPS=fshell.h
SRC=fshell.c

OBJ=$(SRC:.c=.o)


#%.o: %.c $(DEPS)
#	$(CC) -Wall -c -g -o $@ $(CFLAGS)

all: $(OBJ)
	$(CC) $(CFLAGS) $(SRC)
#	TODO: compilation for part 2

.PHONY: clean

clean:
	rm -f *.o part1
