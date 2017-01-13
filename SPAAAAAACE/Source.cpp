#include"space.hpp"

int main(int argc, char** argv) {
	if (!init()) return -1;

	Timer capTimer;
	Menu mainMenu;

	if (!loadMainMenu(&mainMenu)) return -1;

	bool quit = false;
	string action;
	SDL_Event e;
	
	while (!quit) {
		capTimer.start();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT) gWindow.handleEvent(e);
			else if (e.type == SDL_QUIT) quit = true;
			else action = mainMenu.handleInput(e);
		}

		if (action == "EXIT") {
			quit = true;
			continue;
		}
		else if (action == "PLAY") {
			mainMenu.exit();
			break;
		}

		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 150);
		SDL_RenderClear(gRenderer);
		mainMenu.display();
		SDL_RenderPresent(gRenderer);
		
		float frameTicks = capTimer.getTicks();
		if (frameTicks < TICKS_PER_FRAME)
			SDL_Delay(TICKS_PER_FRAME - frameTicks);
	}

	if (quit) {
		close();
		return 0;
	}

	Menu *pauseMenu = new Menu(), *gameOverMenu = nullptr;
	if (!loadGame() || !loadPauseMenu(pauseMenu)) {
		cout << "Unable to load game" << endl;
		return -1;
	}

	Level* level = nullptr;
	Player* player = nullptr;
	Camera* cam = nullptr;
	start(&level, &player, &cam);
	
	Timer stepTimer;
	bool paused = false, gameOver = false;
	while (!quit) {
		capTimer.start();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT) gWindow.handleEvent(e);
			else if (e.type == SDL_QUIT) quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					if (paused) {
						paused = false;
						pauseMenu->exit();
						resume(player);
					}
					else {
						paused = true;
						pause(player);
					}
					break;
				}
			}
			if (paused) {
				action = pauseMenu->handleInput(e);
				if (action == "RESUME") {
					paused = false;
				}
				else if (action == "RESTART") {
					start(&level, &player, &cam);
					quit = paused = gameOver = false;
					continue;
				}
				else if (action == "EXIT") {
					quit = true;
					continue;
				}
			}
			else if (gameOver) {
				action = gameOverMenu->handleInput(e);
				if (action == "PLAY AGAIN") {
					start(&level, &player, &cam);
					quit = paused = gameOver = false;
					continue;
				}
				else if (action == "EXIT") {
					quit = true;
					continue;
				}
			}

			if (!paused && !gameOver)
				player->handleInput(e, level);
		}
		
		float timeStep = stepTimer.getTicks() / 1000.f;
		
		if (!paused && !gameOver) {
			for (int i = 0; i < gLasers.size(); i++) {
				gLasers[i].move(timeStep, level, player);
			}
			updateLasers();
			updateEnemies(timeStep, level, player, cam);
			
			gameOver = !player->update(timeStep, level);
			cam->move(player, level);
		}
		
		stepTimer.start();
		
		if (!gWindow.isMinimized()) {
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderClear(gRenderer);

			for (int i = 0; i < gLasers.size(); i++) {
				gLasers[i].render(cam, level);
			}

			for (int i = 0; i < TOTAL_ENEMIES; i++) {
				gEnemies[i]->render(cam, level);
			}

			player->render(cam, level);

			renderInfo(player);

			if (paused || gameOver) {
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 150);
				SDL_RenderFillRect(gRenderer, nullptr);
				if (paused)
					pauseMenu->display();
				else if (gameOver) {
					if (gameOverMenu == nullptr) {
						gameOverMenu = new Menu();
						loadGameOverMenu(gameOverMenu);
					}
					gameOverMenu->display();
				}
			}
			SDL_RenderPresent(gRenderer);
		}

		float frameTicks = capTimer.getTicks();
		if (frameTicks < TICKS_PER_FRAME)
			SDL_Delay(TICKS_PER_FRAME - frameTicks);
	}

	close();
	return 0;
}
