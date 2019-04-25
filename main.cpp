#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <unordered_set>
#include <string>
#include <random>
#include "Game.cpp"

// Window Dimensions
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

// Textures and SDL vars that we'll be constantly using
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *bg = NULL;
SDL_Texture *spaceship = NULL;
SDL_Texture *asteroid = NULL;
SDL_Texture *lazer = NULL;
TTF_Font *Calibre = NULL;

// funciton to initialize everything in SDL
bool init();

// function to load all of our images to textures and our font
bool loadMedia();

// closes SDL, frees everything up
void close();

// checks if two entities collide
bool checkCollision(Entity *a, Entity *b);

// Main
int main(){

	// Initialze all SDL Libraries we need, window and renderer
	if(!init()){
		printf("Error, Failed to initialize\n");
		return -1;
	}

	// load all the images and our font
	if(!loadMedia()){
		printf("Error, Failed to load media\n");
		return -1;
	}

	// make and initialize the player
	Player player;
	player.init( spaceship, gRenderer, SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT-85, 30, 60);	

	// unordered_set to store all our entities on the screen
	unordered_set<Entity* > entities;

	// random number generator from <random>
	default_random_engine generator;

	// distribution ranges for generating random numbers in between
	uniform_int_distribution<int> randomXpos(0,441);
	uniform_int_distribution<int> randomYpos(100, 250);
	uniform_int_distribution<int> randomAsteroidSize(20, 120);


	// tmp entity pointer for allocating new entities
	Entity* tmp;

	// insert palyer into entities set
	entities.insert(&player);

	// generate 5 asteroids and insert into entities vector
	for(int i=0; i<5; i++){
		tmp = new Asteroid;
		//tmp->init(asteroid, gRenderer, randomXpos(generator), 75, 75, 75);
		tmp->init(asteroid, gRenderer, randomXpos(generator), 75, randomAsteroidSize(generator), randomAsteroidSize(generator) );
		// insert into entities unordered_set
		entities.insert(tmp);
	}

	// bool to quit out of game loop
	bool quit = false;

	// to handle keyboard events
	SDL_Event e;

	// score keeper from score keeper class, which outputs current score to screen
	ScoreKeeper sk(Calibre);

	while(!quit){

		// user requests quit
		while(SDL_PollEvent( &e ) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}

			// else handle keyboard input
			else if (e.type == SDL_KEYDOWN){

				switch(e.key.keysym.sym){

					// fire lazer 
					case SDLK_w:
					case SDLK_f:
					case SDLK_UP:
					case SDLK_SPACE:
						// create new lazer and initialize
						tmp = new Lazer;
						tmp->init(lazer, gRenderer, player.r.x+10, player.r.y+1, 10, 30);
						// insert into entities
						entities.insert(tmp);
						break;

						// move player left
					case SDLK_a:
					case SDLK_LEFT:
						// move player pos left if it doesn't go out of bounds of screen
						if(player.r.x != 0) player.r.x-=20;
						break;

						// move player right
					case SDLK_d:
					case SDLK_RIGHT:
						if(player.r.x != 440) player.r.x+=20;
						break;

						// add to score for debugging/easy testing of difficulty
					default:
						sk.score+=100;
						break;
				}		
			}
		}

		// for adding more asteroids to the screen 
		int numAsteroids =0;	

		// check for collisions between entities
		for(auto i : entities){
			for(auto j : entities){
				
				// entities are the same, don't do anything
				if(i == j) continue;


				// check for collision between lazers and asteroids
				if(i->name == "lazer" && j->name == "asteroid"){
					if(checkCollision(i, j)){
						i->life=0;
						j->life=0;
						sk.score+=25;
						numAsteroids++;
					}
				}

				// check for collision between player and asteroids
				else if( i->name == "player" && j->name == "asteroid") {
					if(checkCollision(i, j)){
						i->life=0;
						j->life=0;
					}
				}

				// for making the asteroids at bottom of screen dissapear better
				int yCheck = i->r.y + i->r.h/2;

				// increment numAsteroids to add new ones

				//	if(i->name == "asteroid" && i->r.y >= SCREEN_HEIGHT -85 && i->life != 0) numAsteroids++;
				if(i->name == "asteroid" && yCheck >= SCREEN_HEIGHT -85 && i->life != 0) numAsteroids++;
				//	if(i->name == "asteroid" && i->r.y >= SCREEN_HEIGHT -85 && i->life != 0 && sk.score > 1000) numAsteroids++;
				if(i->name == "asteroid" && yCheck >= SCREEN_HEIGHT -85 && i->life != 0 && sk.score > 1000) numAsteroids++;

				// kill out of bounds asteroids
				
				//if(i->r.y >= SCREEN_HEIGHT - 85 && i->name == "asteroid") i->life=0;  
				if(yCheck >= SCREEN_HEIGHT - 85 && i->name == "asteroid") i->life=0;  
				if(i->r.y <= 0 && i->name == "lazer") i->life=0;  

			}
		}

		// spawn new asteroids
		for(int i=0; i<numAsteroids ; i++){
			// 16 is max size of entities on the screen
			if(entities.size() > 16) break;
			
			// create new random asteroids
			tmp = new Asteroid;
			tmp->init(asteroid, gRenderer, randomXpos(generator), randomYpos(generator), randomAsteroidSize(generator), randomAsteroidSize(generator));

			// set the difficulty based on the current score
			if(sk.score < 1000 && sk.score > 0) tmp->difficulty=0;
			else if(sk.score > 1000 && sk.score < 2000) tmp->difficulty=1; 
			else if(sk.score > 2000 && sk.score < 3000) tmp->difficulty=2;
			else if(sk.score > 3000 && sk.score < 4000) tmp->difficulty=3;
			else if(sk.score > 4000 && sk.score < 5000) tmp->difficulty=4;
			else if(sk.score > 5000) tmp->difficulty=5;

			// insert into entities
			entities.insert(tmp);
		}

		// if little asteroids on screen, spawn more
		if(entities.size() <= 4){
			// create new random asteroids
			tmp = new Asteroid;
			tmp->init(asteroid, gRenderer, randomXpos(generator), randomYpos(generator), randomAsteroidSize(generator), randomAsteroidSize(generator));

			// set the difficulty based on the current score
			if(sk.score < 1000 && sk.score > 0) tmp->difficulty=0;
			else if(sk.score > 1000 && sk.score < 2000) tmp->difficulty=1; 
			else if(sk.score > 2000 && sk.score < 3000) tmp->difficulty=2;
			else if(sk.score > 3000 && sk.score < 4000) tmp->difficulty=3;
			else if(sk.score > 4000 && sk.score < 5000) tmp->difficulty=4;
			else if(sk.score > 5000) tmp->difficulty=5;

			// insert into entities
			entities.insert(tmp);
	
		}

		// clear Screen
		SDL_RenderClear( gRenderer );

		// render background
		SDL_RenderCopy(gRenderer, bg, NULL, NULL);

		// render score
		sk.render(gRenderer, Calibre);


		// go through entities
		for(auto i = entities.begin(); i != entities.end();){
			// get entity
			Entity *e = *i;
			
			// render to screen
			e->update(gRenderer);

			// if it is killed, 
			if(e->life == 0){

				// if it is the player, dont delete it, iterate to next element 
				if(e->name == "player"){ ++i; continue;}
				// erase entity from entities
				i = entities.erase(i);
				// delete allocated entity
				delete e;
			}
			// else increment iterator
			else ++i;

		}

		// Update Screen
		SDL_RenderPresent( gRenderer );


		// if player is dead, signal quit loop
		if(! player.life ) quit = true;

		// delay SDL 16ms
		SDL_Delay(16);

	}	

	// erase player from unordered_set we can delete rest of allocated entities
	entities.erase(&player);

	// deallocate rest or entities
	for(auto i : entities) delete i;

	// print final score to console
	printf("GAME OVER\nFINAL SCORE: %d\n", sk.score);

	// clear screen
	SDL_RenderClear( gRenderer );

	// render background
	SDL_RenderCopy(gRenderer, bg, NULL, NULL);

	// output players final score, game over screen
	sk.gameOver(gRenderer, Calibre);

	// update screen
	SDL_RenderPresent( gRenderer );

	// delay so player can see final score
	SDL_Delay(4000);

	// close SDL
	close();

}


