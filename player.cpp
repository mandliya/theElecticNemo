#include "player.h"
#include "ioManager.h"
#include "collisionStrategy.h"

extern int keyPCheck;


Player::~Player() {
  for (unsigned i = 0; i < fishFrames.size(); ++i) {
    delete fishFrames[i];
    delete fishFramesLeft[i];
    delete fishCFrames[i];
    delete fishCFramesLeft[i];
  }
  SDL_FreeSurface(fishSurface);
  delete fish;
  delete strategy;
}

Player::Player() :
  gdata( Gamedata::getInstance() ),
  keyPressed(false),
  worldWidth( gdata->getXmlInt("worldWidth") ), 
  worldHeight( gdata->getXmlInt("worldHeight") ), 
  fishInitialVelocity( Vector2f(gdata->getXmlInt("fishSpeedX"), 
                               gdata->getXmlInt("fishSpeedY") )
  ), 
  fishWidth( gdata->getXmlInt("fishWidth") ), 
  fishHeight( gdata->getXmlInt("fishWidth") ),
  fishSurface( 
    IOManager::getInstance().loadAndSet(gdata->getXmlStr("fishFile"), 
    gdata->getXmlBool("fishTransparency") )
  ),
  fishFrames(),
  fishFramesLeft(),
  fishCFrames(),
  fishCFramesLeft(),
  fish( NULL ),
  strategy( new PerPixelCollisionStrategy )
{ 
  makefish();
}

void Player::makefish() {
  unsigned numberOfFrames = gdata->getXmlInt("fishFrames");
  Uint16 pwidth = gdata->getXmlInt("fishWidth");
  Uint16 pheight = gdata->getXmlInt("fishHeight");
  Uint16 srcX = gdata->getXmlInt("fishSrcX");
  Uint16 srcY = gdata->getXmlInt("fishSrcY");
  Uint16 srcLeftY = gdata->getXmlInt("fishLeftSrcY");
  Uint16 srcLeft2Y = 2 * (gdata->getXmlInt("fishLeftSrcY"));
  Uint16 srcLeft3Y = 3 * (gdata->getXmlInt("fishLeftSrcY"));


  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * pwidth + srcX;

     fishFrames.push_back( 
      new Frame(fishSurface, pwidth, pheight, frameX, srcY) );

    fishFramesLeft.push_back( 
      new Frame(fishSurface, pwidth, pheight, frameX, srcLeftY) );

    fishCFrames.push_back( 
      new Frame(fishSurface, pwidth, pheight, frameX, srcLeft2Y) );

   fishCFramesLeft.push_back( 
      new Frame(fishSurface, pwidth, pheight, frameX, srcLeft3Y) );
 

  }
  fish = new ShootingSprite("fish", fishFramesLeft, fishFrames, fishCFramesLeft, fishCFrames );
}


void Player::update(Uint32 ticks) { 
  fish->update(ticks); 

  if (!keyPressed ) {
	stop();
   }
  keyPressed = false;

}

void Player::cupdate(Uint32 ticks) { 
  fish->cupdate(ticks); 
  if (!keyPressed ) {
	fish->cupdate(ticks); 
	stop();
  }
  keyPressed = false;
}

void Player::stop() { 
  fish->velocityX(0);  
  fish->velocityY(0);  
}

void Player::right() { 
  keyPressed = true;
  keyPCheck = 1;
  if ( fish->X() < worldWidth-fishWidth) {
    fish->velocityX(fishInitialVelocity[0]);
  }
} 
void Player::left()  { 
  keyPressed = true;
  keyPCheck = 2;
  if ( fish->X() > 0) {
    fish->velocityX(-fishInitialVelocity[0]);
  }
} 
void Player::up()    { 
  keyPressed = true;
  if ( fish->Y() < Gamedata::getInstance()->getXmlInt("fishMinY") ) {
    fish->velocityY( abs( fish->velocityY() ) );
  }
  else {
    fish->velocityY(-fishInitialVelocity[0]);
  }
} 
void Player::down()  { 
  keyPressed = true;
  if ( fish->Y() < worldHeight-fishHeight) {
    fish->velocityY(fishInitialVelocity[0]);
  }
} 

void Player::shoot(){
	fish->shoot();
} 

float  Player::X() const{
	return fish->X();
}

float Player::Y() const{
	return fish->Y();
}

bool Player::collidedWith(const Drawable* obj) const{

  return fish->collidedWith(obj);
}

bool Player::collision(const Drawable* obj) const{
  const Drawable* fishp  =  static_cast<Drawable*> (fish);
  return strategy->execute(*fishp, *obj);
}


