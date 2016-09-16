all: main

main: main.o parse.o
	clang main.o parse.o -o shredder

main.o: main.c
	clang -c main.c

parse.o: parse.c
	clang -c parse.c

clean:
	rm *o shredder
