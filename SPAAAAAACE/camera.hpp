#pragma once

#include"g.hpp"
#include"level.hpp"

class Player;
class Sprite;

class Camera {
	SDL_Rect cameraRect;
public:
	Camera();
	void move(Player*, Level*);
	SDL_Rect* getRect();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
};