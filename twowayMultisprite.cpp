#include <iostream>
#include <cmath>
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "collisionStrategy.h"

TwowayMultiframeSprite::~TwowayMultiframeSprite(){
	delete strategy;
}

extern int keyPCheck;
void TwowayMultiframeSprite::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name, 
                  std::vector<Frame*> & fmsLeft,
                  std::vector<Frame*> & fmsRight) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY"))
           ),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  framesCLeft(fmsLeft),
  framesCRight(fmsRight),
  frames(fmsLeft),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames")),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval")),
  scale(1.0)
{ }

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name, 
                  std::vector<Frame*> & fmsLeft,
                  std::vector<Frame*> & fmsRight,std::vector<Frame*> & fmsCLeft,
                  std::vector<Frame*> & fmsCRight) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY"))
           ),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  framesCLeft(fmsCLeft),
  framesCRight(fmsCRight),
  frames(fmsLeft),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames")),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval")),
  scale(1.0)
{ }

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name,  const Vector2f& pos, const Vector2f& vel, 
                  std::vector<Frame*> & fmsLeft,
                  std::vector<Frame*> & fmsRight) :
  Drawable(name,pos,vel),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  framesCLeft(fmsLeft),
  framesCRight(fmsRight),
  frames(fmsRight),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames")),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval")),
  scale(1.0)
{ }


TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name) :
  Drawable(name, 
          Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")),  
           Vector2f(
       (rand()%2?1:-1)*
       getRand(100, Gamedata::getInstance()->getXmlInt(name+"SpeedX")),
       (rand()%2?1:-1)*
       getRand(100, Gamedata::getInstance()->getXmlInt(name+"SpeedY")))
           ),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesRight(FrameFactory::getInstance().getMultiFrame(name)),
  framesCLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesCRight(FrameFactory::getInstance().getMultiFrame(name)),
  frames(FrameFactory::getInstance().getMultiFrame(name)),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") ),
  scale(1.0)
{ }

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name, float scale) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY"))
           ),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesRight(FrameFactory::getInstance().getMultiFrame(name)),
  framesCLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesCRight(FrameFactory::getInstance().getMultiFrame(name)),
  frames(FrameFactory::getInstance().getMultiFrame(name)),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") ),
  scale(1.0-scale)
{ }


TwowayMultiframeSprite::TwowayMultiframeSprite(const string& name, const Vector2f& pos, const Vector2f vel) :
  Drawable(name, pos, vel),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesRight(FrameFactory::getInstance().getMultiFrame(name)),
  framesCLeft(FrameFactory::getInstance().getTwoWayMultiFrame(name)),
  framesCRight(FrameFactory::getInstance().getMultiFrame(name)),
  frames(FrameFactory::getInstance().getMultiFrame(name)),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") ),
  scale(1.0)
{ }





bool TwowayMultiframeSprite::collidedWith(const Drawable* obj) const {
  return strategy->execute(*this, *obj);
}



TwowayMultiframeSprite::
TwowayMultiframeSprite(const TwowayMultiframeSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  strategy( new MidPointCollisionStrategy ),
  framesLeft(s.framesLeft),
  framesRight(s.framesRight),
  framesCLeft(s.framesLeft),
  framesCRight(s.framesRight),
  frames(s.framesRight),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  scale(1.0)
  { }

void TwowayMultiframeSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwowayMultiframeSprite::update(Uint32 ticks) { 
   if ( getVelocity().magnitude() > 1.0 ) {
  	advanceFrame(ticks);
  } 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  int limit = Gamedata::getInstance()->getXmlInt("skyLimit");

  if ( Y()-limit < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  

  if ( velocityX() < 0 ) frames = framesLeft;
  if ( velocityX() > 0 ) frames = framesRight;
}

void TwowayMultiframeSprite::cupdate(Uint32 ticks) { 

  advanceFrame(ticks);
  
  if ( velocityX() <=0 && (keyPCheck==2)  ) {
	
	frames = framesCLeft;
  }
  if ( velocityX() >= 0 && (keyPCheck==0 || keyPCheck==1)  ) {
		frames = framesCRight;
   }
  
  
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  int limit = Gamedata::getInstance()->getXmlInt("skyLimit");

  if ( Y()-limit < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  

  
}


unsigned TwowayMultiframeSprite::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frames[currentFrame]->getSurface()->pixels);
  return pixels[ ( y * frames[currentFrame]->getWidth() ) + x ];
}
