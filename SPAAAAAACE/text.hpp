#pragma once

#include<iostream>
#include<SDL.h>
#include"texture.hpp"
#include<memory>
#include<string>
using namespace std;

class Font;

class Text {
	string text;
	SDL_Point position;
	shared_ptr<Texture> textTexture;
	SDL_Color textColor;
	shared_ptr<Font> font;
	public:
		Text();
		Text(string text, shared_ptr<Font> font);
		Text(string text, shared_ptr<Font> font, int x, int y);
		Text(const Text &obj);
		~Text();
		Text operator=(const Text& obj);
		void setPosition(int x, int y);
		int getX();
		int getY();
		bool setText(string text);
		string getText();
		void render();
		void setColor(SDL_Color);
		SDL_Color getColor();
		void setFont(shared_ptr<Font> font);
		shared_ptr<Font> getFont();
		int getTextWidth();
		int getTextHeight();
		shared_ptr<Texture> getTextTexture();
};

extern Text scoreText, healthText, scorePrompt, healthPrompt, gMenuItemPointer;