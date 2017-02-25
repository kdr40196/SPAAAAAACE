#pragma once

#include<vector>
#include<memory>
#include"window.hpp"
#include"sprite.hpp"

#define MINUS_INFINITY -9999

class Font;
class Texture;
class Enemy;
class Laser;

const int FPS = 60;
const float TICKS_PER_FRAME = 1000.f / FPS;
const int TOTAL_ENEMIES = 25;

extern int gSpawnedEnemies;							//total spawned enemies - ++ on spawing, -- on dying
extern int gScreenWidth, gScreenHeight;
extern Window gWindow;
extern SDL_Renderer* gRenderer;
extern shared_ptr<Texture> gSpriteSheet;						//texture storing ship images and maybe projectile image
extern Enemy** gEnemies;							//references to all enemy ships
extern vector<Laser> gLasers;
extern shared_ptr<Font> gSmallFont, gLargeFont;

extern int gScore;