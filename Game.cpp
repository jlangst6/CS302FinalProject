#ifndef GAME_CPP
#define GAME_CPP

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Game.h"

using namespace std;

// Entity constructor
Entity::Entity(){
	life = true;
	t = NULL;
}

// Entity init
void Entity::init(SDL_Texture *text, SDL_Renderer *renderer, int startX, int startY, int width, int height){
	// set texture to text
	t = text;
	// set SDL_Rect values
	r.x=startX;
	r.y=startY;
	r.w=width;
	r.h = height;
}

// Player constructor
Player::Player(){
	name = "player";

}

// Player update
void Player::update(SDL_Renderer *renderer){
	// if killed, dont render, else render
	if(life == false) return;
	SDL_RenderCopy(renderer, t, NULL, &r);

}

// Lazer constructor
Lazer::Lazer(){
	name = "lazer";
}

// Lazer update
void Lazer::update(SDL_Renderer *renderer){
	// if killed, don't render
	if(life == false) return;
	// else increment y pos by 20
	r.y-=20;
	// render
	SDL_RenderCopy(renderer, t, NULL, &r);
}

// Asteroid constructor
Asteroid::Asteroid(){
	name = "asteroid";
}

// Asteroid update
void Asteroid::update(SDL_Renderer *renderer){
	// if killed, don't render
	if(life == false) return;
//	add++;
//	if(add % 3 == 0){r.y++; add=0; } // can actually remove the add=0 bc it will never go out of range of the screen height
	r.y++;
	
	// delay SDL based on the difficulty
	switch(difficulty){
		case 0:
			SDL_Delay(5);
			break;
		case 1:
			SDL_Delay(4);
			break;
		case 2:
			SDL_Delay(3);
			break;
		case 3:
			SDL_Delay(2);
			break;
		case 4:
			SDL_Delay(1);
		case 5:
			SDL_Delay(0);
			break;
		default:
			break;
	}

	// render texture to screen
	SDL_RenderCopy(renderer, t, NULL, &r);
}

// Scorekeeper constructor
ScoreKeeper::ScoreKeeper(TTF_Font *font){
	Message1 = NULL;
	Message2 = NULL;
	Message3 = NULL;
	score =0;
	
	// create this tmpsurface in constructor, so we don't have to keep on creating it in the render function
	tmpSurface1 = TTF_RenderText_Blended(font, "Score:", White);
}

// Scorekeeper render
void ScoreKeeper::render(SDL_Renderer *renderer, TTF_Font *font){

	// convert the score to a string
	string number = to_string(score);

	// create tmp surface for the score into a image surface
	tmpSurface2 = TTF_RenderText_Blended(font, number.c_str(), White);

	// create the message textures from the tmp surfaces
	Message1 = SDL_CreateTextureFromSurface(renderer, tmpSurface1);
	Message2 = SDL_CreateTextureFromSurface(renderer, tmpSurface2);

	// set position and scale of message on screen 
	Message1Rect.x = 150; //500/2 - 100;
	Message1Rect.y = -15;
	Message1Rect.w = 100;
	Message1Rect.h = 100;

	// set position and scale of message on screen 
	Message2Rect.x = 250; 
	Message2Rect.y = -15;
	Message2Rect.h = 100;
	// scale width appropriate to size of score
	if(score == 0) Message2Rect.w=50;
	else if(score < 1000) Message2Rect.w=75;
	else if(score < 10000) Message2Rect.w = 100;
	else Message2Rect.w = 150;

	// render messages to the screen
	SDL_RenderCopy(renderer, Message1, NULL, &Message1Rect);
	SDL_RenderCopy(renderer, Message2, NULL, &Message2Rect);
	
	// free the score surface
	//SDL_FreeSurface(tmpSurface1);
	SDL_FreeSurface(tmpSurface2);

	// destroy the textures
	SDL_DestroyTexture(Message1);
	SDL_DestroyTexture(Message2);
}

// Scorekeeper gameOver
void ScoreKeeper::gameOver(SDL_Renderer *renderer, TTF_Font *font){

	// convert score to a string
	string number = to_string(score);
	
	// strings for other messages
	string t1 = "GAME OVER...";
	string t2 = "FINAL SCORE :";

	// create texture for Message 1 ("GAME OVER...")
	tmpSurface1 = TTF_RenderText_Solid(font, t1.c_str(), White);
	Message1 = SDL_CreateTextureFromSurface(renderer, tmpSurface1);

	// create texture for Message 2 ("FINAL SCORE :")
	tmpSurface2 = TTF_RenderText_Solid(font, t2.c_str(), White);
	Message2 = SDL_CreateTextureFromSurface(renderer, tmpSurface2);

	// create texture for Message 3 (score)
	tmpSurface1 = TTF_RenderText_Solid(font, number.c_str(), White);
	Message3 = SDL_CreateTextureFromSurface(renderer, tmpSurface1); 

	// set position and scale on screen 
	Message1Rect.x = 100; 
	Message1Rect.y = 0;
	Message1Rect.w = 300;
	Message1Rect.h = 200;

	// set position and scale on screen 
	Message2Rect.x = 100;
	Message2Rect.y = 150;
	Message2Rect.w = 300;
	Message2Rect.h = 200;

	// create new rect for Message 3
	SDL_Rect Message3Rect ;

	// set position and scale on screen 
	Message3Rect.x = 50;
	Message3Rect.y = 350;
	Message3Rect.h = 125;

	// based on score, scale the width to the appropriate size
	if(score == 0) {Message3Rect.w=100; Message3Rect.x = 500/2 -50;  }
	else if(score < 1000) {Message3Rect.w=175; Message3Rect.x = 500/2 - 100;}
	else if(score < 10000) {Message3Rect.w = 200; Message3Rect.x = 500/2 - 110;}
	else {Message3Rect.w = 250;  Message3Rect.x = 500/2 -125;}
	

	// render all messages to the screen
	SDL_RenderCopy(renderer, Message1, NULL, &Message1Rect);
	SDL_RenderCopy(renderer, Message2, NULL, &Message2Rect);
	SDL_RenderCopy(renderer, Message3, NULL, &Message3Rect);

	// free the tmp surfaces
	SDL_FreeSurface(tmpSurface1);
	SDL_FreeSurface(tmpSurface2);

	// destroy the textures
	SDL_DestroyTexture(Message1);
	SDL_DestroyTexture(Message2);
	SDL_DestroyTexture(Message3);
}

#endif
