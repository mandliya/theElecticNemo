#include <iostream>
#include <cmath>
#include "ioManager.h"
#include "bullets.h"
#include "SDL/SDL_rotozoom.h"

Bullets::~Bullets() {
  SDL_FreeSurface( bulletSurface );
  SDL_FreeSurface( ibulletSurface );
  delete strategy;
  for ( unsigned int i = 0; i < bulletFrameLeft.size(); ++i ) {
    delete bulletFrameLeft[i]; // ExplodingSprite made them, so it deletes them
  }
  
   for ( unsigned int i = 0; i < bulletFrameLeft.size(); ++i ) {
    delete bulletFrameRight[i]; // ExplodingSprite made them, so it deletes them
  }
}

Bullets::Bullets(const std::string& n) :
  name(n),
  strategy( new PerPixelCollisionStrategy ),
  bulletSurface(IOManager::getInstance().loadAndSet(
    Gamedata::getInstance()->getXmlStr(name+"File"), true)
  ),
  ibulletSurface(),
  bulletFrameLeft(),
  bulletFrameRight(), 
  frameInterval(Gamedata::getInstance()->getXmlInt(name+"BulletInterval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{
    ibulletSurface = rotozoomSurface(bulletSurface, 180, 1, SMOOTHING_ON);
    unsigned num = Gamedata::getInstance()->getXmlInt(name+"Frames"); 
    Uint16 pwidth = Gamedata::getInstance()->getXmlInt(name+"Width");
    Uint16 pheight = Gamedata::getInstance()->getXmlInt(name+"Height");
    Uint16 srcX = Gamedata::getInstance()->getXmlInt(name+"SrcX");
    Uint16 srcY = Gamedata::getInstance()->getXmlInt(name+"SrcY");
   for (unsigned i = 0; i < num; i++)
        {
         unsigned frameX = i*pwidth + srcX;   
         unsigned frameX1 = pwidth*(num-1-i) + srcX;   
         bulletFrameRight.push_back(new Frame(ibulletSurface, pwidth, pheight,frameX, srcY) );   
         bulletFrameLeft.push_back(new Frame(bulletSurface, pwidth, pheight,frameX1,srcX) ); 
        } 
   

 }

Bullets::Bullets(const Bullets& b) :
  name(b.name),
  strategy(b.strategy),
  bulletSurface(b.bulletSurface),
  ibulletSurface(b.ibulletSurface),
  bulletFrameLeft(b.bulletFrameLeft),
  bulletFrameRight(b.bulletFrameRight), 
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList), 
  freeList(b.freeList)
{ }

bool Bullets::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void Bullets::shoot(const Vector2f& position, const Vector2f& velocity) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      Bullet b(name, position, velocity, bulletFrameLeft,bulletFrameRight );
      bulletList.push_back( b );
    }

    else {
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      bulletList.push_back( b );
    }

		timeSinceLastFrame = 0;
	}
}

void Bullets::draw() const { 
    std::list<Bullet>::const_iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->draw();
    ++ptr;
  }
}

void Bullets::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
