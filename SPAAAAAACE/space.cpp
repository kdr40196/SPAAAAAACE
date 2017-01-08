#pragma once
#include"space.hpp"

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Initialization failed" << endl;
		return false;
	}
	if (!gWindow.init()) {
		cout << "Window Creation failed" << endl;
		return false;
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		cout << "SDL Image initialization failed" << endl;
		return false;
	}
	if (TTF_Init() == -1) {
		cout << "TTF initialization failed" << endl;
		return false;
	}
	gRenderer = gWindow.createRenderer();
	if (gRenderer == nullptr) {
		cout << "Unable to create renderer" << endl;
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
	if (!gSpriteSheet.loadTextureFromImage("ship.png", SDL_MapRGB(SDL_GetWindowSurface(gWindow.getReference())->format, Enemy::color.r, Enemy::color.g, Enemy::color.b))) return false;

	gFont.setFont("BANK.TTF", 20);

	if (gFont.getFont() == nullptr) {
		cout << "Unable to load font" << endl;
		return false;
	}

	scorePrompt.updateText("SCORE");
	scorePrompt.updatePosition(0, 0);
	scoreText.updatePosition(0, scorePrompt.getY() + scorePrompt.getTextHeight());
	scoreText.updateText("0");
	healthPrompt.updateText("HEALTH");
	healthPrompt.updatePosition(0, scoreText.getY() + scoreText.getTextHeight());
	healthText.updatePosition(0, healthPrompt.getY() + healthPrompt.getTextHeight());
	healthText.updateText("100");
	
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

void updateInfo(Player* player) {
	scoreText.updateText(to_string(gScore));
	healthText.updateText(to_string(player->getHealth()));

	if (player->getHealth() <= 20) {
		healthText.setColor({ 200, 0, 0 });
	}
	else {
		healthText.setColor({ 255, 255, 255 });
	}

	scorePrompt.render();
	scoreText.render();
	healthPrompt.render();
	healthText.render();
}
