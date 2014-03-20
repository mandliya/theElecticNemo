#include "health.h"

Health::Health(SDL_Surface* screen) :
  screen(screen),
  start(Vector2f(130, 10)), 
  totalLength(200), 
  currentLength(200), 
  thick(14), 
  increments(20),
  interval(600),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  BLUE( SDL_MapRGB(screen->format, 0x00, 0x00, 0xff) ),
  color(BLUE) {
}
Health::Health(SDL_Surface* screen, int increments) :
  screen(screen),
  start(Vector2f(550, 10)), 
  totalLength(200), 
  currentLength(200), 
  thick(14), 
  increments(increments),
  interval(100),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  BLUE( SDL_MapRGB(screen->format, 0x00, 0x00, 0xff) ),
  color(RED) {
}





Health::Health(int sx, int sy, int tl, int cl,
               float t, int inc, Uint32 c, float sp, SDL_Surface* screen):
  screen(screen),
  start(Vector2f(sx, sy)), 
  totalLength(tl), 
  currentLength(cl), 
  thick(t), 
  increments(inc),
  interval(sp),
  deltaTime(SDL_GetTicks()),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  BLUE( SDL_MapRGB(screen->format, 0x00, 0x00, 0xff) ),
    color(c) {
}

void Health::drawBox() const {
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+totalLength, start[1], 
                      thick, GRAY);
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8, 
                      start[0]+totalLength, start[1]-8, 
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+8, 
                      start[0]+totalLength, start[1]+8, 
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8, 
                      start[0]-1, start[1]+8, 
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8, 
                      start[0]+totalLength+1, start[1]+8, 
                      2.0, BLACK);
}

void Health::draw() const {
  
  drawBox();
  if(currentLength>=0){
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+currentLength, start[1], 
                      thick, color);
  }
}
void Health::update(Uint32 ticks, bool shark, int add) {
  increments = increments + add;
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
	if(!shark){		
	    currentLength -= increments;
	    }
	else
	    currentLength = currentLength - 2* increments;
  }
}


