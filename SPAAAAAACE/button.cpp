#include "Button.hpp"

Button::Button(){
	position = { 0, 0 };
}

Button::Button(int x, int y) {
	position = { x, y };
}

Button::~Button() {
	position = { 0, 0 };
}

void Button::setPosition(int x, int y) {
	position = { x, y };
}

int Button::getX() {
	return position.x;
}

int Button::getY() {
	return position.y;
}
