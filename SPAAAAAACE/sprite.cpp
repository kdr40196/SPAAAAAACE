#pragma once
#include"sprite.hpp"
#include"camera.hpp"
#include"g.hpp"
#include<cmath>
#include<typeinfo>

Texture gSpriteSheet;
Enemy* gEnemies[TOTAL_ENEMIES];
vector<Laser> gLasers;
int gSpawnedEnemies = 0, gScore = 0;

const SDL_Color Player::color = {0, 0, 255, 255};
const SDL_Color Enemy::color = { 255, 0, 0, 255 };
int Player::maxHealth = 100;
int Enemy::maxHealth = 15;

int distance(SDL_Point a, SDL_Point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

bool checkCollision(SDL_Rect* a, SDL_Rect* b) {
	int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;

	leftB = b->x;
	rightB = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;

	if (topA >= bottomB || bottomA <= topB || rightA <= leftB || leftA >= rightB) return false;
	
	return true;
}

bool checkCollision(Circle* a, SDL_Rect* b) {
	int cX, cY;					//closest x, y

	if (a->x < b->x)
		cX = b->x;
	else if (a->x > b->x + b->w)
		cX = b->x + b->w;
	else cX = a->x;

	if (a->y < b->y)
		cY = b->y;
	else if (a->y > b->y + b->h)
		cY = b->y + b->h;
	else cY = a->y;

	if (distance({ cX, cY }, { a->x, a->y }) < a->r)
		return true;

	return false;
}



Sprite::Sprite() {
	angle = 0;
	texture = new Texture();
	width = 0;
	height = 0;
}

Sprite::Sprite(string path) {
	angle = 0;
	texture = new Texture();
	if (!texture->loadTexture(path)) exit(-1);
	width = texture->getWidth();
	height = texture->getHeight();
}

void Sprite::render(Camera* cam) {
	/*SDL_Rect renderRect;
	renderRect.x = position.x;
	renderRect.y = position.y;
	renderRect.w = texture.getWidth();
	renderRect.h = texture.getHeight();*/
	if (cam != nullptr) {
		texture->render(position.x - cam->getX(), position.y - cam->getY(), &clipRect, angle);
	}
	else texture->render(position.x, position.y, &clipRect, angle);
}

void Sprite::rotate(int x1, int y1) {
	int x2 = position.x + width / 2;
	int y2 = position.y + height / 2;
	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
}

void Sprite::rotate(int x1, int y1, Level* l) {
	int x2, y2;
	if (position.x < gScreenWidth / 2)
		x2 = position.x + width / 2;
	else if (position.x > l->getWidth() - gScreenWidth / 2)
		x2 = (gScreenWidth + position.x - l->getWidth()) + width / 2;
	else x2 = gScreenWidth / 2;

	if (position.y < gScreenHeight / 2)
		y2 = position.y + height / 2;
	else if (position.y > l->getHeight() - gScreenHeight / 2)
		y2 = (gScreenHeight + position.y - l->getHeight()) + height / 2;
	else y2 = gScreenHeight / 2;

	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
}

int Sprite::getX(bool getScreenPos) {
	/*if (normalized) {

	}
	else*/
		return position.x;
}	

int Sprite::getY(bool getScreenPos) {
	/*if (normalized) {

	}
	else*/
		return position.y;
}

Collider * Sprite::getCollider() {
	return &collider;
}

/*int Sprite::getWidth() {
	return texture->getWidth();
}

int Sprite::getHeight() {
	return texture->getHeight();
}*/




Laser::Laser(int start_x, int start_y, int x, int y, Level* l, bool playerStarted) {

	this->playerStarted = playerStarted;
	texture = &gSpriteSheet;
	clipRect = { 31, 56, LASER_WIDTH, LASER_HEIGHT };

	start = { start_x, start_y };
	position = {start_x, start_y};
	click = { x, y };

	width = LASER_WIDTH, height = LASER_HEIGHT;

	//rotate sprite
	if(playerStarted)
		rotate(x, y, l);
	else rotate(x, y);

	xVel = sin(angle*M_PI/180) * LASER_VEL;
	yVel = -cos(angle*M_PI/180) * LASER_VEL;

	collider.init(start_x, start_y, LASER_WIDTH, LASER_HEIGHT);
}

Laser::~Laser() { }

void Laser::move(float timestep, Level* l, Player* player) {

	position.x += xVel * timestep;
	position.y += yVel * timestep;

	collider.move(position);

	if (playerStarted) {
		for (int i = 0; i < TOTAL_ENEMIES; i++) {
			if (checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
				gEnemies[i]->takeDamage();
				position.x = position.y = -999;
				break;
			}
		}
	}
	else {
		if (checkCollision(collider.getColliderRect(), player->getCollider()->getColliderRect())) {
			player->takeDamage();
			position.x = position.y = -999;
			cout << "Health: " << player->getHealth() << endl;
		}
	}

	if (distance(start, position) > RANGE || position.x < 0 || position.y < 0 || position.x > l->getWidth() || position.y > l->getHeight())
		position.x = position.y = -999;
}




Ship::Ship() {
	xVel = yVel = 0;
	width = SHIP_WIDTH;
	height = SHIP_HEIGHT;
}

Ship::Ship(string path) {
	position.y = gScreenHeight - Ship::SHIP_HEIGHT;
	if (!texture->loadTexture(path)) exit(-1);
	width = SHIP_WIDTH;
	height = SHIP_HEIGHT;
}

void Ship::move(float timeStep, Level* l) {
	int displacement = xVel * timeStep;
	
	position.x += displacement;
	collider.move(position);
	
	if (position.x < 0 || position.x + SHIP_WIDTH > l->getWidth()) position.x -= displacement;

	//check if colliding with enemy
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
			position.x -= displacement;
			collider.move(position);
			break;
		}
	}
	
	displacement = yVel * timeStep;
	position.y += displacement;
	
	collider.move(position);
	
	if (position.y < 0 || position.y + SHIP_HEIGHT > l->getHeight()) position.y -= displacement;
	
	//check if colliding with enemy
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
			position.y -= displacement;
			collider.move(position);
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

	Laser tmp(position.x + SHIP_WIDTH / 2, position.y, x, y, l, playerStarted);

	gLasers.push_back(tmp);
}

