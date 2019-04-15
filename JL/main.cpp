#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <string>
#include "Game.cpp"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *bg = NULL;
TTF_Font *Calibre = NULL;

SDL_Color White = {255, 255, 255};

// funciton to initialize everything in SDL
bool init();

// closes SDL, frees everything up
void close();

// checks if two entities collide
bool checkCollision(Entity *a, Entity *b){
}


int main(){
	if(!init()){
		printf("Error in initialization\n");
		return -1;
	}

	Entity player;
	player.init("media/spaceship.png", gRenderer, SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT-85, 60, 60);	

	Entity *tmp = new Entity;
	tmp->init("media/rock.png", gRenderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100, 100);

	
	bool quit = false;

	SDL_Event e;
	ScoreKeeper sk;
	
	while(!quit){

		while(SDL_PollEvent( &e ) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}

			else if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_w:
					case SDLK_UP:
					case SDLK_SPACE:
						printf("FIRE\n");
						break;
					case SDLK_a:
					case SDLK_LEFT:
						if(player.r.x != 0) player.r.x-=20;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						if(player.r.x != 440) player.r.x+=20;
						break;
					default:
						sk.score+=25;
						printf("xPos: %d\n", player.r.x);


				}		
					

			}

		}
		// Clear Screen
		SDL_RenderClear( gRenderer );

		// render background
		SDL_RenderCopy(gRenderer, bg, NULL, NULL);

		sk.render(gRenderer, Calibre);
		// render player
		player.update(gRenderer);

		tmp->update(gRenderer);

		// Update Screen
		SDL_RenderPresent( gRenderer );


	}	
	printf("Your final score was: %d\n", sk.score);
	close();

}


bool init(){
	bool success = true;
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("SDL INIT VIDEO COULD NOT INIT, SDL ERROR: %s\n", SDL_GetError());
		success = false;
	}

	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
		printf("WARNING: Linear texture filtering not enabled!\n");
	}

	gWindow = SDL_CreateWindow("SSG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(gWindow == NULL){
		printf("Window could not be created, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL){
		printf("Renderer could not be created, SDL_ERROR: %s\n", SDL_GetError());
		success=false;
	}
	
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;

	if( !(IMG_Init(imgFlags) & imgFlags) ){
		printf("SDL_image could not initialize, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	if(TTF_Init() < 0){
		printf("SDL_ttf could not initialize, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	// Load the background picture
	SDL_Surface *tmpSurface = NULL;

	tmpSurface = IMG_Load("media/bg.png");
	if(tmpSurface == NULL) printf("Failed to load the background\n");
	bg = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);


	// Load the font
	Calibre = TTF_OpenFont("media/Calibre.ttf", 24);
	if(Calibre == NULL ) printf("Could not load text %s\n", SDL_GetError());

	return success;
	
}


void close(){
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );

	SDL_DestroyTexture( bg );

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}
