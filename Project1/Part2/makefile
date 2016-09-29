CC = gcc
CFLAGS = -std=gnu99 -Wall -g -I.

OBJ = file2string.c custout.c

file2string: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run:
	./custout

.PHONY: clean
clean:
	rm -f *.o *~ file2string
