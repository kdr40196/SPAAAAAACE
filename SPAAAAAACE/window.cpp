#pragma once
#include<iostream>
#include"g.hpp"
using namespace std;

Window gWindow;
SDL_Renderer* gRenderer = nullptr;
int gScreenWidth = 640, gScreenHeight = 360;

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
	window = SDL_CreateWindow("SPAAAAAACE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
		width = gScreenWidth = e.window.data1;
		height = gScreenHeight = e.window.data2;
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

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

bool Window::isMinimized() {
	return minimized;
}
