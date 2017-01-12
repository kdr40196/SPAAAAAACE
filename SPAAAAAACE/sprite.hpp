#pragma once
#include<iostream>
#include<memory>
#include"collider.hpp"
#include"timer.hpp"
using namespace std;

class Camera;
class Texture;
class Level;
class Text;

enum class ShipType { SHIP_TYPE_PLAYER, SHIP_TYPE_ENEMY };

enum class EnemyState { IDLE, ATTACKING, COOLDOWN, RETURNING_TO_IDLE };


class Sprite {
	protected:
		int width, height;
		SDL_Point position;
		float angle;
		//Texture* texture;
		shared_ptr<Texture> texture;
		SDL_Rect clipRect;
		Collider* collider;
	public:
		Sprite();
		Sprite(string path);
		Sprite(Texture texture);
		Sprite(Text text);
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
		float health;
		bool damaged;
		ShipType type;
	public:
		static const int VEL = 400;
		static const int SHIP_WIDTH = 64, SHIP_HEIGHT = 28;
		Ship();
		Ship(string path);
		void move(float timeStep, Level* level);
		void attack(int x, int y, Level* level);
		void takeDamage();
		int getHealth();
		bool isDamaged();
		ShipType getType();
};

class Player : public Ship {
	Camera* cam;
	Timer hitTimer, regenerateTimer;
	public:
		static const SDL_Color color;
		static int maxHealth;
		
		//regenerate after 3 secs without a hit
		static const int REGENERATE_RATE = 1, REGENERATE_TIMEOUT = 500, REGENERATE_AFTER = 3000;
		
		Player();
		Player(Level*);
		void handleInput(SDL_Event& e, Level* level);
		bool update(float timeStep, Level* level);
		void move(float timeStep, Level* level);
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
		Enemy(Level*, Player*);
		int getId();
		void update(float timeStep, Level* level, Player* player);
		void move(float timeStep, Level* level, Player* player);
		void attack(Player* player, Level* level);
		void spawn(Level*, Camera*);
		void takeDamage();
		void die();
		void respawn(Level*, Camera*);
		void upgrade();
		EnemyState getState();
		void pause();
		void resume();
		static friend void resetEnemy();
};

class Laser : public Sprite {
	SDL_Point start, click;
	int xVel, yVel;
	bool playerStarted;
	public:
		static const int LASER_VEL = 1000, RANGE = 1000, LASER_WIDTH = 2, LASER_HEIGHT = 28;
		Laser(int start_x, int start_y, int x, int y, int angle, Level* l, bool playerStarted);
		~Laser();
		void move(float timestep, Level* l, Player* player);
};
