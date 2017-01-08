#include"space.hpp"

int main(int argc, char** argv) {
	if (!init()) return -1;

	if (!loadMedia()) return -1;
	Level level(3000, 3000);

	Player player(&level);
	Camera cam;

	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		gEnemies[i] = new Enemy(&level, &player);
		gEnemies[i]->spawn(&level, &cam);
	}

	Timer stepTimer, capTimer;
	bool quit = false;
	SDL_Event e;
	int frame = 0;
	while (!quit) {
		capTimer.start();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT) gWindow.handleEvent(e);
			else if (e.type == SDL_QUIT) quit = true;
			player.handleInput(e, &level);
		}

		float timeStep = stepTimer.getTicks() / 1000.f;

		player.update(timeStep, &level);
		cam.move(&player, &level);
		
		for (int i = 0; i < gLasers.size(); i++) {
			gLasers[i].move(timeStep, &level, &player);
		}

		stepTimer.start();
		if (!gWindow.isMinimized()) {
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 200);
			SDL_RenderClear(gRenderer);

			updateLasers();

			for (int i = 0; i < gLasers.size(); i++) {
				gLasers[i].render(&cam);
			}

			player.render(&cam);
			
			updateEnemies(timeStep, &level, &player, &cam);
			
			updateInfo(&player);

			SDL_RenderPresent(gRenderer);
		}

		float frameTicks = capTimer.getTicks();
		if (frameTicks < TICKS_PER_FRAME)
			SDL_Delay(TICKS_PER_FRAME - frameTicks);
	}

	close();
	return 0;
}
