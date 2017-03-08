#pragma once
#include "level.hpp"

Level::Level(int width, int height) {
	levelWidth = width, levelHeight = height;
}

int Level::getWidth() {
	return levelWidth;
}

int Level::getHeight() {
	return levelHeight;
}
