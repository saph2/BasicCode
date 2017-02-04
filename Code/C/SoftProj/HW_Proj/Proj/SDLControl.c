#include "SDLControl.h"

int apply_surface(SDL_Surface* source, SDL_Surface* destination, int x, int y){
	/*applay panel on screen*/
	SDL_Rect offset; /*Temporary rectangle to hold the offsets*/
	offset.x = x; /*Get the offsets*/
	offset.y = y;
	return SDL_BlitSurface(source, NULL, destination, &offset); /*Blit the surface*/
}

int control_Constractor(char type, SDL_Surface **screen, int x, int y, char *filename, int trans, int trans_level, int flip) {
	/*constract window or panel*/
	switch (type){
	case WINDOW:
		return (window_Constracor(screen));
		break;
	case PANEL:
		return ((panel_Constracor(screen, filename, x, y, trans, trans_level, flip)));
		break;
	default:
		return 1;
		break;
	}
	return 1;
}

int window_Constracor(SDL_Surface **win){
	/*windoe constractor*/
	*win = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (*win == NULL){
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	if (SDL_FillRect(*win, 0, 0xFFFFFF) != 0) {/*color window white*/
		printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
		SDL_FreeSurface(*win);
		return 1;
	}

	SDL_WM_SetCaption("Chess", NULL);/*caption on screen*/

	return 0;
}

int panel_Constracor(SDL_Surface **base, char *filename, int x, int y, int trans, int trans_level, int flip){
	/*panel constractor*/
	int alpha = SDL_ALPHA_OPAQUE;
	SDL_Surface *panel;
	if (base != NULL){
		panel = SDL_LoadBMP(filename);
		if (panel == NULL){
			printf("ERROR: failed to load image: %s\n", SDL_GetError());
			return 1;
		}
		if (trans == 1){/*make picture more trasparent*/
			if (alpha > SDL_ALPHA_TRANSPARENT)
			{
				alpha -= trans_level;
			}
			SDL_SetAlpha(panel, SDL_SRCALPHA, alpha);
		}

		if (apply_surface(panel, *base, x, y) != 0){/*blit image on surface*/
			SDL_FreeSurface(panel);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return 1;
		}
		SDL_FreeSurface(panel);
		if (flip == 1){/*open window*/
			if (SDL_Flip(*base) != 0) {
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
				return 1;
			}
		}
	}
	else{
		return 1;
	}
	return 0;
}



