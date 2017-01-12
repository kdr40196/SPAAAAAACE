#pragma once
#include"collider.hpp"
#include<iostream>
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
			SDL_Point normal = { p2.y - p1.y, p1.x - p2.x };
			int minA = -9999, maxA = -9999;
			for (int jPoints = 0; jPoints < 4; jPoints++) {
				int projected = normal.x * a[jPoints].x + normal.y * a[jPoints].y;
				if (minA == -9999 || projected < minA)
					minA = projected;
				if (maxA == -9999 || projected > maxA)
					maxA = projected;
			}
			int minB = -9999, maxB = -9999;
			for (int jPoints = 0; jPoints < 4; jPoints++) {
				int projected = normal.x * b[jPoints].x + normal.y * b[jPoints].y;
				if (minB == -9999 || projected < minB)
					minB = projected;
				if (maxB == -9999 || projected > maxB)
					maxB = projected;
			}
			if (maxA < minB || minA > maxB)
				return false;
		}
	}
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


Collider::Collider() {
	colliderRect = { 0, 0, 0, 0 };
	rotatedPoints[0] = { colliderRect.x, colliderRect.y };
	rotatedPoints[1] = { colliderRect.x + colliderRect.w, colliderRect.y };
	rotatedPoints[2] = { colliderRect.x + colliderRect.w, colliderRect.y  + colliderRect.h};
	rotatedPoints[3] = { colliderRect.x, colliderRect.y + colliderRect.h };
	angle = 0;
}

Collider::Collider(int x, int y, int w, int h, float angle) {
	colliderRect = { x, y, w, h };
	this->angle = angle;
	rotate(angle);
}

SDL_Rect* Collider::getColliderRect() {
	return &colliderRect;
}

void Collider::move(SDL_Point position) {
	colliderRect.x = position.x;
	colliderRect.y = position.y;
	rotate(angle);
}

void Collider::move(SDL_Point position, float angle) {
	move(position);
	this->angle = angle;
}

float Collider::getAngle() {
	return angle;
}

SDL_Point* Collider::getRotatedPoints() {
	return rotatedPoints;
}

void Collider::rotate(float angle) {
	angle = angle*M_PI / 180;

	//center coordinates
	int cX = colliderRect.x + colliderRect.w / 2, cY = colliderRect.y + colliderRect.h / 2;

	int tempX = colliderRect.x - cX, tempY = colliderRect.y - cY;
	rotatedPoints[0].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedPoints[0].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = (colliderRect.x + colliderRect.w) - cX, tempY = colliderRect.y - cY;
	rotatedPoints[1].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedPoints[1].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = colliderRect.x - cX, tempY = (colliderRect.y + colliderRect.h) - cY;
	rotatedPoints[2].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedPoints[2].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;

	tempX = (colliderRect.x + colliderRect.w) - cX, tempY = (colliderRect.y + colliderRect.h) - cY;
	rotatedPoints[3].x = (tempX * cos(angle) - tempY * sin(angle)) + cX;
	rotatedPoints[3].y = (tempX * sin(angle) + tempY * cos(angle)) + cY;
}

bool Collider::collides(Collider* collider) {
	//SAT
	SDL_Point* collidingPoints = new SDL_Point[4];
	collidingPoints = collider->getRotatedPoints();
	if (checkCollisionSAT(rotatedPoints, collidingPoints)) {
		return true;
	}
	return false;
}
