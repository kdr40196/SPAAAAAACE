#pragma once
#include"sprite.hpp"
#include"camera.hpp"
#include"g.hpp"
#include<cmath>
//#include<vector>


Texture gSpriteSheet;
Enemy* gEnemies[TOTAL_ENEMIES];
vector<Laser> gLasers;
int spawnedEnemies = 0, gScore = 0;

const SDL_Color Player::color = {0, 0, 255, 255};
const SDL_Color Enemy::color = { 255, 0, 0, 255 };
int Player::maxHealth = 50;
int Enemy::maxHealth = 15;

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
	return false;
}

int distance(SDL_Point a, SDL_Point b) {
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
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

	//cout << angle << endl;
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

int Sprite::getX() {
	return position.x;
}

int Sprite::getY() {
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




Laser::Laser(int start_x, int start_y, int x, int y, Level* l) {

	texture = &gSpriteSheet;
	clipRect = { 31, 56, LASER_WIDTH, LASER_HEIGHT };

	start = { start_x, start_y };
	position = {start_x, start_y};
	click = { x, y };

	width = LASER_WIDTH, height = LASER_HEIGHT;

	//rotate sprite
	rotate(x, y, l);

	xVel = sin(angle*M_PI/180) * LASER_VEL;
	yVel = cos(angle*M_PI/180) * -LASER_VEL;

	collider.init(start_x, start_y, LASER_WIDTH, LASER_HEIGHT);
}

Laser::~Laser() { }

void Laser::move(float timestep, Level* l) {

	position.x += xVel * timestep;
	position.y += yVel * timestep;

	collider.move(position);

	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
			gEnemies[i]->takeDamage();
			position.x = position.y = -999;
			break;
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
			break;
		}
	}
	collider.move(position);
	
	displacement = yVel * timeStep;
	position.y += displacement;
	
	collider.move(position);
	
	if (position.y < 0 || position.y + SHIP_HEIGHT > l->getHeight()) position.y -= displacement;
	
	//check if colliding with enemy
	for (int i = 0; i < TOTAL_ENEMIES; i++) {
		if (checkCollision(collider.getColliderRect(), gEnemies[i]->getCollider()->getColliderRect())) {
			position.y -= displacement;
			break;
		}
	}
	collider.move(position);
}

void Ship::attack(int x, int y, Level* l) {
	//projectile follows straight line from current tip position 
	//to x, y and beyond till it hits an enemy or goes out of some range or out of window
	//eg: range: 1000 - projectile will be destroyed after travelling 1000 pixels

	//create new projectile object - add into global projectile vector
	//start from center of the ship
	//follow straight line from start point to the point clicked

	Laser tmp(position.x + SHIP_WIDTH / 2, position.y, x, y, l);
	
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



Player::Player() {
	position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = (gScreenHeight - SHIP_HEIGHT) / 2;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };
	
	texture = &gSpriteSheet;

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);

	health = maxHealth;
}

Player::Player(Level* l) {
	/*position.x = (gScreenWidth - SHIP_WIDTH) / 2;
	position.y = l->getHeight() - ((gScreenHeight + SHIP_HEIGHT) / 2);*/
	position.x = gScreenWidth / 2;
	position.y = gScreenHeight / 2;
	clipRect = { 0, 28, SHIP_WIDTH, SHIP_HEIGHT };
	texture = &gSpriteSheet;

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

void Player::move(float timeStep, Level *l) {
	Ship::move(timeStep, l);

	int x, y;
	SDL_GetMouseState(&x, &y);
	rotate(x, y, l);
}



Enemy::Enemy() {
	texture = &gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };
	position.x = rand() % (gScreenWidth - Ship::SHIP_WIDTH);
	position.y = rand() % (gScreenHeight - Ship::SHIP_HEIGHT);

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);
}

Enemy::Enemy(Level* l, Player* p) {
	texture = &gSpriteSheet;
	clipRect = { 0, 0, SHIP_WIDTH, SHIP_HEIGHT };

	//spawn(l, p);

	//angle = rand() % 360;

	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);
}

void Enemy::spawn(Level* l, Camera* cam) {
	health = maxHealth;
	
	SDL_Rect shipColliderRect;					//temp collider for new ship
	bool success;
	do {
		success = true;
		position.x = rand() % (l->getWidth() - SHIP_WIDTH);
		position.y = rand() % (l->getHeight() - SHIP_HEIGHT);

		shipColliderRect = { position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT };
		
		//check if enemy is spawned in player area
		if (!checkCollision(&shipColliderRect, cam->getRect())) {
			
			//check if colliding with previously spawned enemies
			for (int i = 0; i < spawnedEnemies; i++) {
				if (checkCollision(&shipColliderRect, gEnemies[i]->getCollider()->getColliderRect())) {
					success = false;
					break;
				}
			}
		}
		else success = false;
	} while (!success);
	spawnedEnemies++;
	collider.init(position.x, position.y, SHIP_WIDTH, SHIP_HEIGHT);				//collider for new ship
	
	//health = maxHealth;
}

void Enemy::die() {
	spawnedEnemies--;
	gScore++;
	cout << gScore << endl;
	if (maxHealth < 50) {
		maxHealth++;
		cout << "Enemy Max Health: " << maxHealth << endl;
	}
}

void Enemy::respawn(Level* l, Camera* cam) {
	spawn(l, cam);
}

void Enemy::upgrade() {
	health = maxHealth;
}
