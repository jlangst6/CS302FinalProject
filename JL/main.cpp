#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <set>
#include <string>
#include <random>
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
bool checkCollision(Entity *a, Entity *b);



int main(){
	if(!init()){
		printf("Error in initialization\n");
		return -1;
	}

	Player player;
	player.init("media/spaceship.png", gRenderer, SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT-85, 60, 60);	

	//Entity *tmp = new Entity;
	//tmp->init("media/rock.png", gRenderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100, 100);

	set<Entity* > entities;

	srand(time(NULL));

	Entity* tmp;

	entities.insert(&player);

	for(int i=0; i<3; i++){
		tmp = new Asteroid;
		tmp->init("media/rock.png", gRenderer, rand()%441, SCREEN_HEIGHT/2, 75, 75);
		entities.insert(tmp);
	}

	//entities.push_back(&player);
	//entities.push_back(tmp);


	// write update for each entity inherited class

	bool quit = false;

	SDL_Event e;
	ScoreKeeper sk;

	// possible vals for x are 0 - 440 at 20 increments, so must spawn asteroids in there	


	while(!quit){
		while(SDL_PollEvent( &e ) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}

			else if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_w:
						player.r.y-=20;
						break;
					case SDLK_UP:
					case SDLK_SPACE:
			//			printf("FIRE\n");
						tmp = new Lazer;
						tmp->init("media/lazer.png", gRenderer, player.r.x+30, player.r.y+1, 10, 30);
						entities.insert(tmp);
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

		// tmp
		//if(checkCollision(tmp, &player)) player.life=false;//printf("COLLISION\n");
		int numAsteroids =0;	
		for(auto i : entities){
			for(auto j: entities){
				if(i == j) continue;
				if(i->name == "lazer" && j->name == "asteroid"){
					if(checkCollision(i, j)){
						i->life=0;
						j->life=0;
				//		entities.erase(i);
				//		entities.erase(j);
						sk.score+=25;
						numAsteroids++;

					}
				}

				else if( i->name == "player" && j->name == "asteroid") {
					if(checkCollision(i, j)){
						i->life=0;
						j->life=0;
				//		entities.erase(i);
				//		entities.erase(j);
						//			delete i;
						//			delete j;
					}
				}

				if(i->name == "asteroid" && i->r.y >= SCREEN_HEIGHT -50 && i->life != 0) numAsteroids++;
				if(i->r.y <= 0 || i->r.y >= SCREEN_HEIGHT - 50) i->life=0;  
					
					
			
		
		}
		}

		for(int i=0; i<numAsteroids ; i++){
		tmp = new Asteroid;
		tmp->init("media/rock.png", gRenderer, rand()%441, SCREEN_HEIGHT/2, 75, 75);
		entities.insert(tmp);

		}


		// Clear Screen
		SDL_RenderClear( gRenderer );

		// render background
		SDL_RenderCopy(gRenderer, bg, NULL, NULL);

		// render score
		sk.render(gRenderer, Calibre);

		// render player
		player.update(gRenderer);

		// render tmp asteroid
		//tmp->update(gRenderer);

		for(auto i = entities.begin(); i != entities.end();){
			Entity *e = *i;

			e->update(gRenderer);

			if(e->life == 0){
				if(e->name == "player"){ ++i; continue;}
				i = entities.erase(i);
				e->~Entity();
				delete e;
			}
			else ++i;

		}

		// Update Screen
		SDL_RenderPresent( gRenderer );

//		printf("%lu\n",entities.size());

		if(! player.life ) break;
		SDL_Delay(16);
	}	
/*
	for(auto i = entities.begin(); i != entities.end(); ++i){
			Entity *e = *i;
			i = entities.erase(i);
			e->~Entity();
			delete e;
		}

	*/

	printf("GAME OVER\nFINAL SCORE: %d\n", sk.score);

	SDL_RenderClear( gRenderer );
	SDL_RenderCopy(gRenderer, bg, NULL, NULL);
	sk.gameOver(gRenderer, Calibre);
	SDL_RenderPresent( gRenderer );

	
	SDL_Delay(4000);
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

bool checkCollision(Entity *a, Entity *b){
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA= a->r.x;
	rightA = a->r.x + a->r.w;
	topA=a->r.y;
	bottomA = a->r.y + a->r.h;

	leftB= b->r.x;
	rightB = b->r.x + b->r.w;
	topB=b->r.y;
	bottomB = b->r.y + b->r.h;

	if( bottomA <= topB )
		return false;
	if(topA >= bottomB )
		return false;
	if(rightA <= leftB)
		return false;
	if(leftA >= rightB)
		return false;

	return true;

}


