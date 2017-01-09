#pragma once

#include<iostream>
#include<SDL.h>
using namespace std;

class Text;

class Button {
	SDL_Point position;
	public:
		Button();
		Button(int x, int y);
		void setPosition(int x, int y);
		int getX();
		int getY();
		virtual void render() = 0;
};

class TextButton: public Button {
	Text* text;
	public:
		TextButton();
		TextButton(int x, int y, Text* text);
		void render();
		string getButtonText();
		void setText(string text);
		void setPosition(int x, int y);
		int getWidth();
		int getHeight();
};