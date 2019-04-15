#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef GAME_H
#define GAME_H

using namespace std;

class Entity {
	public:
	SDL_Rect r;
	SDL_Texture* t;
	string name;
	bool life;

	float xPos=0, yPos=0;
	bool init(string filename, SDL_Renderer *renderer, int startX, int startY, int width, int height); // will make texture and rectangle from filename
	Entity();
	~Entity();
	virtual void update(SDL_Renderer *){};

};



class Player : public Entity {
	public:
	Player();
	~Player();
	void update(SDL_Renderer* renderer); // will render texture to screen, also check if life is true or not, if not will delete rect and texture
};


class Asteroid : public Entity {
	// maybe need to have each entity have their own update class
	public:
	Asteroid();	
	~Asteroid();	
	void update(SDL_Renderer* renderer); // will render texture to screen, also check if life is true or not, if not will delete rect and texture


};

class Lazer : public Entity {
	public:
	Lazer();			
	~Lazer();			
	void update(SDL_Renderer* renderer); // will render texture to screen, also check if life is true or not, if not will delete rect and texture


};


class ScoreKeeper{
	public:
		unsigned int score;
		string text = "Score: ";
		SDL_Color White = {255, 255, 255};
		SDL_Texture* Message1;
		SDL_Texture* Message2;
		SDL_Texture* Message3;

		SDL_Rect Message1Rect;
		SDL_Rect Message2Rect;

		SDL_Surface *tmpSurface1;
		SDL_Surface *tmpSurface2;

		void render(SDL_Renderer *renderer, TTF_Font *font);
		void gameOver(SDL_Renderer *renderer, TTF_Font *font);

		ScoreKeeper();
		~ScoreKeeper();
};

#endif
