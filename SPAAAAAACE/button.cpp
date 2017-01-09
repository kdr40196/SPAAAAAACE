#include "Button.hpp"
#include "text.hpp"

Button::Button(){
	position = { 0, 0 };
}

Button::Button(int x, int y) {
	position = { x, y };
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

TextButton::TextButton() {
	text = nullptr;
}

TextButton::TextButton(int x, int y, Text * text): Button(x, y) {
	this->text = text;
}

void TextButton::render() {
	text->render();
}

string TextButton::getButtonText() {
	return text->getText();
}

void TextButton::setText(string text) {
	this->text->setText(text);
}

void TextButton::setPosition(int x, int y) {
	Button::setPosition(x, y);
	text->setPosition(x, y);
}

int TextButton::getWidth() {
	return text->getTextWidth();
}

int TextButton::getHeight() {
	return text->getTextHeight();
}
