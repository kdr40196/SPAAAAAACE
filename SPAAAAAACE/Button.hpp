#pragma once

#include<iostream>
#include<memory>
#include<SDL.h>
#include "text.hpp"
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

