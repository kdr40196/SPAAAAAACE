#pragma once
#include"collider.hpp"

int distance(SDL_Point a, SDL_Point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

bool checkCollision(SDL_Rect* a, SDL_Rect* b) {
	int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;

	leftB = b->x;
	rightB = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;

	if (topA >= bottomB || bottomA <= topB || rightA <= leftB || leftA >= rightB) return false;

	return true;
}

bool checkCollision(Circle* a, SDL_Rect* b) {
	int cX, cY;					//closest x, y
	if (a->x < b->x)
		cX = b->x;
	else if (a->x > b->x + b->w)
		cX = b->x + b->w;
	else cX = a->x;

	if (a->y < b->y)
		cY = b->y;
	else if (a->y > b->y + b->h)
		cY = b->y + b->h;
	else cY = a->y;

	if (distance({ cX, cY }, { a->x, a->y }) < a->r)
		return true;

	return false;
}


Collider::Collider() {
	colliderRect = { 0, 0, 0, 0 };
}

Collider::Collider(int x, int y, int w, int h, int angle) {
	colliderRect = { x, y, w, h };
	this->angle = angle;
}

SDL_Rect * Collider::getColliderRect() {
	return &colliderRect;
}

void Collider::move(SDL_Point position, int angle) {
	colliderRect.x = position.x;
	colliderRect.y = position.y;
	this->angle = angle;
}

bool Collider::collides(Collider* collider) {
	return checkCollision(&colliderRect, collider->getColliderRect());
}
