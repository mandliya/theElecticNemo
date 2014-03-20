#include "drawable.h"
#include "frame.h"
#include "ioManager.h"
#include "viewport.h"

Frame::Frame( const Frame& frame ) :
  screen(frame.screen),
  spriteSurface(frame.spriteSurface), 
  spriteWidth(frame.spriteWidth), spriteHeight(frame.spriteHeight),
  spriteSourceX(frame.spriteSourceX), spriteSourceY(frame.spriteSourceY) 
  { }

Frame::Frame( SDL_Surface* spr, Uint16 sprite_width, Uint16 sprite_height,
              Sint16 src_x, Sint16 src_y) :
  screen(IOManager::getInstance().getScreen()),
  spriteSurface(spr), 
  spriteWidth(sprite_width), spriteHeight(sprite_height),
  spriteSourceX(src_x), spriteSourceY(src_y) {
}

Frame& Frame::operator=(const Frame& rhs) {
  spriteSurface = rhs.spriteSurface;
  screen = rhs.screen;
  spriteWidth = rhs.spriteWidth;
  spriteHeight = rhs.spriteHeight;
  spriteSourceX = rhs.spriteSourceX;
  spriteSourceY = rhs.spriteSourceY;
  return *this;
}

void Frame::draw(Sint16 x, Sint16 y) const {
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect src = { spriteSourceX, spriteSourceY, 
                   spriteWidth, spriteHeight 
                 };    
  SDL_Rect dest = {x, y, spriteWidth, spriteHeight };
  SDL_BlitSurface(spriteSurface, &src, screen, &dest);
}

void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, spriteWidth, spriteHeight };    
  SDL_Rect dest = {dx, dy, spriteWidth, spriteHeight };
  SDL_BlitSurface(spriteSurface, &src, screen, &dest);
}


