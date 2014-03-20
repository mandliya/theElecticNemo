#include <cmath>
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}


void SmartSprite::draw() {
  MultiframeSprite::draw();
  std::stringstream strm;
  strm << currentMode;
  io.printMessageAt( strm.str(), 
      static_cast<int>(X())+10, static_cast<int>(Y()) );
}

void SmartSprite::update(Uint32 ticks) {
  MultiframeSprite::update(ticks);
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float ex= enemy.X()+enemy.getFrame()->getWidth()/2;
  float ey= enemy.Y()+enemy.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = ATTACK;
  }
  else if  ( currentMode == ATTACK ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}

