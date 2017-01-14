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

	gSmallFont.setFont("BANK.TTF", 20);
	gLargeFont.setFont("BANK.TTF", 40);
	if (gSmallFont.getFont() == nullptr || gLargeFont.getFont() == nullptr) {
		cout << "Unable to load font" << endl;
		return false;
	}

	gMenuItemPointer.setText(">");
	gMenuItemPointer.setFont(&gSmallFont);

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

bool loadMainMenu(Menu* mainMenu) {
	mainMenu->addMenuTitle("SPAAAAAACE");
	mainMenu->addMenuItem("PLAY");
	mainMenu->addMenuItem("EXIT");
	mainMenu->addMenuInfo("WASD TO MOVE");
	mainMenu->addMenuInfo("MOVE MOUSE TO AIM");
	mainMenu->addMenuInfo("CLICK TO SHOOT");
	mainMenu->addMenuInfo("ESC TO PAUSE");
	return true;
}

bool loadPauseMenu(Menu* pauseMenu) {
	pauseMenu->addMenuTitle("PAUSE");
	pauseMenu->addMenuItem("RESUME");
	pauseMenu->addMenuItem("RESTART");
	pauseMenu->addMenuItem("EXIT");
	return true;
}

bool loadGameOverMenu(Menu * gameOverMenu) {
	gameOverMenu->addMenuTitle("GAME OVER. SCORE: " + to_string(gScore));
	gameOverMenu->addMenuItem("PLAY AGAIN");
	gameOverMenu->addMenuItem("EXIT");
	return true;
}

bool loadGame() {
	if (!gSpriteSheet->loadTextureFromImage("images/ship.png", SDL_MapRGB(SDL_GetWindowSurface(gWindow.getReference())->format, Enemy::color.r, Enemy::color.g, Enemy::color.b)))
		return false;

	scorePrompt.setText("SCORE");
	scorePrompt.setPosition(0, 0);
	scoreText.setText("0");
	scoreText.setPosition(0, scorePrompt.getY() + scorePrompt.getTextHeight());
	healthPrompt.setText("HEALTH");
	healthPrompt.setPosition(0, scoreText.getY() + scoreText.getTextHeight());
	healthText.setText("100");
	healthText.setPosition(0, healthPrompt.getY() + healthPrompt.getTextHeight());

	return true;
}

void start(Level** level, Player** player, Camera** cam) {
	if (*level != nullptr) {
		delete *level;
	}
	if (*player != nullptr) {
		delete *player;
	}
	if (*cam != nullptr) {
		delete *cam;
	}
	*level = new Level(3000, 3000);
	*player = new Player(*level);
	*cam = new Camera();

	gSpawnedEnemies = 0;
	resetEnemy();
	if (gEnemies != nullptr) {
		for (int i = 0; i < TOTAL_ENEMIES; i++) {
			delete gEnemies[i];
		}
		delete[] gEnemies;
	}
	gEnemies = new Enemy*[TOTAL_ENEMIES];
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		gEnemies[i] = new Enemy(*level, *player);
		gEnemies[i]->spawn(*level, *cam);
	}

	gScore = 0;

	scoreText.setText("0");
	healthText.setText("100");
}

void pause(Player* player) {
	player->pause();
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		gEnemies[i]->pause();
	}
}

void resume(Player * player) {
	player->resume();
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		gEnemies[i]->resume();
	}
}

void updateLasers() {
	for (int i = 0; i < gLasers.size(); i++) {
		if (gLasers[i].getX() == -9999 || gLasers[i].getY() == -9999) {
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
	}
}

void renderInfo(Player* player) {
	scoreText.setText(to_string(gScore));

	healthText.setText(to_string(player->getHealth()));

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

void resetEnemy() {
	Enemy::maxHealth = Enemy::DEFAULT_HEALTH;
}