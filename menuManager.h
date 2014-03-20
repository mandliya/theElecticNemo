#include <SDL.h>
#include "ioManager.h"
#include "menu.h"
#include "clock.h"
#include "sprite.h"
#include "smartSprite.h"
#include "viewport.h"

class MenuManager {
public:
  MenuManager ();
  void play();
  float getSafeDistance() const { return safeDistance; }

private:
  bool env;
  SDL_Surface *screen;
  Sprite menuBg;

  const Clock& clock;
  
  SDL_Color backColor;
  Menu menu;
  Viewport& viewport;
  float safeDistance;
  void displayHelp();
  void drawBackground() const;
  MenuManager(const MenuManager&);
  MenuManager& operator=(const MenuManager&);
  void setLevel();
};
