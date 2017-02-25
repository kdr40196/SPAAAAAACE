#pragma once

#include<iostream>
#include<string>
#include<algorithm>
#include<memory>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"sprite.hpp"
#include"ship.hpp"
#include"player.hpp"
#include"enemy.hpp"
#include"laser.hpp"
#include"texture.hpp"
#include"timer.hpp"
#include"window.hpp"
#include"level.hpp"
#include"camera.hpp"
#include"collider.hpp"
#include"g.hpp"
#include"font.hpp"
#include"text.hpp"
#include"menu.hpp"
#include"button.hpp"

using namespace std;

bool init();
void close();
bool loadMainMenu(Menu* mainMenu);
bool loadPauseMenu(Menu* pauseMenu);
bool loadGameOverMenu(Menu* gameOverMenu);
bool loadGame();
void start(Level** level, Player** player, Camera** cam);
void pause(Player* player);
void resume(Player* player);

void updateLasers();
void updateEnemies(float, Level*, Player*, Camera*);
void renderInfo(Player* player);