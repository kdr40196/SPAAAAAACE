#pragma once
#include"collider.hpp"


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



/*bool Collider::collides(Collider* collider) {
	SDL_Rect* b = collider->getColliderRect();
	int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
	leftA = rect.x;
	rightA = rect.x + rect.w;
	topA = rect.y;
	bottomA = rect.y + rect.h;

	leftB = b->x;
	rightB = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;

	if (topA >= bottomB || bottomA <= topB || rightA <= leftB || leftA >= rightB) return false;
	return true;
}*/
