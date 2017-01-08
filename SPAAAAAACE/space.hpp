#pragma once

#include<iostream>
#include<string>
#include<algorithm>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"sprite.hpp"
#include"texture.hpp"
#include"timer.hpp"
#include"window.hpp"
#include"level.hpp"
#include"camera.hpp"
#include"collider.hpp"
#include"g.hpp"
#include"font.hpp"
#include"text.hpp"

using namespace std;

bool init();
void close();
bool loadMedia();

void updateLasers();
void updateEnemies(float, Level*, Player*, Camera*);
void updateInfo(Player* player);