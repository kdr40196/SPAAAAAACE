#pragma once

#include "sprite.hpp"

class Ship : public Sprite {
protected:
	int xVel, yVel;
	float health;
	bool damaged;
	ShipType type;
public:
	static const int VEL = 400;
	static const int SHIP_WIDTH = 64, SHIP_HEIGHT = 28;
	Ship();
	Ship(string path);
	void move(float timeStep, Level& level);
	void attack(int x, int y, Level& level);
	void takeDamage();
	int getHealth();
	bool isDamaged();
	ShipType getType();
};

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

class Enemy :public Ship {
	int id, originalAngle;
	Circle attackRadar;
	SDL_Point original;
	static const int MOVEMENT_RANGE = 800, ATTACK_RADAR_RADIUS = 200, ATTACK_TIMEOUT = 1000,
		COOLDOWN_TIME = 3000, DEFAULT_HEALTH = 15;
	bool playerDetected;
	Timer attackTimer, cooldownTimer;
	EnemyState state;
	static int maxHealth;
public:
	static const SDL_Color color;
	Enemy();
	Enemy(Level&, Player&);
	int getId();
	void update(float timeStep, Level& level, Player& player);
	void move(float timeStep, Level& level, Player& player);
	void attack(Player& player, Level& level);
	void rotate(int x, int y, Level& level);
	void spawn(Level&, Camera&);
	void takeDamage();
	void die();
	void respawn(Level&, Camera&);
	void upgrade();
	EnemyState getState();
	void pause();
	void resume();
	static friend void resetEnemy();
};