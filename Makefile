all:
	gcc -static -o conway screen.c main.c -IC:\MinGW\include -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -fopenmp