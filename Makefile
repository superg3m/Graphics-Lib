all:
	g++ -I ./SDL2 -L ./lib -o ./Test/Output ./Test/*.c -lmingw32 -lSDL2main -lSDL2