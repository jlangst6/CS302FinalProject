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

/*
void Entity::update(SDL_Renderer* renderer){
	if(life == false) return;
	SDL_RenderCopy(renderer, t, NULL, &r);
}
*/

void Player::update(SDL_Renderer *renderer){
	if(life == false) return;
	SDL_RenderCopy(renderer, t, NULL, &r);

}

Player::Player(){
	name = "player";
}
Player::~Player(){
//	SDL_DestroyTexture(t);
}

void Lazer::update(SDL_Renderer *renderer){
	if(life == false) return;
	yPos-=.025;
	r.y+=yPos;
	if(yPos < 1) yPos=0;
	SDL_RenderCopy(renderer, t, NULL, &r);
}

Lazer::Lazer(){
	name = "lazer";
		
}

Lazer::~Lazer(){
//	SDL_DestroyTexture(t);
}



void Asteroid::update(SDL_Renderer *renderer){
	if(life == false) return;
	yPos+=.015;
	r.y+=yPos;
	if(yPos > 1) yPos=0;
	SDL_RenderCopy(renderer, t, NULL, &r);
}

Asteroid::Asteroid(){
	name = "asteroid";
}
Asteroid::~Asteroid(){
//	SDL_DestroyTexture(t);
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

	SDL_RenderCopy(renderer, Message1, NULL, &Message1Rect);
	SDL_RenderCopy(renderer, Message2, NULL, &Message2Rect);
	
	SDL_FreeSurface(tmpSurface1);
	SDL_FreeSurface(tmpSurface2);

	SDL_DestroyTexture(Message1);
	SDL_DestroyTexture(Message2);



}

void ScoreKeeper::gameOver(SDL_Renderer *renderer, TTF_Font *font){
	string number = to_string(score);
	string t1 = "GAME OVER...";
	string t2 = "FINAL SCORE :";

	tmpSurface1 = TTF_RenderText_Solid(font, t1.c_str(), White);
	Message1 = SDL_CreateTextureFromSurface(renderer, tmpSurface1);

	tmpSurface2 = TTF_RenderText_Solid(font, t2.c_str(), White);
	Message2 = SDL_CreateTextureFromSurface(renderer, tmpSurface2);

	tmpSurface1 = TTF_RenderText_Solid(font, number.c_str(), White);
	Message3 = SDL_CreateTextureFromSurface(renderer, tmpSurface1); 

	Message1Rect.x = 500/2 - 150;
	Message1Rect.y = 0;
	Message1Rect.w = 300;
	Message1Rect.h = 200;


	Message2Rect.x = 500/2 - 150;
	Message2Rect.y = 150;
	Message2Rect.w = 300;
	Message2Rect.h = 200;

	SDL_Rect Message3Rect ;
	Message3Rect.x = 500/2 - 200;
	Message3Rect.y = 350;
	Message3Rect.h = 125;
	if(score == 0) {Message3Rect.w=100; Message3Rect.x = 500/2 -50;  }
	else if(score < 1000) {Message3Rect.w=175; Message3Rect.x = 500/2 - 100;}
	else if(score < 10000) {Message3Rect.w = 200; Message3Rect.x = 500/2 - 110;}
	else {Message3Rect.w = 250;  Message3Rect.x = 500/2 -125;}
	

	SDL_RenderCopy(renderer, Message1, NULL, &Message1Rect);
	SDL_RenderCopy(renderer, Message2, NULL, &Message2Rect);
	SDL_RenderCopy(renderer, Message3, NULL, &Message3Rect);

	SDL_FreeSurface(tmpSurface1);
	SDL_FreeSurface(tmpSurface2);

	SDL_DestroyTexture(Message1);
	SDL_DestroyTexture(Message2);
	SDL_DestroyTexture(Message3);

	// deconstructor to free surfaces?

}

ScoreKeeper::~ScoreKeeper(){
}

#endif
