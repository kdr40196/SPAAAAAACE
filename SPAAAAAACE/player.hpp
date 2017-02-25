#pragma once

#include"ship.hpp"

class Player : public Ship {
	Timer hitTimer, regenerateTimer;
public:
	static const SDL_Color color;
	static int maxHealth;

	//regenerate after 3 secs without a hit
	static const int REGENERATE_RATE = 1, REGENERATE_TIMEOUT = 500, REGENERATE_AFTER = 3000;

	Player();
	Player(Level&);
	void handleInput(SDL_Event& e, Level& level);
	bool update(float timeStep, Level& level);
	void move(float timeStep, Level& level);
	void takeDamage();
	void regenerate();
	void pause();
	void resume();
};