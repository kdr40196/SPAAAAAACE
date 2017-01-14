#pragma once
#include"texture.hpp"
#include"g.hpp"
#include"font.hpp"
#include<string>

Texture::Texture() {
	texture = nullptr;
	width = height = 0;
}

Texture::Texture(const Texture & obj) {
	texture = nullptr;
	width = obj.width;
	height = obj.height;
}

Texture::~Texture() {
	free();
}

Texture Texture::operator=(const Texture & obj) {
	Texture tempTexture;
	
	tempTexture.width = obj.width;
	tempTexture.height = obj.height;
	return tempTexture;
}

void Texture::free() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = height = 0;
	}
}

bool Texture::loadTextureFromImage(string path, Uint32 color) {
	free();
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to load image", gWindow.getReference());
		return false;
	}

	texture = SDL_CreateTextureFromSurface(gRenderer, surface);

	if (texture == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create blank texture", gWindow.getReference());
		return false;
	}

	width = surface->w;
	height = surface->h;
	SDL_FreeSurface(surface);
	surface = nullptr;
	return true;
}

bool Texture::loadTextureFromText(string text, shared_ptr<Font> font, SDL_Color color) {
	free();
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font->getFont(), text.c_str(), color, 300);
	if (textSurface == nullptr) {
		cout << "Unable to create text surface: " << SDL_GetError() << endl;
		return false;
	}
	texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (texture == nullptr) {
		cout << "Unable to create text texture: " << text << endl;
		return false;
	}
	width = textSurface->w, height = textSurface->h;
	SDL_FreeSurface(textSurface);
	return true;
}

void Texture::render(int x, int y, SDL_Rect* clipRect, double angle,
	SDL_Point* center) {
	
	SDL_Rect renderRect;
	renderRect.x = x;
	renderRect.y = y;
	if (clipRect == nullptr) {
		renderRect.h = height;
		renderRect.w = width;
	}
	else {
		renderRect.h = clipRect->h;
		renderRect.w = clipRect->w;
	}
	SDL_RenderCopyEx(gRenderer, texture, clipRect, &renderRect, angle, center, SDL_FLIP_NONE);
}

int Texture::getWidth() { return width; }

int Texture::getHeight() { return height; }
