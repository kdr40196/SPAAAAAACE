#pragma once
#include"space.hpp"

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Initialization failed", gWindow.getReference());
		return false;
	}
	if (!gWindow.init()) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Window Creation failed", gWindow.getReference());
		return false;
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Image initialization failed", gWindow.getReference());
		return false;
	}
	gRenderer = gWindow.createRenderer();
	if (gRenderer == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create renderer", gWindow.getReference());
		return false;
	}

	srand(SDL_GetTicks());

	return true;
}

void close() {
	SDL_DestroyWindow(gWindow.getReference());
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia() {
	if (!gSpriteSheet.loadTexture("ship.png", SDL_MapRGB(SDL_GetWindowSurface(gWindow.getReference())->format, Enemy::color.r, Enemy::color.g, Enemy::color.b))) return false;
	return true;
}

void updateLasers() {
	for (int i = 0; i < gLasers.size(); i++) {
		if (gLasers[i].getX() == -999 || gLasers[i].getY() == -999) {
			gLasers.erase(gLasers.begin() + i);
		}
	}
}

void updateEnemies(float timeStep, Level* level, Player* player, Camera* cam) {
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (!gEnemies[i]->isDamaged()) gEnemies[i]->upgrade();
	}
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (gEnemies[i]->getHealth() <= 0) {
			gEnemies[i]->die();
			gEnemies[i]->respawn(level, cam);
		}
		gEnemies[i]->update(timeStep, level, player);
		gEnemies[i]->render(cam);
	}
}