bool init(){
	// success flag
	bool success = true;

	// init SDL video
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("SDL INIT VIDEO COULD NOT INIT, SDL ERROR: %s\n", SDL_GetError());
		success = false;
	}

	// set scaling
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
		printf("WARNING: Linear texture filtering not enabled!\n");
	}

	// create window
	gWindow = SDL_CreateWindow("SSG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// error check window
	if(gWindow == NULL){
		printf("Window could not be created, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	// create renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL){
		printf("Renderer could not be created, SDL_ERROR: %s\n", SDL_GetError());
		success=false;
	}


	// init SDL image
	int imgFlags = IMG_INIT_PNG;
	if( !(IMG_Init(imgFlags) & imgFlags) ){
		printf("SDL_image could not initialize, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	// init SDL TTF
	if(TTF_Init() < 0){
		printf("SDL_ttf could not initialize, SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}

	return success;

}

bool loadMedia(){
	bool success = true;

	// tmp surface
	SDL_Surface *tmpSurface = NULL;

	// Load the background picture to texture
	tmpSurface = IMG_Load("media/bg.png");
	if(tmpSurface == NULL){ printf("Failed to load the background, SDL_ERROR: %s\n", SDL_GetError()); success = false; }
	bg = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	tmpSurface = NULL;

	// Load the spaceship to texture 
	tmpSurface = IMG_Load("media/spaceship.png");
	if(tmpSurface == NULL){ printf("Failed to load the spaceship, SDL_ERROR: %s\n", SDL_GetError()); success = false; }
	spaceship = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	tmpSurface = NULL;

	// Load the media to texture 
	tmpSurface = IMG_Load("media/asteroid.png");
	if(tmpSurface == NULL){ printf("Failed to load the asteroid, SDL_ERROR: %s\n", SDL_GetError()); success = false; }
	asteroid = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	tmpSurface = NULL;

	// Load the lazer to texture 
	tmpSurface = IMG_Load("media/lazer.png");
	if(tmpSurface == NULL){ printf("Failed to load the lazer, SDL_ERROR: %s\n", SDL_GetError()); success = false; }
	lazer = SDL_CreateTextureFromSurface(gRenderer, tmpSurface);

	// Load the font
	Calibre = TTF_OpenFont("media/Calibre.ttf", 24);
	if(Calibre == NULL ) printf("Could not load text, SDL_ERROR: %s\n", SDL_GetError());

	// free the tmp surface
	SDL_FreeSurface(tmpSurface);

	return success;

}

// close 
void close(){
	// destroy window and renderer
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );

	// destroy all textures 
	SDL_DestroyTexture( bg );
	SDL_DestroyTexture( lazer );
	SDL_DestroyTexture( spaceship );
	SDL_DestroyTexture( asteroid );

	// close font
	TTF_CloseFont(Calibre);
	
	// quit all SDL systems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

// collision detection on 2 rectangles from Lazyfoo tutorials
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
