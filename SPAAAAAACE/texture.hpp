#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
using namespace std;

class Texture {
	SDL_Texture* texture;
	int width, height;
	
	//not working for transparent pixels
	/*int pitch;
	void* pixels;*/

public:
	//not working for transparent pixels
	//bool loadTexture(string path);
	bool loadTextureFromImage(string path, Uint32 color = 0);
	bool loadTextureFromText(string text, SDL_Color color);

	void render(int x, int y, SDL_Rect* clipRect = nullptr,
		double angle = 0, SDL_Point* center = nullptr);
	/*void render(int x, int y, Viewport viewport, SDL_Rect* clipRect = nullptr,
		double angle = 0, SDL_Point* center = nullptr);*/
	int getWidth();
	int getHeight();
	/*bool lock();
	bool unlock();
	void* getPixels();
	int getPitch();*/
	void free();
	Texture();
	~Texture();
};