#pragma once
#include<SDL.h>

class Window {
	SDL_Window* window;
	int width, height;
	bool minimized;
	public:
		Window();
		~Window();
		SDL_Window* getReference();
		bool init();
		SDL_Renderer* createRenderer();
		void handleEvent(SDL_Event& e);
		void free();
		int getWidth();
		int getHeight();
		bool isMinimized();
};

