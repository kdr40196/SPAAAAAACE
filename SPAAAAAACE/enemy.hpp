#pragma once

#include"ship.hpp"
#include"EnemyStateManager.hpp"

class Enemy :public Ship {
	int id, originalAngle;
	Circle attackRadar;
	SDL_Point original;
	bool playerDetected;
	//EnemyState state;
	static int maxHealth;
	EnemyStateManager* stateManager;
public:
	static const int MOVEMENT_RANGE = 800, ATTACK_RADAR_RADIUS = 200, ATTACK_TIMEOUT = 1000,
		COOLDOWN_TIME = 3000, DEFAULT_HEALTH = 15;
	static const SDL_Color color;
	Timer attackTimer, cooldownTimer;
	Enemy();
	Enemy(Level&, Player&);
	int getId();
	Circle& getAttackRadar();
	float getAngle();
	void setAngle(float angle);
	void update(float timeStep, Level& level, Player& player);
	void move(float timeStep, Level& level, Player& player);
	void attack(Player& player, Level& level);
	void rotate(int x, int y, Level& level);
	void spawn(Level&, Camera&);
	void takeDamage();
	void die();
	void respawn(Level&, Camera&);
	void upgrade();
	void pause();
	void resume();
	static friend void resetEnemy();
};