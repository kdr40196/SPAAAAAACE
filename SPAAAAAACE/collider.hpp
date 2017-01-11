#pragma once

#include<SDL.h>

class Collider {
	SDL_Rect colliderRect;
	int angle;
	public:
		Collider();
		Collider(int x, int y, int w, int h, int angle = 0);
		//void init(int x, int y, int w, int h);
		SDL_Rect* getColliderRect();
		//bool collides(Collider*);
		void move(SDL_Point position, int angle = 0);
};