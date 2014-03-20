#include <iostream>
#include <cmath>
#include "dyingtwo.h"
#include "gamedata.h"
#include <limits>
using std::cout; using std::endl;

void DyingTwo::advanceFrame(Uint32 ticks) {
	// Frame animation specialized for a dying crow (rip):
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > dieInterval) {
		if (currentFrame < numberOfFrames-1) {
		  ++currentFrame;
		}
		timeSinceLastFrame = 0;
	}
}

DyingTwo::
DyingTwo(const std::string& name) :
  TwowayMultiframeSprite(name,0),
  dyingname(name),
  dead(false),
  timeSinceLastFrame( 0 ),
  dieInterval(Gamedata::getInstance()->getXmlInt(dyingname+"FrameInterval"))
{ }

DyingTwo::DyingTwo(const DyingTwo& s) :
  TwowayMultiframeSprite(s),
  dyingname(s.dyingname),
  dead(s.dead),
  timeSinceLastFrame( 0 ),
  dieInterval(s.dieInterval)
{ }

DyingTwo& DyingTwo::operator=(const DyingTwo& rhs) {
  dead = rhs.dead;
  std::cout << "FIX THIS" << std::endl;
  return *this;
}

void DyingTwo::update(Uint32 ticks) { 
  if ( Y() + frameHeight  < worldHeight ) {
    TwowayMultiframeSprite::update(ticks);
    advanceFrame(ticks);
  }
  else {
      dead = true;
  }
}

