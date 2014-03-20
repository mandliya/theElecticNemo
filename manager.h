#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "sprite.h"
#include "world.h"
#include "viewport.h"
#include "multisprite.h"
#include "scaledTwowaySprite.h"
#include "lsystemSprite.h"
#include "smartSprite.h"
#include "player.h"
#include "explodingSprite.h"
#include "health.h"


class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void setDifficultyLevel();
  void setDLevel(int x){level = x;}
  float getSafeDistance() const { return safeDistance; }
  

private:
  const bool env;
  const Gamedata* gdata;
  const IOManager& io;
  Clock& clock;
  Player player;
  

  SDL_Surface *screen;
  SDL_Surface *SmallFishSurface;

  World worldBack;
  World worldFront;

  Viewport& viewport;
  
  
  int currObject;

  std::list<Drawable*> objects;
  std::list<Drawable*> deadObjects;
  std::list<ScaledTwowaySprite*> scaledFish;
  std::list<ExplodingSprite*> explodingObjects;
  int collisionCount;

  const unsigned TICK_INTERVAL;
  unsigned nextTime;
  Health bar;
  Health Sharkbar;
  int collcount;
  int level;
  bool gameOver; 
  bool youWon;
  float safeDistance;
  int leveladd;
  

  void displayHud();
  int timeLeft();

  void draw() const;
  void update(Uint32);
  void makeSmallFish();
  Manager(const Manager&);
  Manager& operator=(const Manager&);

  
};
