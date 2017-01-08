#pragma once

#include<iostream>
#include<SDL.h>
#include"texture.hpp"
using namespace std;

class Text {
	SDL_Point position;
	Texture textTexture;
	SDL_Color textColor;
	public:
		Text();
		Text(string text);
		Text(string text, int x, int y);
		void updatePosition(int x, int y);
		void updateText(string text);
		void render();
		SDL_Color getColor();
		void setColor(SDL_Color);
		int getTextWidth();
		int getTextHeight();
		int getX();
		int getY();
		//bool loadText(string text, SDL_Color color);
};

extern Text scoreText, healthText, scorePrompt, healthPrompt;