void Ship::takeDamage() {
	health -= 10;
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

	texture = &gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);

	health = maxHealth;
}

Player::Player(Level* l) {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;
	
	type = ShipType::SHIP_TYPE_PLAYER;
	health = maxHealth;

	texture = &gSpriteSheet;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);
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
		rotate(x, y, l);
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		attack(x, y, l);
	}
}

void Player::update(float timeStep, Level * l) {
	move(timeStep, l);
	if (health < maxHealth && hitTimer.getTicks() > REGENERATE_AFTER) {
		regenerate(timeStep);
	}
}

void Player::move(float timeStep, Level *l) {
	Ship::move(timeStep, l);

	int x, y;
	SDL_GetMouseState(&x, &y);
	rotate(x, y, l);
}

void Player::takeDamage() {
	Ship::takeDamage();
	hitTimer.start();
}

void Player::regenerate(float timeStep) {
	health += timeStep * ((maxHealth - health) / 100) * REGENERATE;
	if (health > maxHealth) health = maxHealth;
	//cout << health << endl;
}



Enemy::Enemy() {
	id = gSpawnedEnemies;
	position.x = rand() % (gScreenWidth - SHIP_WIDTH);
	position.y = rand() % (gScreenHeight - SHIP_HEIGHT);

	texture = &gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };

	health = maxHealth;
	playerDetected = false;
	type = ShipType::SHIP_TYPE_ENEMY;
	state = EnemyState::IDLE;

	attackTimer.start();

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);
}

