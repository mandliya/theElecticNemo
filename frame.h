#ifndef FRAME__H
#define FRAME__H

#include <iostream>
#include <string>
#include <SDL.h>

class Frame {
public:
  Frame( const std::string& fn );
  // Must pass both width and height here because the sprite might
  // not consist of the whole surface, only part of the surface.
  Frame( SDL_Surface* spr, Uint16 sprite_width, Uint16 sprite_height,
         Sint16 src_x, Sint16 src_y);
  Frame(const Frame&);
  Frame& operator=(const Frame&);

  SDL_Surface* getSurface() const { return spriteSurface; }
  void draw(Sint16 x, Sint16 y) const;
  void draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const;

  Uint16 getWidth()  const { return spriteWidth; }
  Uint16 getHeight() const { return spriteHeight; }
  Uint16 getSourceX() const { return spriteSourceX; }
  Uint16 getSourceY() const { return spriteSourceY; }

private:
  SDL_Surface * screen;
  SDL_Surface * spriteSurface;
  Uint16 spriteWidth;
  Uint16 spriteHeight;
  Sint16 spriteSourceX;
  Sint16 spriteSourceY;

  Frame();
};

#endif
