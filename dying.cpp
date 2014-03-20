#include <iostream>
#include <cmath>
#include "dying.h"
#include "gamedata.h"
#include <limits>
using std::cout; using std::endl;

void Dying::advanceFrame(Uint32 ticks) {
	// Frame animation specialized for a dying crow (rip):
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > dieInterval) {
		if (currentFrame < numberOfFrames-1) {
		  ++currentFrame;
		}
		timeSinceLastFrame = 0;
	}
}

Dying::
Dying(const std::string& name) :
  MultiframeSprite(name,0),
  dyingname(name),
  dead(false),
  timeSinceLastFrame( 0 ),
  dieInterval(Gamedata::getInstance()->getXmlInt(dyingname+"FrameInterval"))
{ }

Dying::Dying(const Dying& s) :
  MultiframeSprite(s),
  dyingname(s.dyingname),
  dead(s.dead),
  timeSinceLastFrame( 0 ),
  dieInterval(s.dieInterval)
{ }

Dying& Dying::operator=(const Dying& rhs) {
  dead = rhs.dead;
  std::cout << "FIX THIS" << std::endl;
  return *this;
}

void Dying::update(Uint32 ticks) { 
  if ( Y() + frameHeight  < worldHeight ) {
    MultiframeSprite::update(ticks);
    advanceFrame(ticks);
  }
  else {
      dead = true;
  }
}

