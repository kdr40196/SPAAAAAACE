#pragma once

#include"texture.hpp"

class Level {
	int levelWidth, levelHeight;
	public:
		Level(int width, int height);
		int getWidth();
		int getHeight();
};