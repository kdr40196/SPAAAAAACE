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
		int angle;
		shared_ptr<Texture> texture;
		SDL_Rect clipRect;
		Collider* collider;
	public:
		Sprite();
		Sprite(string path);
		Sprite(Texture texture);
		Sprite(Text text);
		~Sprite();
		void render(Camera* cam = nullptr, Level* l = nullptr);
		void rotate(int x1, int y1);
		void rotate(int x1, int y1, int x2, int y2);
		void rotate(int x1, int y1, Level* l);
		int getX();
		int getY();
		Collider* getCollider();
		int getWidth();
		int getHeight();
};
