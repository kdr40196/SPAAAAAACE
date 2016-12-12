#pragma once
#include "level.hpp"

Level::Level(int width, int height) {
	levelWidth = width, levelHeight = height;
}

void Level::render() {
	levelTexture.render(0, 0);
}

int Level::getWidth() {
	return levelWidth;
}

int Level::getHeight() {
	return levelHeight;
}
