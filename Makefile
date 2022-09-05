FLAGS = -Wall -Wextra -pedantic -std=c11 

all: libgol.so gameoflife.o
	gcc $(FLAGS) gameoflife.o -L. -lgol -g -o gameoflife

libgol.so: gol.o 
	gcc $(FLAGS) -shared $< -o $@ 

%.o: %.c gol.h
	gcc -fPIC  $(FLAGS) -c $< -o $@

clean:
	rm -rf gameoflife gameoflife.o gol.o libgol.so
