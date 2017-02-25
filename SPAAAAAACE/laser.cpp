#include"laser.hpp"
#include"player.hpp"
#include"enemy.hpp"
#include"g.hpp"
#include"level.hpp"

vector<Laser> gLasers;

Laser::Laser(int start_x, int start_y, int x, int y, int angle, Level& l, bool playerStarted) {

	this->playerStarted = playerStarted;
	texture = gSpriteSheet;
	clipRect = { 31, 56, LASER_WIDTH, LASER_HEIGHT };

	start = { start_x - LASER_WIDTH / 2, start_y };
	position = { start_x - LASER_WIDTH / 2, start_y };
	click = { x, y };

	width = LASER_WIDTH, height = LASER_HEIGHT;
	this->angle = angle;
	collider = new LaserCollider(position.x, position.y, LASER_WIDTH, LASER_HEIGHT, angle);

	xVel = sin(angle*M_PI / 180) * LASER_VEL;
	yVel = -cos(angle*M_PI / 180) * LASER_VEL;
}

Laser::~Laser() { }

void Laser::move(float timestep, Level& l, Player& player) {

	int xDisplacement = xVel * timestep, yDisplacement = yVel * timestep;

	position.x += xDisplacement;
	position.y += yDisplacement;


	bool dead = true;

	int w = l.getWidth(), h = l.getHeight();

	for (int i = -w; i <= w; i += w) {
		for (int j = -h; j <= h; j += h) {
			if (distance(start, { position.x + i, position.y + j }) < RANGE) {
				dead = false;
				break;
			}
		}
	}

	if (dead) {
		position = { MINUS_INFINITY, MINUS_INFINITY };
		return;
	}
	else {
		if (position.x < 0) {
			position.x = l.getWidth();
			position.y -= yDisplacement;
		}
		else if (position.x > l.getWidth()) {
			position.x = 0;
			position.y -= yDisplacement;
		}
		if (position.y < 0) {
			position.y = l.getHeight();
			position.x -= xDisplacement;
		}
		else if (position.y > l.getHeight()) {
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
		if (collider->collides(player.getCollider())) {
			player.takeDamage();
			position.x = position.y = MINUS_INFINITY;
		}
	}
}
