// Name: Justin Langston

#include <iostream>
#include <cstdio>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT= 480;

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Surface *image = NULL;

bool init();
bool loadMedia();
void close();


bool loadMedia(){

	image = SDL_LoadBMP("src/ff.bmp");
	if(image == NULL ){
		printf("IMAGE COULD NOT BE LOADED\n");
		return false;
	}

	// 								        R     G     B
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x33, 0xB5, 0xFF) );
	SDL_BlitSurface(image, NULL, screenSurface, NULL);

	return true;

}

bool init(){

	if( SDL_Init( SDL_INIT_VIDEO  ) < 0 ){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow( "Dank Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if(window == NULL){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	screenSurface = SDL_GetWindowSurface( window );

	return true;
}


void close(){
	SDL_FreeSurface( image );

	SDL_DestroyWindow( window );

	SDL_Quit();

}

int main(int argc, char** argv){

	if( !init() ){
		printf("Error in initialization\n");
		return -1;
	} 


	if( !loadMedia() ) {
		printf("Error in loading the media\n");
		return -1;	
	}

	bool exit = false;
	SDL_Event e;

	printf("Use up, down, left, right or w, s, a, d to change color. Press q to exit, or click exit window button\n");

	while( !exit ){

		while( SDL_PollEvent( &e ) != 0 ){

			if( e.type == SDL_QUIT ) exit = true;
			else if(e.type == SDL_KEYDOWN){


				switch( e.key.keysym.sym ){

					case SDLK_UP:
						printf("up\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x00, 0xB5, 0xFF) );
						break;

					case SDLK_DOWN:
						printf("down\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0xFF, 0xB5, 0xFF) );
						break;

					case SDLK_LEFT:
						printf("left\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0xFF, 0xFF, 0x00 ) );
						break;

					case SDLK_RIGHT:
						printf("right\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x00, 0xFF, 0x00) );
						break;

					case SDLK_w:
						printf("w\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x00, 0xB5, 0xFF) );
						break;

					case SDLK_s:
						printf("s\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0xFF, 0xB5, 0xFF) );
						break;

					case SDLK_a:
						printf("a\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0xFF, 0xFF, 0x00 ) );
						break;

					case SDLK_d:
						printf("d\n");
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x00, 0xFF, 0x00) );
						break;

					case SDLK_q:
						printf("q\n");
						exit = true;
						break;

					default:
						SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format,  0x33, 0xB5, 0xFF) );
						break;



				}



			}

		}

		SDL_BlitSurface(image, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface( window );
		//		SDL_Delay( 5000 );


	}



	close();

	return 0;
}
