#ifndef GAME_CPP
#define GAME_CPP

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"

using namespace std;

/*
   class Entity {
   public:
   SDL_Rect r;
   SDL_Texture* t;
   string name;
   bool life;

   int xPos, yPos;
   bool init(string filename); // will make texture and rectangle from filename
   void update(SDL_Renderer* g); // will render texture to screen, also check if life is true or not, if not will delete rect and texture
   Entity();
   ~Entity();

   };

*/

Entity::Entity(){
	life = true;
	t = NULL;
}

Entity::~Entity(){
	SDL_DestroyTexture( t );	
	t = NULL;
}


bool Entity::init(string filename, SDL_Renderer *renderer, int startX, int startY, int width, int height){
	SDL_Surface* tmpSurface = NULL;
	tmpSurface = IMG_Load(filename.c_str());

	if(tmpSurface == NULL){
		printf("Error In Entity Init, Couldn't Load Image. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	t = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface( tmpSurface );


	// set SDL_Rect values
	r.x=startX;
	r.y=startY;
	r.w=width;
	r.h = height;

}

void Entity::update(SDL_Renderer* renderer){
	if(life == false) return;
	SDL_RenderCopy(renderer, t, NULL, &r);
}

/*


class ScoreKeeper{
	public:
		unsigned int score;
		string text = "Score: ";
		SDL_Color White = {255, 255, 255};
		SDL_Texture* Message1;
		SDL_Texture* Message2;

		SDL_Rect Message1Rect;
		SDL_Rect Message2Rect;

		SDL_Surface *tmpSurface1;
		SDL_Surface *tmpSurface2;

		void Render(SDL_Renderer *renderer);

		ScoreKeeper();
};
*/

ScoreKeeper::ScoreKeeper(){
	Message1 = NULL;
	Message2 = NULL;
	score =0;

}

void ScoreKeeper::render(SDL_Renderer *renderer, TTF_Font *font){

	string number = to_string(score);
	//tmpSurface1 = TTF_RenderText_Solid(font, text.c_str(), White);
	tmpSurface1 = TTF_RenderText_Blended(font, text.c_str(), White);
	//tmpSurface2 = TTF_RenderText_Solid(font, number.c_str(), White);
	tmpSurface2 = TTF_RenderText_Blended(font, number.c_str(), White);

	Message1 = SDL_CreateTextureFromSurface(renderer, tmpSurface1);
	Message2 = SDL_CreateTextureFromSurface(renderer, tmpSurface2);

	Message1Rect.x = 500/2 - 100;
	Message1Rect.y = -15;
	Message1Rect.w = 100;
	Message1Rect.h = 100;


	Message2Rect.x = 500/2;
	Message2Rect.y = -15;
	if(score == 0) Message2Rect.w=50;
	else if(score < 1000) Message2Rect.w=75;
	else if(score < 10000) Message2Rect.w = 100;
	else Message2Rect.w = 150;
	Message2Rect.h = 100;

	SDL_RenderCopy(renderer, Message1, NULL, &Message1Rect);
	SDL_RenderCopy(renderer, Message2, NULL, &Message2Rect);

}



#endif
