#pragma once

#include"camera.hpp"
#include"sprite.hpp"

Camera::Camera() {
	cameraRect = { 0, 0, gScreenWidth, gScreenHeight };
}

void Camera::move(Player* player, Level* l) {
	cameraRect.x = (player->getX() + Ship::SHIP_WIDTH / 2) - gScreenWidth / 2;
	cameraRect.y = (player->getY() + Ship::SHIP_HEIGHT / 2) - gScreenHeight/ 2;
	//cout << cameraRect.x << ", " << cameraRect.y << endl;
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
