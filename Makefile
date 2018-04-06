all:
	gcc -o conway_32_bits screen.c main.c -IC:\MinGW\include -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2
	gcc -o conway_32_bits_omp screen.c main.c -IC:\MinGW\include -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -fopenmp