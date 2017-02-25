#include"enemy.hpp"
#include"player.hpp"

const SDL_Color Enemy::color = { 255, 0, 0, 255 };
int Enemy::maxHealth = 15;

Enemy** gEnemies = nullptr;
int gSpawnedEnemies = 0;

Enemy::Enemy() {
	id = gSpawnedEnemies;
	position.x = rand() % (gScreenWidth - SHIP_WIDTH);
	position.y = rand() % (gScreenHeight - SHIP_HEIGHT);

	texture = gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };

	health = maxHealth;
	playerDetected = false;
	type = ShipType::SHIP_TYPE_ENEMY;
	state = EnemyState::IDLE;

	attackTimer.start();

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);
}

Enemy::Enemy(Level& l, Player& p) {
	id = gSpawnedEnemies;
	texture = gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };

	health = maxHealth;
	playerDetected = false;
	type = ShipType::SHIP_TYPE_ENEMY;
	state = EnemyState::IDLE;
	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);
	attackTimer.start();
}

int Enemy::getId() {
	return id;
}

void Enemy::move(float timeStep, Level& l, Player& player) {

	if (distance(original, position) > MOVEMENT_RANGE / 2) {
		angle = (int(angle) + 180) % 360;
		xVel = -xVel, yVel = -yVel;
	}

	int xDisplacement = xVel * timeStep;
	int yDisplacement = yVel * timeStep;

	position.x += xDisplacement;
	position.y += yDisplacement;

	collider->move(position, angle);

	//going out of level
	if (position.x < 0 || position.x + SHIP_WIDTH > l.getWidth() ||
		position.y < 0 || position.y + SHIP_HEIGHT > l.getHeight()) {

		position.x -= xDisplacement, position.y -= yDisplacement;

		angle = (int(angle) + 180) % 360;
		xVel = -xVel;
		yVel = -yVel;
		collider->move(position, angle);
	}

	//check if colliding with other enemies
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (this->id != gEnemies[i]->getId() && collider->collides(gEnemies[i]->getCollider())) {

			position.x -= xDisplacement, position.y -= yDisplacement;

			angle = (int(angle) + 180) % 360;
			xVel = -xVel;
			yVel = -yVel;

			break;
		}
	}

	attackRadar.x = position.x, attackRadar.y = position.y;

}

void Enemy::attack(Player& player, Level& l) {
	if (attackTimer.getTicks() >= ATTACK_TIMEOUT) {
		int x = player.getX() + SHIP_WIDTH / 2;
		int y = player.getY() + SHIP_HEIGHT / 2;
		Ship::attack(x, y, l);
		attackTimer.start();
	}
}

void Enemy::rotate(int x, int y, Level& level) {
	int x1 = x, y1 = y;
	int minDistance = distance(position, { x, y });
	int tempDistance = distance(position, { x - level.getWidth(), y });
	if (tempDistance < minDistance) {
		x1 = x - level.getWidth();
		y1 = y;
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level.getWidth(), y });
	if (tempDistance < minDistance) {
		x1 = x + level.getWidth();
		y1 = y;
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x, y - level.getHeight() });
	if (tempDistance < minDistance) {
		x1 = x;
		y1 = y - level.getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x, y + level.getHeight() });
	if (tempDistance < minDistance) {
		x1 = x;
		y1 = y + level.getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x - level.getWidth(), y - level.getHeight() });
	if (tempDistance < minDistance) {
		x1 = x - level.getWidth();
		y1 = y - level.getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level.getWidth(), y + level.getHeight() });
	if (tempDistance< minDistance) {
		x1 = x + level.getWidth();
		y1 = y + level.getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level.getWidth(), y - level.getHeight() });
	if (tempDistance< minDistance) {
		x1 = x + level.getWidth();
		y1 = y - level.getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x - level.getWidth(), y + level.getHeight() });
	if (tempDistance< minDistance) {
		x1 = x - level.getWidth();
		y1 = y + level.getHeight();
		minDistance = tempDistance;
	}

	Sprite::rotate(x1, y1);
}

void Enemy::update(float timeStep, Level& level, Player& player) {

	//check if player detected
	if (checkCollision(&attackRadar, player.getCollider()->getColliderRect(), level.getWidth(), level.getHeight())) {
		if (state == EnemyState::IDLE || state == EnemyState::RETURNING_TO_IDLE)
			originalAngle = angle;

		rotate(player.getX() + SHIP_WIDTH / 2, player.getY() + SHIP_HEIGHT / 2, level);

		state = EnemyState::ATTACKING;
		attack(player, level);
	}
	else {
		if (state == EnemyState::ATTACKING) {
			cooldownTimer.start();
			state = EnemyState::COOLDOWN;
		}
		else if (state == EnemyState::COOLDOWN) {
			rotate(player.getX() + SHIP_WIDTH / 2, player.getY() + SHIP_HEIGHT / 2, level);
			if (cooldownTimer.getTicks() >= COOLDOWN_TIME) {
				state = EnemyState::RETURNING_TO_IDLE;
				angle = originalAngle;
				cooldownTimer.stop();
			}
		}
		else if (state == EnemyState::RETURNING_TO_IDLE) {
			EnemyState::IDLE;
			move(timeStep, level, player);
		}
		else if (state == EnemyState::IDLE)
			move(timeStep, level, player);
	}
}

void Enemy::spawn(Level& level, Camera& cam) {

	SDL_Rect shipColliderRect;					//temp collider for new ship
	SDL_Rect camRect = { cam.getX(), cam.getY(), cam.getWidth(), cam.getHeight() };

	bool success;
	do {
		success = true;
		position.x = rand() % (level.getWidth() - SHIP_WIDTH);
		position.y = rand() % (level.getHeight() - SHIP_HEIGHT);

		shipColliderRect = { position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT };

		//check if enemy is spawned in player area
		if (position.x > camRect.x && (position.x < (camRect.x + camRect.w) || position.x < (camRect.x + camRect.w) % level.getWidth())
			&& position.y > camRect.y && (position.y < (camRect.y + camRect.h) || position.y < camRect.y + camRect.h) % level.getHeight()) {

			success = false;
		}
		else {
			//check if colliding with previously spawned enemies
			for (int i = 0; i < gSpawnedEnemies; i++) {
				if (this->id != gEnemies[i]->getId() && checkCollision(&shipColliderRect, gEnemies[i]->getCollider()->getColliderRect())) {
					success = false;
					break;
				}
			}
		}
	} while (!success);

	original = position;

	originalAngle = angle = rand() % 360;
	xVel = sin(angle*M_PI / 180) * VEL;
	yVel = -cos(angle*M_PI / 180) * VEL;

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);				//collider for new ship

	attackRadar = { position.x, position.y, ATTACK_RADAR_RADIUS };

	gSpawnedEnemies++;
}

void Enemy::takeDamage() {
	Ship::takeDamage();
}

void Enemy::die() {
	gSpawnedEnemies--;
	gScore++;
	if (maxHealth < 50) {
		maxHealth++;
	}
}

void Enemy::respawn(Level& l, Camera& cam) {
	spawn(l, cam);

	health = maxHealth;
	playerDetected = false;
	type = ShipType::SHIP_TYPE_ENEMY;

	attackTimer.start();
}

void Enemy::upgrade() {
	health = maxHealth;
}

EnemyState Enemy::getState() {
	return state;
}

void Enemy::pause() {
	attackTimer.pause();
	cooldownTimer.pause();
}

void Enemy::resume() {
	attackTimer.unpause();
	cooldownTimer.unpause();
}
