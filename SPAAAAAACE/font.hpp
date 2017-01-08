#pragma once

#include<iostream>
#include<SDL_ttf.h>
#include"texture.hpp"

using namespace std;

class Font {
	TTF_Font* font;
	int size;
	public:
		Font();
		Font(string font, int size);
		~Font();
		void free();
		void setFont(string fontName, int size);
		TTF_Font* getFont();
};