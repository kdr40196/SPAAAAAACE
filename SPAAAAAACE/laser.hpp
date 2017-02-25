#pragma once

#include "sprite.hpp"

class Player;

class Laser : public Sprite {
	SDL_Point start, click;
	int xVel, yVel;
	bool playerStarted;
public:
	static const int LASER_VEL = 1000, RANGE = 1000, LASER_WIDTH = 2, LASER_HEIGHT = 28;
	Laser(int start_x, int start_y, int x, int y, int angle, Level& l, bool playerStarted);
	~Laser();
	void move(float timestep, Level& l, Player& player);
};

