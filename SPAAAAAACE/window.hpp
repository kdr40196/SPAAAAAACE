#pragma once
#include<SDL.h>

class Window {
	SDL_Window* window;
	int width, height;
	bool minimized, fullScreen;
	public:
		Window();
		~Window();
		SDL_Window* getReference();
		bool init();
		SDL_Renderer* createRenderer();
		void handleEvent(SDL_Event& e);
		void free();
		void toggleFullScreen();
		int getWidth();
		int getHeight();
		bool isMinimized();
		bool isFullScreen();
};

