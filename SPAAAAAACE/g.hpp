#pragma once

#include<vector>
#include"window.hpp"
#include"sprite.hpp"

//class Viewport;
class Font;
class Texture;
class Enemy;

const int FPS = 60;
const float TICKS_PER_FRAME = 1000.f / FPS;
const int TOTAL_ENEMIES = 25;

extern int gSpawnedEnemies;							//total spawned enemies - ++ on spawing, -- on dying
extern int gScreenWidth, gScreenHeight;/* gPlayAreaWidth, gPlayAreaHeight;*/
extern Window gWindow;
/*extern Viewport gPlayViewport, gInfoViewport;*/
extern SDL_Renderer* gRenderer;
extern Texture gSpriteSheet;						//texture storing ship images and maybe projectile image
extern Enemy* gEnemies[TOTAL_ENEMIES];				//references to all enemy ships
extern vector<Laser> gLasers;
extern Font gFont;

extern int gScore;