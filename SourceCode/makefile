MAZE: main.o support.o game.o heap.o
	gcc main.o support.o game.o heap.o -o GAME1.0 -lm -lncurses
	
main.o: main.c
	gcc -c main.c

heap.o: heap.c
	gcc -c heap.c

support.o: support.c support.h
	gcc -c support.c

game.o: game.c game.h
	gcc -c game.c


