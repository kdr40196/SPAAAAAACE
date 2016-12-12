#pragma once
#include<SDL.h>

class Timer {
	Uint32 startTicks, pausedTicks;
	bool started, paused;
public:
	Timer();
	Uint32 getTicks();
	void start();
	void stop();
	void pause();
	void unpause();
	bool isStarted();
	bool isPaused();
};