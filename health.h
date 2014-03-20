#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Health {
public:
  Health(SDL_Surface*);
  Health(SDL_Surface* screen, int increments);
  Health(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c, float sp, SDL_Surface*);
  void draw() const;
  void update(Uint32, bool, int);
  void reset() { currentLength = totalLength; }
  int getCurrentHealth(){ return currentLength;}

private:
  SDL_Surface* screen;
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 BLUE;
  const Uint32 color;
  void drawBox() const;
  Health(const Health&);
  Health& operator=(const Health&);
};
