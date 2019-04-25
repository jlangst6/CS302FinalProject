#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class Entity {
	public:
		Entity();           // Entity Constructor which will initialize name
		SDL_Rect r;         // For keeping track of size of entity
		SDL_Texture* t;     // Image texture for rendering
		string name;        // For keeping the name of entity in inheritence
		bool life;          // To keep track if the entity is alive or not
		int difficulty = 0; // For the dificulty of the game

		virtual void update(SDL_Renderer *){}; // Inherited function to update position and render entity to screen
		void init(SDL_Texture *text, SDL_Renderer *renderer, int startX, int startY, int width, int height); // Will set the texture and rectangle size
};

// Player class that inherits from Entity
class Player : public Entity {
	public:
		Player();
		void update(SDL_Renderer* renderer);
};

// Asteroid class that inherits from Entity
class Asteroid : public Entity {
	public:
		int add=0;
		Asteroid();	
		void update(SDL_Renderer* renderer);
};

// Lazer class that inherits from Entity
class Lazer : public Entity {
	public:
		Lazer();			
		void update(SDL_Renderer* renderer);
};

// Score Keeping class for keeping track of score
class ScoreKeeper{
	public:
		ScoreKeeper(TTF_Font *font);       // ScoreKeeper Constructor, pass in a TTF_Font 
		unsigned int score;                // uint for keeping track of the score
		SDL_Color White = {255, 255, 255}; // SDL_Color type for rendering white text to screen
		SDL_Texture* Message1;             // Texture to use for rendering to screen
		SDL_Texture* Message2;             // Texture to use for rendering to screen
		SDL_Texture* Message3;             // Texture to use for rendering to screen
		SDL_Rect Message1Rect;             // Rectangle for size of Message1
		SDL_Rect Message2Rect;             // Rectangle for size of Message2
		SDL_Surface *tmpSurface1;          // Tmp surface for converting to textures
		SDL_Surface *tmpSurface2;          // Tmp surface for converting to textures

		void render(SDL_Renderer *renderer, TTF_Font *font);   // Renders the current score to the screen
		void gameOver(SDL_Renderer *renderer, TTF_Font *font); // Renders the game over screen with the final score
};

#endif
