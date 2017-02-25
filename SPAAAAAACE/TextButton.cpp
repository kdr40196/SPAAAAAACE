#include"TextButton.hpp"

TextButton::TextButton() {
	text = nullptr;
}

TextButton::TextButton(int x, int y, shared_ptr<Text> text) : Button(x, y) {
	this->text = text;
}

TextButton::~TextButton() {
	text.reset();
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