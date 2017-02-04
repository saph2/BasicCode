#define SDLControl_H_

#include "chessprog.h"
#include "Game.h"
#include "Moves.h"
#include "files.h"
//#include <SDL.h>
#include <SDL_image.h>

#define WIN_W 640
#define WIN_H 480
#define IMG_W 240
#define IMG_H 296
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32
#define SQUARE 44
#define WINDOW 'w'
#define PANEL 'p'
#define SAVE_GAME 's'
#define LOAD_GAME 'l'

int apply_surface(SDL_Surface* source, SDL_Surface* destination, int x, int y);
int control_Constractor(char type, SDL_Surface **screen, int x, int y, char *filename, int trans, int trans_level, int flip);
int window_Constracor(SDL_Surface **win);
int panel_Constracor(SDL_Surface **base, char *filename, int x, int y, int trans, int trans_level, int flip);