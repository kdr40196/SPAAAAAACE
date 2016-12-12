/*#pragma once

#include "laser.hpp"

Laser::Laser(int start_x, int start_y, int x, int y) {
	//add in a global linked list/vector
	
	texture = &gSpriteSheet;
	clipRect = {20, 69, 24, 2};

	start = { start_x, start_y };
	click = { x, y };

	//rotate sprite
	rotate(x, y);
}

void Laser::move(float timestep, Level* l) {
	//move as a sprite
	//check for collision with enemies
	//reduce health from enemy
	//kill laser
}
*/