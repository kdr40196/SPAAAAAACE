#pragma once

#include"texture.hpp"

class Level {
	int levelWidth, levelHeight;
	Texture* levelTexture;
	public:
		Level(int width, int height);
		void render();
		int getWidth();
		int getHeight();
};