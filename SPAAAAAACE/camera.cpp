#pragma once

#include"camera.hpp"
#include"sprite.hpp"
#include"ship.hpp"

Camera::Camera() {
	cameraRect = { 0, 0, gScreenWidth, gScreenHeight };
}

void Camera::move(Player* player, Level* l) {
	cameraRect.x = (player->getX() + Ship::SHIP_WIDTH / 2) - gScreenWidth / 2;
	cameraRect.y = (player->getY() + Ship::SHIP_HEIGHT / 2) - gScreenHeight/ 2;
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

int Camera::getWidth() {
	return cameraRect.w;
}

int Camera::getHeight() {
	return cameraRect.h;
}
