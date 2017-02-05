#pragma once
#include"collider.hpp"
#include"g.hpp"
#include<cmath>

int distance(SDL_Point a, SDL_Point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

//check collision between normal rectangles
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

//check collision between rotated rectangles
bool checkCollisionSAT(SDL_Point* a, SDL_Point* b) {

	//Separating Axis Theorem
	for (int iShape = 0; iShape < 2; iShape++) {
		SDL_Point* currentShape = nullptr;
		(iShape == 0) ? currentShape = a : currentShape = b;
		for (int iPoints = 0; iPoints < 4; iPoints++) {
			int iPoints2 = (iPoints + 1) % 4;
			SDL_Point p1 = currentShape[iPoints], p2 = currentShape[iPoints2];
			SDL_Point normal = { p1.y - p2.y, p1.x - p2.x };
			int minA = MINUS_INFINITY, maxA = MINUS_INFINITY;
			for (int jPoints = 0; jPoints < 4; jPoints++) {
				int projected = normal.x * a[jPoints].x + normal.y * a[jPoints].y;
				if (minA == MINUS_INFINITY || projected < minA)
					minA = projected;
				if (maxA == MINUS_INFINITY || projected > maxA)
					maxA = projected;
			}
			int minB = MINUS_INFINITY, maxB = MINUS_INFINITY;
			for (int jPoints = 0; jPoints < 4; jPoints++) {
				int projected = normal.x * b[jPoints].x + normal.y * b[jPoints].y;
				if (minB == MINUS_INFINITY || projected < minB)
					minB = projected;
				if (maxB == MINUS_INFINITY || projected > maxB)
					maxB = projected;
			}
			if (maxA < minB || minA > maxB)
				return false;
		}
	}
	return true;
}

//check collision between a circle and a normal rectangle
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

bool checkCollision(Circle* a, SDL_Rect* b, int distanceX, int distanceY) {
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
	else if (distance({ cX - (distanceX - b->w), cY }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX, cY - (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX - (distanceX - b->w), cY - (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX + (distanceX - b->w), cY }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX, cY + (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX + (distanceX - b->w), cY + (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX + (distanceX - b->w), cY - (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	else if (distance({ cX - (distanceX - b->w), cY + (distanceY - b->h) }, { a->x, a->y }) < a->r)
		return true;
	return false;
}

Collider::Collider() {
	colliderRect = { 0, 0, 0, 0 };
	rotatedColliderRect[0] = { colliderRect.x, colliderRect.y };
	rotatedColliderRect[1] = { colliderRect.x + colliderRect.w, colliderRect.y };
	rotatedColliderRect[2] = { colliderRect.x + colliderRect.w, colliderRect.y  + colliderRect.h};
	rotatedColliderRect[3] = { colliderRect.x, colliderRect.y + colliderRect.h };
	angle = 0;
	noOfColliders = 0;
	colliders = nullptr;
	rotatedColliders = nullptr;
}

Collider::Collider(int x, int y, int w, int h, float angle) {
	colliderRect = { x, y, w, h };
	this->angle = angle;
	noOfColliders = 1;
	colliders = nullptr;
	rotatedColliders = nullptr;
}

Collider::~Collider() {
	delete[] colliders;
	colliders = nullptr;
	for (int i = 0; i < noOfColliders; i++) {
		delete[] rotatedColliders;
		rotatedColliders[i] = nullptr;
	}
	delete[] rotatedColliders;
	rotatedColliders = nullptr;
}

SDL_Rect* Collider::getColliderRect() {
	return &colliderRect;
}

void Collider::move(SDL_Point position) {
	colliderRect.x = position.x;
	colliderRect.y = position.y;
	
	//move colliders
	int r = 0;
	for (int iColliders = 0; iColliders < noOfColliders; iColliders++) {
		colliders[iColliders].x = position.x + (colliderRect.w - colliders[iColliders].w) / 2;
		colliders[iColliders].y = position.y + r;
		r += colliders[iColliders].h;
	}
	rotate(angle);
}

void Collider::move(SDL_Point position, float angle) {
	this->angle = angle;
	move(position);
}

float Collider::getAngle() {
	return angle;
}

SDL_Point* Collider::getRotatedColliderRect() {
	return rotatedColliderRect;
}

SDL_Point ** Collider::getColliders() {
	return rotatedColliders;
}

void Collider::rotate(float angle) {
	angle = angle*M_PI / 180;

	//center coordinates
	int cX = colliderRect.x + colliderRect.w / 2, cY = colliderRect.y + colliderRect.h / 2;

	int tempX = colliderRect.x - cX, tempY = colliderRect.y - cY;
	rotatedColliderRect[0].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedColliderRect[0].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = (colliderRect.x + colliderRect.w) - cX, tempY = colliderRect.y - cY;
	rotatedColliderRect[1].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedColliderRect[1].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = (colliderRect.x + colliderRect.w) - cX, tempY = (colliderRect.y + colliderRect.h) - cY;
	rotatedColliderRect[2].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedColliderRect[2].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = colliderRect.x - cX, tempY = (colliderRect.y + colliderRect.h) - cY;
	rotatedColliderRect[3].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedColliderRect[3].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	//update rotatedColliders
	for (int iColliders = 0; iColliders < noOfColliders; iColliders++) {
		cX = colliders[iColliders].x + colliders[iColliders].w / 2, cY = colliders[iColliders].y + colliders[iColliders].h / 2;

		tempX = colliders[iColliders].x - cX, tempY = colliders[iColliders].y - cY;
		rotatedColliders[iColliders][0].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
		rotatedColliders[iColliders][0].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

		tempX = (colliders[iColliders].x + colliders[iColliders].w) - cX, tempY = colliders[iColliders].y - cY;
		rotatedColliders[iColliders][1].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
		rotatedColliders[iColliders][1].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

		tempX = (colliders[iColliders].x + colliders[iColliders].w) - cX, tempY = (colliders[iColliders].y + colliders[iColliders].h) - cY;
		rotatedColliders[iColliders][2].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
		rotatedColliders[iColliders][2].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

		tempX = colliders[iColliders].x - cX, tempY = (colliders[iColliders].y + colliders[iColliders].h) - cY;
		rotatedColliders[iColliders][3].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
		rotatedColliders[iColliders][3].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;
	}
}

int Collider::getNoOfColliders() {
	return noOfColliders;
}

bool Collider::collides(Collider* anotherCollider) {
	SDL_Point* collidingPoints = new SDL_Point[4];

	collidingPoints = anotherCollider->getRotatedColliderRect();

	if (checkCollisionSAT(rotatedColliderRect, collidingPoints)) {
		//check per pixel collision
		SDL_Point** collidingColliders = anotherCollider->getColliders();
		for (int iColliders = 0; iColliders < noOfColliders; iColliders++) {
			for (int jColliders = 0; jColliders < anotherCollider->getNoOfColliders(); jColliders++) {
				if (checkCollisionSAT(rotatedColliders[iColliders], collidingColliders[jColliders]))
					return true;
			}
		}
	}
	return false;
}

ShipCollider::ShipCollider(int x, int y, int w, int h, float angle)
	: Collider(x, y, w, h, angle) {

	noOfColliders = 13;
	colliders = new SDL_Rect[noOfColliders];
	
	//initialize colliders - with screen position
	colliders[0] = { x + 31, y + 0, 2, 2 };
	colliders[1] = { x + 30, y + 2, 4, 2 };
	colliders[2] = { x + 28, y + 4, 8, 2 };
	colliders[3] = { x + 26, y + 6, 12, 2 };
	colliders[4] = { x + 20, y + 8, 24, 2 };
	colliders[5] = { x + 18, y + 10, 28, 2 };
	colliders[6] = { x + 16, y + 12, 32, 2 };
	colliders[7] = { x + 14, y + 14, 36, 2 };
	colliders[8] = { x + 10, y + 16, 44, 4 };
	colliders[9] = { x + 8, y + 18, 48, 2 };
	colliders[10] = { x + 4, y + 20, 56, 2 };
	colliders[11] = { x + 2, y + 24, 60, 2 };
	colliders[12] = { x + 0, y + 26, 64, 2 };

	rotatedColliders = new SDL_Point*[noOfColliders];
	for (int iColliders = 0; iColliders < noOfColliders; iColliders++) {
		rotatedColliders[iColliders] = new SDL_Point[4];
	}
	rotate(angle);
}

LaserCollider::LaserCollider(int x, int y, int w, int h, float angle)
	: Collider(x, y, w, h, angle) {

	noOfColliders = 1;
	colliders = new SDL_Rect[noOfColliders];
	colliders[0] = colliderRect;

	rotatedColliders = new SDL_Point*[noOfColliders];
	for (int iColliders = 0; iColliders < noOfColliders; iColliders++) {
		rotatedColliders[iColliders] = new SDL_Point[4];
	}
	rotate(angle);
}
