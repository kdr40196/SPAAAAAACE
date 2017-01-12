#pragma once

#include<SDL.h>

class Collider {
	SDL_Rect colliderRect;
	SDL_Point rotatedPoints[4];
	float angle;
	public:
		Collider();
		Collider(int x, int y, int w, int h, float angle = 0);
		SDL_Rect* getColliderRect();
		bool collides(Collider*);
		void move(SDL_Point position);
		void move(SDL_Point position, float angle);
		float getAngle();
		SDL_Point* getRotatedPoints();
		void rotate(float angle);
};

struct Circle { int x, y, r; };

bool checkCollision(SDL_Rect* a, SDL_Rect* b);
bool checkCollision(Circle* a, SDL_Rect* b);
int distance(SDL_Point a, SDL_Point b);
