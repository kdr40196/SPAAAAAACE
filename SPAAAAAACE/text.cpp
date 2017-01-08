#include "text.hpp"

Text scoreText, healthText, scorePrompt, healthPrompt;

Text::Text() {
	position = { 0, 0 };
	textColor = { 255, 255, 255 };
}

Text::Text(string text) {
	textTexture.loadTextureFromText(text, textColor);
	position = { 0, 0 };
	textColor = { 255, 255, 255 };
}

Text::Text(string text, int x, int y) {
	textTexture.loadTextureFromText(text, textColor);
	position = { x, y };
	textColor = { 255, 255, 255 };
}

void Text::updatePosition(int x, int y) {
	position = { x, y };
}

void Text::updateText(string text) {
	textTexture.loadTextureFromText(text, textColor);
}

void Text::render() {
	textTexture.render(position.x, position.y);
}

SDL_Color Text::getColor() {
	return textColor;
}

void Text::setColor(SDL_Color color) {
	textColor = color;
}

int Text::getTextWidth() {
	return textTexture.getWidth();
}

int Text::getTextHeight() {
	return textTexture.getHeight();
}

int Text::getX() {
	return position.x;
}

int Text::getY() {
	return position.y;
}

/*bool Text::loadText(string text, SDL_Color color) {
	textTexture.loadTextureFromText(text, color);
	return false;
}*/
