#pragma once
#include"timer.hpp"

Timer::Timer() {
	startTicks, pausedTicks = 0;
	started = paused = false;
}

Uint32 Timer::getTicks() {
	if (started) {
		if (paused) return pausedTicks;
		else return SDL_GetTicks() - startTicks;
	}
	return 0;
}

void Timer::start() {
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop() {
	started = false;
	paused = false;
	startTicks = pausedTicks = 0;
}

void Timer::pause() {
	if (started && !paused) {
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause() {
	if (started && paused) {
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

bool Timer::isStarted() {
	return started;
}

bool Timer::isPaused() {
	return paused;
}