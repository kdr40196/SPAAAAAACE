/*#pragma once

#include<SDL.h>
#include"sprite.hpp"
#include"collider.hpp"

class Level;

class Laser : public Sprite {
	SDL_Point start, click;
	public:
		static const int VEL = 6000, range = 1000, WIDTH = 24, HEIGHT = 2;
		Laser(int start_x, int start_y, int x, int y);
		void move(float timestep, Level* l);
};*/