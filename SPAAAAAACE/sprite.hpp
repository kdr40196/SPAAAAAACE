#pragma once
#include<iostream>
#include"collider.hpp"
#include"timer.hpp"
using namespace std;

class Camera;
class Texture;
class Level;


bool checkCollision(SDL_Rect* a, SDL_Rect* b);

enum class ShipType { SHIP_TYPE_PLAYER, SHIP_TYPE_ENEMY };


class Sprite {
	protected:
		int width, height;
		SDL_Point position;
		int angle;
		Texture* texture;
		SDL_Rect clipRect;
		Collider collider;
	public:
		Sprite();
		Sprite(string path);
		void render(Camera* cam = nullptr);
		void rotate(int x1, int y1);
		void rotate(int x1, int y1, Level*);
		int getX(bool screenPos = false);
		int getY(bool screePos = false);
		Collider* getCollider();
		/*int getWidth();
		int getHeight();*/
};

class Ship : public Sprite {
	protected:
		int xVel, yVel;
		int health;
		bool damaged;
		ShipType type;
	public:
		static const int VEL = 400;
		static const int SHIP_WIDTH = 64, SHIP_HEIGHT = 28;
		Ship();
		Ship(string path);
		void move(float timeStep, Level* l);
		void attack(int x, int y, Level* l);
		void takeDamage();
		int getHealth();
		bool isDamaged();
		ShipType getType();
};

class Player : public Ship {
	Camera* cam;
	public:
		static const SDL_Color color;
		static int maxHealth;
		Player();
		Player(Level*);
		void handleInput(SDL_Event& e, Level* l);
		void move(float timeStep, Level* l);
		//int getHealth();
};

class Enemy :public Ship {
	SDL_Point original;
	static const int MOVEMENT_RANGE = 800, CHASE_RADIUS = 1000, ATTACK_TIMEOUT = 500;
	bool playerDetected;
	Timer attackTimer;
	public:
		static const SDL_Color color;
		static int maxHealth;
		Enemy();
		Enemy(Level*, Player*);
		void update(float timeStep, Level* l, Player* player, Camera* cam);
		void move(float timeStep, Level* l, Player* player);
		void attack(Player* player, Level* l);
		void chase(Player* player, Level* l);
		void spawn(Level*, Camera*);
		void die();
		void respawn(Level*, Camera*);
		void upgrade();
};

class Laser : public Sprite {
	SDL_Point start, click;
	int xVel, yVel;
	bool playerStarted;
	public:
		static const int LASER_VEL = 1000, RANGE = 1000, LASER_WIDTH = 2, LASER_HEIGHT = 28;
		Laser(int start_x, int start_y, int x, int y, Level* l, bool playerStarted);
		~Laser();
		void move(float timestep, Level* l, Player* player);
};
