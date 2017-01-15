#pragma once
#include"sprite.hpp"
#include"camera.hpp"
#include"g.hpp"
#include"text.hpp"
#include<cmath>

shared_ptr<Texture> gSpriteSheet = make_shared<Texture>();
Enemy** gEnemies = nullptr;
vector<Laser> gLasers;
int gSpawnedEnemies = 0, gScore = 0;

const SDL_Color Player::color = {0, 0, 255, 255};
const SDL_Color Enemy::color = { 255, 0, 0, 255 };
int Player::maxHealth = 100;
int Enemy::maxHealth = 15;


Sprite::Sprite() {
	angle = 0;
	texture = make_shared<Texture>(Texture());
	width = 0;
	height = 0;
	collider = nullptr;
}

Sprite::Sprite(string path) {
	angle = 0;
	texture = make_shared<Texture>(Texture());
	if (!texture->loadTextureFromImage(path)) exit(-1);
	width = texture->getWidth();
	height = texture->getHeight();
	collider = nullptr;
}

Sprite::Sprite(Texture texture) {
	angle = 0;
	this->texture = make_shared<Texture>(texture);
	width = texture.getWidth();
	height = texture.getHeight();
	collider = nullptr;
}

Sprite::Sprite(Text text) {
	angle = 0;
	texture = text.getTextTexture();
	width = texture->getWidth();
	height = texture->getHeight();
	collider = nullptr;
}

Sprite::~Sprite() {
	width = height = angle = 0;
	texture.reset();
	collider->getColliders();
	collider = nullptr;
}


void Sprite::render(Camera* cam, Level* l) {
	if (cam != nullptr) {
		int x = position.x - cam->getX(), y = position.y - cam->getY();
		if (x + width> l->getWidth()) {
			x -= l->getWidth();
		}
		else if (x + width < 0) {
			x += l->getWidth();
		}
		if (y + height> l->getHeight()) {
			y -= l->getHeight();
		}
		else if (y + height < 0) {
			y += l->getWidth();
		}

		texture->render(x, y, &clipRect, angle);
	}
	else texture->render(position.x, position.y, &clipRect, angle);
}

void Sprite::rotate(int x1, int y1) {
	int x2 = position.x + width / 2;
	int y2 = position.y + height / 2;
	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
	collider->rotate(angle);
}

void Sprite::rotate(int x1, int y1, Level* l) {
	int x2 = position.x + width / 2;
	int y2 = position.y + height / 2;

	if (abs(x1 - x2) > abs(x1 - (x2 - l->getWidth())))
		x2 = x2 - l->getWidth();
	if (abs(x1 - x2) > abs(x1 - (x2 + l->getWidth())))
		x2 = x2 + l->getWidth();

	if (abs(y1 - y2) > abs(y1 - (y2 - l->getHeight())))
		y2 = y2 - l->getHeight();
	if (abs(y1 - y2) > abs(y1 - (y2 + l->getHeight())))
		y2 = y2 + l->getHeight();

	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
	collider->rotate(angle);
}

void Sprite::rotate(int x1, int y1, int x2, int y2) {
	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
}

int Sprite::getX() {
	return position.x;
}	

int Sprite::getY() {
	return position.y;
}

Collider* Sprite::getCollider() {
	return collider;
}

int Sprite::getWidth() {
	return width;
}

int Sprite::getHeight() {
	return height;
}


Laser::Laser(int start_x, int start_y, int x, int y, int angle, Level* l, bool playerStarted) {

	this->playerStarted = playerStarted;
	texture = gSpriteSheet;
	clipRect = { 31, 56, LASER_WIDTH, LASER_HEIGHT };

	start = { start_x, start_y };
	position = { start_x, start_y };
	click = { x, y };

	width = LASER_WIDTH, height = LASER_HEIGHT;
	this->angle = angle;
	collider = new LaserCollider(start_x, start_y, LASER_WIDTH, LASER_HEIGHT, angle);

	xVel = sin(angle*M_PI/180) * LASER_VEL;
	yVel = -cos(angle*M_PI/180) * LASER_VEL;
}

Laser::~Laser() { }

void Laser::move(float timestep, Level* l, Player* player) {

	int xDisplacement = xVel * timestep, yDisplacement = yVel * timestep;

	position.x += xDisplacement;
	position.y += yDisplacement;


	bool dead = true;
	if (distance(start, position) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x - l->getWidth(), position.y }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x + l->getWidth(), position.y }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x, position.y - l->getHeight() }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x, position.y + l->getHeight() }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x - l->getWidth(), position.y - l->getHeight() }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x + l->getWidth(), position.y + l->getHeight() }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x + l->getWidth(), position.y - l->getHeight() }) < RANGE) {
		dead = false;
	}
	else if (distance(start, { position.x - l->getWidth(), position.y + l->getHeight() }) < RANGE) {
		dead = false;
	}

	if (dead) {
		position = { MINUS_INFINITY, MINUS_INFINITY };
		return;
	}
	else {
		if (position.x < 0) {
			position.x = l->getWidth();
			position.y -= yDisplacement;
		}
		else if (position.x > l->getWidth()) {
			position.x = 0;
			position.y -= yDisplacement;
		}
		if (position.y < 0) {
			position.y = l->getHeight();
			position.x -= xDisplacement;
		}
		else if (position.y > l->getHeight()) {
			position.y = 0;
			position.x -= xDisplacement;
		}
	}

	collider->move(position);

	if (playerStarted) {
		for (int i = 0; i < TOTAL_ENEMIES; i++) {
			if (collider->collides(gEnemies[i]->getCollider())) {
				gEnemies[i]->takeDamage();
				position.x = position.y = MINUS_INFINITY;
				break;
			}
		}
	}
	else {
		if (collider->collides(player->getCollider())) {
			player->takeDamage();
			position.x = position.y = MINUS_INFINITY;
		}
	}
}



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

