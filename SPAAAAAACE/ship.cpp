#include"ship.hpp"
#include"enemy.hpp"

Ship::Ship() {
	xVel = yVel = 0;
	width = SHIP_WIDTH;
	height = SHIP_HEIGHT;
}

Ship::Ship(string path) {
	position.y = gScreenHeight - Ship::SHIP_HEIGHT;
	if (!texture->loadTextureFromImage(path)) exit(-1);
	width = SHIP_WIDTH;
	height = SHIP_HEIGHT;
}

void Ship::move(float timeStep, Level& l) {
	int displacement = xVel * timeStep;

	position.x += displacement;

	if (position.x < 0) position.x = l.getWidth() - displacement;
	else if (position.x > l.getWidth()) position.x = displacement;
	collider->move(position, angle);

	//check if colliding with enemy
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (collider->collides(gEnemies[i]->getCollider())) {
			position.x -= displacement;
			collider->move(position, angle);
			break;
		}
	}

	displacement = yVel * timeStep;
	position.y += displacement;

	if (position.y < 0) position.y = l.getHeight() - displacement;
	else if (position.y > l.getHeight()) position.y = displacement;

	collider->move(position, angle);

	//check if colliding with enemy
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (collider->collides(gEnemies[i]->getCollider())) {
			position.y -= displacement;
			collider->move(position, angle);
			break;
		}
	}
}

void Ship::attack(int x, int y, Level& l) {
	bool playerStarted;
	if (type == ShipType::SHIP_TYPE_PLAYER) {
		playerStarted = true;
	}
	else playerStarted = false;

	Laser tmp((position.x + SHIP_WIDTH / 2) % 3000, position.y, x, y, angle, l, playerStarted);

	gLasers.push_back(tmp);
}

void Ship::takeDamage() {
	health -= 10;
	if (health <= 0) health = 0;
	damaged = true;

}

int Ship::getHealth() {
	return health;
}

bool Ship::isDamaged() {
	return damaged;
}

ShipType Ship::getType() {
	return type;
}
