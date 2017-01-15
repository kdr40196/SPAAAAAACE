#pragma once
#include<iostream>
#include"g.hpp"
using namespace std;

Window gWindow;
SDL_Renderer* gRenderer = nullptr;
int gScreenWidth = 1024, gScreenHeight = 576;

Window::Window() {
	window = nullptr;
	width = height = 0;
	minimized = false;
}

Window::~Window() {
	free();
}

SDL_Window* Window::getReference() {
	return window;
}

bool Window::init() {
	window = SDL_CreateWindow("SPAAAAAACE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		gScreenWidth, gScreenHeight, SDL_WINDOW_SHOWN| SDL_WINDOW_RESIZABLE);
	if (window != nullptr) {
		width = gScreenWidth;
		height = gScreenHeight;
		return true;
	}
	return false;
}

SDL_Renderer* Window::createRenderer() {
	return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event & e) {
	switch (e.window.event) {
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		width = e.window.data1;
		height = e.window.data2;
		cout << width << ", " << height << endl;
		SDL_RenderSetLogicalSize(gRenderer, gScreenWidth, gScreenHeight);
		break;

	case SDL_WINDOWEVENT_MINIMIZED:
		minimized = true;
		break;

	case SDL_WINDOWEVENT_MAXIMIZED:
		minimized = false;

	case SDL_WINDOWEVENT_RESTORED:
		minimized = false;
	}
}

void Window::free() {
	SDL_DestroyWindow(window);
	window = nullptr;
	width = height = 0;
}

void Window::toggleFullScreen() {
	if (fullScreen) {
		fullScreen = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN);
	}
	else {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullScreen = true;
	}
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

bool Window::isMinimized() {
	return minimized;
}

bool Window::isFullScreen() {
	return fullScreen;
}
