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
		Font(const Font& obj) {
			this->font = obj.font;
			this->size = obj.size;
		}
		Font operator=(const Font& obj) {
			Font temp;
			temp.font = obj.font;
			temp.size = obj.size;
			return temp;
		}
		~Font();
		void free();
		void setFont(string fontName, int size);
		TTF_Font* getFont();
};