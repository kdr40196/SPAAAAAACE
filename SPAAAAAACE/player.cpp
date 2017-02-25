#include"player.hpp"
#include"enemy.hpp"

const SDL_Color Player::color = { 0, 0, 255, 255 };
int Player::maxHealth = 100;

int gScore = 0;

Player::Player() {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;

	type = ShipType::SHIP_TYPE_PLAYER;

	texture = gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);

	health = maxHealth;
}

Player::Player(Level& l) {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;

	type = ShipType::SHIP_TYPE_PLAYER;
	health = maxHealth;

	texture = gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);
}

void Player::handleInput(SDL_Event& e, Level& l) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_w:
			yVel -= VEL;
			break;
		case SDLK_s:
			yVel += VEL;
			break;
		case SDLK_d:
			xVel += VEL;
			break;
		case SDLK_a:
			xVel -= VEL;
			break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_w:
			yVel += VEL;
			break;
		case SDLK_s:
			yVel -= VEL;
			break;
		case SDLK_a:
			xVel += VEL;
			break;
		case SDLK_d:
			xVel -= VEL;
			break;
		}
	}

	if (e.type == SDL_MOUSEMOTION) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		x /= (float(gWindow.getWidth()) / gScreenWidth);
		y /= (float(gWindow.getHeight()) / gScreenHeight);
		rotate(x, y, gScreenWidth / 2, gScreenHeight / 2);
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		attack(x, y, l);
	}
}

bool Player::update(float timeStep, Level& l) {
	move(timeStep, l);
	//cout << position.x << ", " << position.y << endl;

	if (health == 0) {
		return false;			//player dead
	}
	else if (health < maxHealth && hitTimer.getTicks() > REGENERATE_AFTER) {
		if (!regenerateTimer.isStarted()) {
			regenerateTimer.start();
		}
		regenerate();
	}
	return true;
}

void Player::move(float timeStep, Level& l) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	x /= (float(gWindow.getWidth()) / gScreenWidth);
	y /= (float(gWindow.getHeight()) / gScreenHeight);
	rotate(x, y, gScreenWidth / 2, gScreenHeight / 2);
	for (int iEnemies = 0; iEnemies < TOTAL_ENEMIES; iEnemies++) {
		if (collider->collides(gEnemies[iEnemies]->getCollider())) {
			gEnemies[iEnemies]->move(-timeStep - timeStep, l, *this);
			break;
		}
	}
	Ship::move(timeStep, l);
}

void Player::takeDamage() {
	Ship::takeDamage();
	hitTimer.start();
}

void Player::regenerate() {
	if (regenerateTimer.getTicks() > REGENERATE_TIMEOUT) {
		health += REGENERATE_RATE;
		regenerateTimer.start();
	}
	if (health > maxHealth) {
		health = maxHealth;
		regenerateTimer.stop();
		damaged = false;
	}
}

void Player::pause() {
	hitTimer.pause();
	regenerateTimer.pause();
}

void Player::resume() {
	hitTimer.unpause();
	regenerateTimer.unpause();
}
