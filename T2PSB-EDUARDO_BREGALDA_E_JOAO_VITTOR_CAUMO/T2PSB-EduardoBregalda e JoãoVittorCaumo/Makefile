CC = gcc
all: ig run
run:
	./main.o leitorAqv.c
ig:
	$(CC) -Wall leitorAqv.c main.c -o main.o
clean:
	-@ rm -f $(main.o leitorAqv.o)
depend:
	makedepend -- ${- Wall -g} -- ${main.c leitorAqv.c}