void Ship::move(float timeStep, Level* l) {
	int displacement = xVel * timeStep;
	
	position.x += displacement;

	if (position.x < 0) position.x = l->getWidth() - displacement;
	else if (position.x > l->getWidth()) position.x = displacement;
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
	
	if (position.y < 0) position.y = l->getHeight() - displacement;
	else if (position.y > l->getHeight()) position.y = displacement;

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

void Ship::attack(int x, int y, Level* l) {
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


Player::Player() {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;
	
	type = ShipType::SHIP_TYPE_PLAYER;

	texture = gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);

	health = maxHealth;
}

Player::Player(Level* l) {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;
	
	type = ShipType::SHIP_TYPE_PLAYER;
	health = maxHealth;

	texture = gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider = new ShipCollider(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT, angle);
}

void Player::handleInput(SDL_Event& e, Level* l) {
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
		rotate(x, y, gScreenWidth / 2, gScreenHeight / 2);
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		attack(x, y, l);
	}
}

bool Player::update(float timeStep, Level * l) {
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

void Player::move(float timeStep, Level *l) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	rotate(x, y, gScreenWidth / 2, gScreenHeight / 2);
	for (int iEnemies = 0; iEnemies < TOTAL_ENEMIES; iEnemies++) {
		if (collider->collides(gEnemies[iEnemies]->getCollider())) {
			gEnemies[iEnemies]->move(-timeStep - timeStep, l, this);
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

Enemy::Enemy(Level* l, Player* p) {
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

void Enemy::move(float timeStep, Level* l, Player* player) {

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
	if (position.x < 0 || position.x + SHIP_WIDTH > l->getWidth() || 
		position.y < 0 || position.y + SHIP_HEIGHT > l->getHeight()) {
		
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

void Enemy::attack(Player* player, Level* l) {
	if (attackTimer.getTicks() >= ATTACK_TIMEOUT) {
		int x = player->getX() + SHIP_WIDTH / 2;
		int y = player->getY() + SHIP_HEIGHT / 2;
		Ship::attack(x, y, l);
		attackTimer.start();
	}
}

void Enemy::rotate(int x, int y, Level* level) {
	int x1 = x, y1 = y;
	int minDistance = distance(position, { x, y });
	int tempDistance = distance(position, { x - level->getWidth(), y });
	if (tempDistance < minDistance) {
		x1 = x - level->getWidth();
		y1 = y;
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level->getWidth(), y });
	if (tempDistance < minDistance) {
		x1 = x + level->getWidth();
		y1 = y;
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x, y - level->getHeight() });
	if (tempDistance < minDistance) {
		x1 = x;
		y1 = y - level->getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x, y + level->getHeight() });
	if (tempDistance < minDistance) {
		x1 = x;
		y1 = y + level->getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x - level->getWidth(), y - level->getHeight() });
	if (tempDistance < minDistance) {
		x1 = x - level->getWidth();
		y1 = y - level->getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level->getWidth(), y + level->getHeight() });
	if (tempDistance< minDistance) {
		x1 = x + level->getWidth();
		y1 = y + level->getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x + level->getWidth(), y - level->getHeight() });
	if (tempDistance< minDistance) {
		x1 = x + level->getWidth();
		y1 = y - level->getHeight();
		minDistance = tempDistance;
	}
	tempDistance = distance(position, { x - level->getWidth(), y + level->getHeight() });
	if (tempDistance< minDistance) {
		x1 = x - level->getWidth();
		y1 = y + level->getHeight();
		minDistance = tempDistance;
	}

	Sprite::rotate(x1, y1);
}

void Enemy::update(float timeStep, Level* level, Player* player) {

	//check if player detected
	if (checkCollision(&attackRadar, player->getCollider()->getColliderRect(), level->getWidth(), level->getHeight())) {
		if (state == EnemyState::IDLE || state == EnemyState::RETURNING_TO_IDLE)
			originalAngle = angle;

		rotate(player->getX() + SHIP_WIDTH / 2, player->getY() + SHIP_HEIGHT / 2, level);

		state = EnemyState::ATTACKING;
		attack(player, level);
	}
	else {
		if (state == EnemyState::ATTACKING) {
			cooldownTimer.start();
			state = EnemyState::COOLDOWN;
		}
		else if(state == EnemyState::COOLDOWN){
			rotate(player->getX() + SHIP_WIDTH / 2, player->getY() + SHIP_HEIGHT / 2, level);
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
		else if(state == EnemyState::IDLE)
			move(timeStep, level, player);
	}
}

void Enemy::spawn(Level* level, Camera* cam) {
	
	SDL_Rect shipColliderRect;					//temp collider for new ship
	SDL_Rect camRect = { cam->getX(), cam->getY(), cam->getWidth(), cam->getHeight() };

	bool success;
	do {
		success = true;
		position.x = rand() % (level->getWidth() - SHIP_WIDTH);
		position.y = rand() % (level->getHeight() - SHIP_HEIGHT);

		shipColliderRect = { position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT };
		
		//check if enemy is spawned in player area
		if(position.x > camRect.x && (position.x < (camRect.x + camRect.w) || position.x < (camRect.x + camRect.w) % level->getWidth())
			&& position.y > camRect.y && (position.y < (camRect.y + camRect.h) || position.y < camRect.y + camRect.h) % level->getHeight()) {
			
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

void Enemy::respawn(Level* l, Camera* cam) {
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
