#pragma once
#include"texture.hpp"
#include"g.hpp"
#include"font.hpp"
#include<string>

Texture::Texture() {
	texture = nullptr;
	width = height = 0;
	//pitch = 0;
	//pixels = nullptr;
}

Texture::~Texture() {
	free();
}

/*void* Texture::getPixels() {
	return pixels;
}

int Texture::getPitch() {
	return pitch;
}*/

void Texture::free() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = height = 0;
		/*pitch = 0;
		pixels = nullptr;*/
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

/*bool Texture::loadTexture(string path) {
	free();
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to load image", gWindow.getReference());
		return false;
	}

	SDL_Surface* tempSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_Surface* formattedSurface = SDL_ConvertSurface(tempSurface, SDL_GetWindowSurface(gWindow.getReference())->format, 0);

	texture = SDL_CreateTexture(gRenderer, SDL_GetWindowPixelFormat(gWindow.getReference()),
		SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);

	if (texture == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create blank texture", gWindow.getReference());
		return false;
	}

	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	lock();

	memcpy(pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

	unlock();

	width = formattedSurface->w;
	height = formattedSurface->h;
	SDL_FreeSurface(surface);
	SDL_FreeSurface(formattedSurface);
	surface = formattedSurface = nullptr;
	return true;
}

bool Texture::loadTexture(string path, Uint32 color) {
	
	if (!loadTexture(path))
		return false;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	lock();

	Uint32* upixels = (Uint32*)pixels;
	int pixelCount = (pitch / 4) * height;
	Uint32 colorKey = SDL_MapRGBA(SDL_GetWindowSurface(gWindow.getReference())->format, 0, 0, 0, 255);

	for (int i = 0; i < pixelCount; i++) {
		if(upixels[i] == colorKey)
			upixels[i] = color;
	}

	unlock();

	return true;
}*/

bool Texture::loadTextureFromText(string text, SDL_Color color) {
	free();
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont.getFont(), text.c_str(), color, 300);
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

/*void Texture::render(int x, int y, Viewport viewport, SDL_Rect * clipRect, double angle, SDL_Point * center) {
	SDL_Rect viewportRect = viewport.getViewPortRect();
	int final_x = x + (gScreenWidth - viewportRect.w);
	int final_y = y + (gScreenHeight - viewportRect.h);
	render(final_x, final_y, clipRect, angle, center);
}*/

int Texture::getWidth() { return width; }

int Texture::getHeight() { return height; }

/*bool Texture::lock() {
	if (pixels != nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Texture already locked", gWindow.getReference());;
		return false;
	}

	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to lock texture", gWindow.getReference());;
		return false;
	}
	return true;
}

bool Texture::unlock() {
	if (pixels == nullptr) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Texture already unlocked", gWindow.getReference());;
		return false;
	}
	SDL_UnlockTexture(texture);
	pixels = nullptr;
	pitch = 0;
	return true;
}*/
