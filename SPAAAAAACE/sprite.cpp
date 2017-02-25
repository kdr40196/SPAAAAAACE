#pragma once
#include"sprite.hpp"
#include"camera.hpp"
#include"g.hpp"
#include"text.hpp"
#include<cmath>

shared_ptr<Texture> gSpriteSheet = make_shared<Texture>();

Sprite::Sprite() {
	angle = 0;
	texture = make_shared<Texture>(Texture());
	width = 0;
	height = 0;
	collider = nullptr;
}

Sprite::Sprite(string path) {
	angle = 0;
	texture = make_shared<Texture>(Texture());
	if (!texture->loadTextureFromImage(path)) exit(-1);
	width = texture->getWidth();
	height = texture->getHeight();
	collider = nullptr;
}

Sprite::Sprite(Texture texture) {
	angle = 0;
	this->texture = make_shared<Texture>(texture);
	width = texture.getWidth();
	height = texture.getHeight();
	collider = nullptr;
}

Sprite::Sprite(Text text) {
	angle = 0;
	texture = text.getTextTexture();
	width = texture->getWidth();
	height = texture->getHeight();
	collider = nullptr;
}

Sprite::~Sprite() {
	width = height = angle = 0;
	texture.reset();
	collider->getColliders();
	collider = nullptr;
}


void Sprite::render(Camera* cam, Level* l) {
	if (cam != nullptr) {
		int x = position.x - cam->getX(), y = position.y - cam->getY();
		if (x + width> l->getWidth()) {
			x -= l->getWidth();
		}
		else if (x + width < 0) {
			x += l->getWidth();
		}
		if (y + height> l->getHeight()) {
			y -= l->getHeight();
		}
		else if (y + height < 0) {
			y += l->getWidth();
		}

		texture->render(x, y, &clipRect, angle);
	}
	else texture->render(position.x, position.y, &clipRect, angle);
}

void Sprite::rotate(int x1, int y1) {
	int x2 = position.x + width / 2;
	int y2 = position.y + height / 2;
	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
	collider->rotate(angle);
}

void Sprite::rotate(int x1, int y1, Level* l) {
	int x2 = position.x + width / 2;
	int y2 = position.y + height / 2;

	if (abs(x1 - x2) > abs(x1 - (x2 - l->getWidth())))
		x2 = x2 - l->getWidth();
	if (abs(x1 - x2) > abs(x1 - (x2 + l->getWidth())))
		x2 = x2 + l->getWidth();

	if (abs(y1 - y2) > abs(y1 - (y2 - l->getHeight())))
		y2 = y2 - l->getHeight();
	if (abs(y1 - y2) > abs(y1 - (y2 + l->getHeight())))
		y2 = y2 + l->getHeight();

	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
	collider->rotate(angle);
}

void Sprite::rotate(int x1, int y1, int x2, int y2) {
	angle = atan2(x1 - x2, y2 - y1) * 180 / M_PI;
}

int Sprite::getX() {
	return position.x;
}	

int Sprite::getY() {
	return position.y;
}

Collider* Sprite::getCollider() {
	return collider;
}

int Sprite::getWidth() {
	return width;
}

int Sprite::getHeight() {
	return height;
}
