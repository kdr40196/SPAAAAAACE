#pragma once
#include<iostream>
#include"collider.hpp"
using namespace std;

class Camera;
class Texture;
class Level;
//class Collider;


bool checkCollision(SDL_Rect* a, SDL_Rect* b);



class Sprite {
	protected:
		int width, height;
		SDL_Point position;
		double angle;
		Texture* texture;
		SDL_Rect clipRect;
		Collider collider;
	public:
		Sprite();
		Sprite(string path);
		void render(Camera* cam = nullptr);
		void rotate(int x1, int y1);
		void rotate(int x1, int y1, Level*);
		int getX();
		int getY();
		Collider* getCollider();
		/*int getWidth();
		int getHeight();*/
};



class Laser : public Sprite {
	SDL_Point start, click;
	int xVel, yVel;
public:
	static const int LASER_VEL = 1000, RANGE = 1000, LASER_WIDTH = 2, LASER_HEIGHT = 28;
	Laser(int start_x, int start_y, int x, int y, Level* l);
	~Laser();
	void move(float timestep, Level* l);
};



class Ship : public Sprite {
	protected:
		int xVel, yVel;
		int health;
		bool damaged;
	public:
		static const int VEL = 600;
		static const int SHIP_WIDTH = 64, SHIP_HEIGHT = 28;
		Ship();
		Ship(string path);
		void move(float timeStep, Level* l);
		void attack(int x, int y, Level* l);
		void takeDamage();
		int getHealth();
		bool isDamaged();
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
	//range in which the enemy roams
	//range in which the enemy chases the player
	static const int range = 200, chaseRadius = 500;
	public:
		static const SDL_Color color;
		static int maxHealth;
		Enemy();
		Enemy(Level*, Player*);
		void move(float timeStep, Level* l, Player* player);
		void spawn(Level*, Camera*);
		void die();
		void respawn(Level*, Camera*);
		void upgrade();
};
