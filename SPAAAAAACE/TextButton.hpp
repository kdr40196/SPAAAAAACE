#pragma once
#include"Button.hpp"

class TextButton : public Button {
	shared_ptr<Text> text;
public:
	TextButton();
	TextButton(int x, int y, shared_ptr<Text> text);
	~TextButton();
	void render();
	string getButtonText();
	void setText(string text);
	void setPosition(int x, int y);
	int getWidth();
	int getHeight();
};