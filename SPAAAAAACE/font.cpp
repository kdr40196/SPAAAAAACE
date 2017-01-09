#include "font.hpp"
#include "g.hpp"

Font gSmallFont, gLargeFont;

Font::Font() {
	font = nullptr;
	size = 0;
}

Font::Font(string fontName, int size) {
	font = TTF_OpenFont(fontName.c_str(), size);
	this->size = size;
}

void Font::free() {
	font = nullptr;
	size = 0;
}

void Font::setFont(string fontName, int size) {
	free();
	font = TTF_OpenFont(fontName.c_str(), size);
	this->size = size;
}

TTF_Font* Font::getFont() {
	return font;
}

Font::~Font() {
	free();
}
