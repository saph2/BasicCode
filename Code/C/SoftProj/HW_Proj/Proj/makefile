all: chessprog 

LIBS = -L /usr/local/lib/SDL_image-1.2.12/lib -lSDL_image
INCLUDES = -I /usr/local/lib/SDL_image-1.2.12/include/

clean:
	-rm chessprog.o Moves.o Game.o files.o SDLControl.o SDLMenu.o SDLGame.o

chessprog: chessprog.o Moves.o Game.o files.o SDLControl.o SDLMenu.o SDLGame.o 
	gcc $(LIBS) -o chessprog chessprog.o Moves.o Game.o files.o SDLControl.o SDLMenu.o SDLGame.o -lm -ansi -pedantic-errors -g `sdl-config --libs`

chessprog.o: chessprog.c chessprog.h Moves.h Game.h files.h SDLControl.h SDLMenu.h SDLGame.h 
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm chessprog.c `sdl-config --cflags`

Moves.o: Moves.c Moves.h chessprog.h Game.h files.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm Moves.c `sdl-config --cflags`

Game.o: Game.c Game.h Moves.h chessprog.h files.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm Game.c `sdl-config --cflags`

files.o: files.c files.h chessprog.h Moves.h Game.h SDLControl.h SDLMenu.h SDLGame.h 
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm files.c `sdl-config --cflags`

SDLMenu.o: SDLMenu.c chessprog.h Moves.h Game.h files.h SDLControl.h SDLMenu.h SDLGame.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm SDLMenu.c `sdl-config --cflags`

SDLGame.o: SDLGame.c chessprog.h Moves.h Game.h files.h SDLControl.h SDLMenu.h SDLGame.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm SDLGame.c `sdl-config --cflags`

SDLControl.o: SDLControl.c chessprog.h Moves.h Game.h files.h SDLControl.h SDLMenu.h SDLGame.h
	gcc $(INCLUDES) -ansi -pedantic-errors -c -Wall -g -lm SDLControl.c `sdl-config --cflags`




	
	
	

