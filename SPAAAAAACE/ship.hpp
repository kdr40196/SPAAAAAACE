#pragma once

#include "sprite.hpp"
#include"g.hpp"
#include"laser.hpp"
#include"texture.hpp"
#include"camera.hpp"
#include<cmath>

enum class ShipType { SHIP_TYPE_PLAYER, SHIP_TYPE_ENEMY };

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
