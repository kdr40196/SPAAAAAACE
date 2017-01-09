#include "text.hpp"
#include "g.hpp"

Text scoreText, healthText, scorePrompt, healthPrompt, gMenuItemPointer;

Text::Text() {
	position = { 0, 0 };
	textColor = { 255, 255, 255 };
	text = "";
	font = &gSmallFont;
	textTexture = make_shared<Texture>();
}

Text::Text(string text, Font* font) {
	position = { 0, 0 };
	this->text = text;
	textColor = { 255, 255, 255 };
	textTexture = make_shared<Texture>();
	this->font = font;
	textTexture->loadTextureFromText(text, font, textColor);
}

Text::Text(string text, Font* font, int x, int y) {
	position = { x, y };
	this->text = text;
	textColor = { 255, 255, 255 };
	textTexture = make_shared<Texture>();
	this->font = font;
	textTexture->loadTextureFromText(text, font, textColor);
}

Text::Text(const Text & obj) {
	text = obj.text;
	position = obj.position;
	textTexture = obj.textTexture;
	this->font = font;
	textColor = obj.textColor;
}

Text Text::operator=(const Text & obj) {
	Text tempText;
	tempText.text = obj.text;
	tempText.position = obj.position;
	tempText.textTexture = obj.textTexture;
	tempText.font = font;
	tempText.textColor = obj.textColor;
	return tempText;
}

void Text::setPosition(int x, int y) {
	position = { x, y };
}

int Text::getX() {
	return position.x;
}

int Text::getY() {
	return position.y;
}


bool Text::setText(string text) {
	textTexture->loadTextureFromText(text, font, textColor);
	this->text = text;
	return true;
}

string Text::getText() {
	return text;
}

void Text::render() {
	textTexture->render(position.x, position.y);
}

void Text::setColor(SDL_Color color) {
	textColor = color;
}

SDL_Color Text::getColor() {
	return textColor;
}

void Text::setFont(Font * font) {
	this->font = font;
	setText(text);
}

Font* Text::getFont() {
	return font;
}

int Text::getTextWidth() {
	return textTexture->getWidth();
}

int Text::getTextHeight() {
	return textTexture->getHeight();
}

shared_ptr<Texture> Text::getTextTexture() {
	return textTexture;
}
