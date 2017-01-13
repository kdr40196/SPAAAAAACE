#pragma once

#include<SDL.h>

class Collider {
	float angle;
	protected:
		SDL_Rect colliderRect;
		SDL_Point rotatedColliderRect[4];
		SDL_Rect* colliders;
		SDL_Point** rotatedColliders;
		int noOfColliders;
	public:
		Collider();
		Collider(int x, int y, int w, int h, float angle = 0);
		SDL_Rect* getColliderRect();
		bool collides(Collider*);
		void move(SDL_Point position);
		void move(SDL_Point position, float angle);
		float getAngle();
		SDL_Point* getRotatedColliderRect();
		SDL_Point** getColliders();
		void rotate(float angle);
		int getNoOfColliders();
};

class ShipCollider : public Collider {
	public:
		ShipCollider(int x, int y, int w, int h, float angle = 0);
};

class LaserCollider : public Collider {
	public:
		LaserCollider(int x, int y, int w, int h, float angle = 0);
};

struct Circle { int x, y, r; };

bool checkCollision(SDL_Rect* a, SDL_Rect* b);
bool checkCollision(Circle* a, SDL_Rect* b);
bool checkCollision(Circle* a, SDL_Rect* b, int distanceX, int distanceY);
int distance(SDL_Point a, SDL_Point b);
