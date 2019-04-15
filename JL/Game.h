#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef ASTEROIDS_H
#define ASTEROIDS_H

using namespace std;

// SDL2 texture class, may rename to animation. for actually putting our entities onto the screen

class Entity {
	public:
	SDL_Rect r;
	SDL_Texture* t;
	string name;
	bool life;

	int xPos, yPos;
	bool init(string filename, SDL_Renderer *renderer, int startX, int startY, int width, int height); // will make texture and rectangle from filename
	void update(SDL_Renderer* renderer); // will render texture to screen, also check if life is true or not, if not will delete rect and texture
	Entity();
	~Entity();

};



class Player : public Entity {
		
};


class Asteroid : public Entity {



};

class Laser : public Entity {



};


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

		void render(SDL_Renderer *renderer, TTF_Font *font);

		ScoreKeeper();
};

#endif
