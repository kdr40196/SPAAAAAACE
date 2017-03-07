#pragma once

#include"ship.hpp"

enum class EnemyState { IDLE, ATTACKING, RETURNING_TO_IDLE, COOLDOWN };

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