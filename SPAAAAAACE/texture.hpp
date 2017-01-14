#pragma once
#include<iostream>
#include<memory>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
using namespace std;

class Font;

class Texture {
	SDL_Texture* texture;
	int width, height;
public:
	bool loadTextureFromImage(string path, Uint32 color = 0);
	bool loadTextureFromText(string text, shared_ptr<Font> font, SDL_Color color);

	void render(int x, int y, SDL_Rect* clipRect = nullptr,
		double angle = 0, SDL_Point* center = nullptr);
	int getWidth();
	int getHeight();
	void free();
	Texture();
	Texture(const Texture& obj);
	~Texture();
	Texture operator=(const Texture& obj);
};