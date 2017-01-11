#pragma once

#include<SDL.h>

class Collider {
	SDL_Rect colliderRect;
	int angle;
	public:
		Collider();
		Collider(int x, int y, int w, int h, int angle = 0);
		SDL_Rect* getColliderRect();
		bool collides(Collider*);
		void move(SDL_Point position, int angle = 0);
};

struct Circle { int x, y, r; };

bool checkCollision(SDL_Rect* a, SDL_Rect* b);
bool checkCollision(Circle* a, SDL_Rect* b);
int distance(SDL_Point a, SDL_Point b);