Enemy::Enemy(Level* l, Player* p) {
	id = gSpawnedEnemies;
	texture = &gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };

	health = maxHealth;
	playerDetected = false;
	type = ShipType::SHIP_TYPE_ENEMY;
	state = EnemyState::IDLE;

	attackTimer.start();
}

int Enemy::getId() {
	return id;
}

void Enemy::move(float timeStep, Level* l, Player* player) {

	if (distance(original, position) > MOVEMENT_RANGE / 2) {
		angle = (angle + 180) % 360;
		xVel = -xVel, yVel = -yVel;
		//move(timeStep, l, player);
	}

	int xDisplacement = xVel * timeStep;
	int yDisplacement = yVel * timeStep;

	position.x += xDisplacement;
	position.y += yDisplacement;

	collider.move(position);

	//if(collidiing with player || going out of level) decrement x, y and update collider
	if (position.x < 0 || position.x + SHIP_WIDTH > l->getWidth() || 
		position.y < 0 || position.y + SHIP_HEIGHT > l->getHeight()) {
		
		position.x -= xDisplacement, position.y -= yDisplacement;

		angle = (angle + 180) % 360;
		xVel = -xVel;
		yVel = -yVel;
		collider.move(position);
	}

	//check if colliding with other enemies
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (this->id != gEnemies[i]->getId() && checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
			
			position.x -= xDisplacement, position.y -= yDisplacement;

			angle = (angle + 180) % 360;
			xVel = -xVel;
			yVel = -yVel;

			break;
		}
	}

	//check if colliding with player
	if (checkCollision(collider.getColliderRect(), player->getCollider()->getColliderRect())) {
		position.x -= xDisplacement, position.y -= yDisplacement;

		angle = (angle + 180) % 360;
		xVel = -xVel;
		yVel = -yVel;
		collider.move(position);
	}

	radar.x = position.x, radar.y = position.y;

}

void Enemy::attack(Player* player, Level* l) {
	if (attackTimer.getTicks() >= ATTACK_TIMEOUT) {
		int x = player->getX() + SHIP_WIDTH / 2;
		int y = player->getY() + SHIP_HEIGHT / 2;
		Ship::attack(x, y, l);
		attackTimer.start();
	}
}

void Enemy::update(float timeStep, Level* level, Player* player) {

	//check if player detected
	if (checkCollision(&radar, player->getCollider()->getColliderRect())) {
		if (state == EnemyState::IDLE || state == EnemyState::RETURNING_TO_IDLE)
			originalAngle = angle;

		rotate(player->getX() + SHIP_WIDTH / 2, player->getY() + SHIP_HEIGHT / 2);
		state = EnemyState::ATTACKING;
		attack(player, level);
	}
	else {
		if (state == EnemyState::ATTACKING) {
			state = EnemyState::RETURNING_TO_IDLE;
			angle = originalAngle;
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
	bool success;
	do {
		success = true;
		position.x = rand() % (level->getWidth() - SHIP_WIDTH);
		position.y = rand() % (level->getHeight() - SHIP_HEIGHT);

		shipColliderRect = { position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT };
		
		//check if enemy is spawned in player area
		if (!checkCollision(&shipColliderRect, cam->getRect())) {
			
			//check if colliding with previously spawned enemies
			for (int i = 0; i < gSpawnedEnemies; i++) {
				if (this->id != gEnemies[i]->getId() && checkCollision(&shipColliderRect, gEnemies[i]->getCollider()->getColliderRect())) {
					success = false;
					break;
				}
			}
		}
		else success = false;
	} while (!success);

	original = position;

	originalAngle = angle = rand() % 360;
	xVel = sin(angle*M_PI / 180) * VEL;
	yVel = -cos(angle*M_PI / 180) * VEL;

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);				//collider for new ship

	radar = { position.x, position.y, RADAR_RADIUS };

	gSpawnedEnemies++;
}

void Enemy::takeDamage() {
	Ship::takeDamage();
}

void Enemy::die() {
	gSpawnedEnemies--;
	gScore++;
	cout << gScore << endl;
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
