#pragma once

#include<iostream>
#include<memory>
#include<SDL.h>
using namespace std;

class Text;

class Button {
	protected:
		SDL_Point position;
	public:
		Button();
		Button(int x, int y);
		~Button();
		void setPosition(int x, int y);
		int getX();
		int getY();
		virtual void render() = 0;
};

class TextButton: public Button {
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