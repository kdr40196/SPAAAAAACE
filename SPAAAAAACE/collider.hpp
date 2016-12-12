#pragma once

#include<SDL.h>

class Collider {
	SDL_Rect colliderRect;
	public:
		Collider();
		void init(int x, int y, int w, int h);
		SDL_Rect* getColliderRect();
		//bool collides(Collider*);
		void move(SDL_Point position);
};