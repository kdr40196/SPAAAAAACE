#pragma once

#include"camera.hpp"
#include"sprite.hpp"

Camera::Camera() {
	cameraRect = { 0, 0, gScreenWidth, gScreenHeight };
}

void Camera::move(Player* ship, Level* l) {
	cameraRect.x = (ship->getX() + Ship::SHIP_WIDTH / 2) - gScreenWidth / 2;
	cameraRect.y = (ship->getY() + Ship::SHIP_HEIGHT / 2) - gScreenHeight/ 2;

	if (cameraRect.x < 0) cameraRect.x = 0;
	if (cameraRect.y < 0) cameraRect.y = 0;
	if (cameraRect.x > l->getWidth() - cameraRect.w) cameraRect.x = l->getWidth() - cameraRect.w;
	if (cameraRect.y > l->getHeight() - cameraRect.h) cameraRect.y = l->getHeight() - cameraRect.h;
}


SDL_Rect * Camera::getRect() {
	return &cameraRect;
}

int Camera::getX() {
	return cameraRect.x;
}

int Camera::getY() {
	return cameraRect.y;
}
