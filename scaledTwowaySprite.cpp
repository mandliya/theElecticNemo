#include <iostream>
#include <cmath>
#include "scaledTwowaySprite.h"
#include "gamedata.h"
#include "SDL/SDL_rotozoom.h"
#include "random.h"
#include <sstream>




void ScaledTwowaySprite::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

ScaledTwowaySprite::~ScaledTwowaySprite() {
 SDL_FreeSurface( scaledSurface1 );
 SDL_FreeSurface( scaledSurface2 );

 for(unsigned i = 0; i < framesLeft.size(); ++i){
  	delete framesLeft[i];	
}

for(unsigned i = 0; i < framesRight.size(); ++i){
  	delete framesRight[i];	
}

}

ScaledTwowaySprite::ScaledTwowaySprite( const std::string& name, SDL_Surface* origSurface) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(
       (rand()%2?1:-1)*
       getRand(100, Gamedata::getInstance()->getXmlInt(name+"SpeedX")),
       (rand()%2?1:-1)*
       getRand(100, Gamedata::getInstance()->getXmlInt(name+"SpeedY")))
           ),
  scale(getRandFloat(Gamedata::getInstance()->getXmlFloat(name+"MinScale"),
                     Gamedata::getInstance()->getXmlFloat(name+"MaxScale"))
  ),
  scaledSurface1( 0 ),
  scaledSurface2( 0 ),
  framesLeft(),
  framesRight(),
  frames(),
  frameWidth(0),
  frameHeight( 0),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance()->getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance()->getXmlInt(name+"FrameInterval") )
{

  double oldWidth = origSurface->w;
  // need to subtract 0.5 because rotozoom will ceil it!
  double newWidth = 
    numberOfFrames*ceil((oldWidth*scale-0.5)/numberOfFrames);
  //Since scale*width0 = width1, scale = width1/width0
  double newScale = newWidth/oldWidth;
  frameWidth = newScale*Gamedata::getInstance()->getXmlInt(name+"Width"); 
  frameHeight = newScale*Gamedata::getInstance()->getXmlInt(name+"Height");
  scaledSurface1 = rotozoomSurface(origSurface, 0, newScale, SMOOTHING_ON); 
  scaledSurface2 = rotozoomSurface(origSurface, 0, newScale, SMOOTHING_ON); 
  makeFish(name);

}

ScaledTwowaySprite::
ScaledTwowaySprite(const ScaledTwowaySprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  scale(s.scale),
  scaledSurface1(s.scaledSurface1),
  scaledSurface2(s.scaledSurface2),
  framesLeft(s.framesLeft),
  framesRight(s.framesRight),
  frames(s.framesRight),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight")),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval )
  { }

void ScaledTwowaySprite::makeFish(const std::string& name){
 
	  unsigned num1 = Gamedata::getInstance()->getXmlInt(name+"LeftFrames"); 
	  unsigned num2 = Gamedata::getInstance()->getXmlInt(name+"Frames"); 
	        Uint16 pwidth = scaledSurface1->w/numberOfFrames;
  	 	Uint16 pheight = scaledSurface1->h/numberOfFrames;
          	Uint16 srcX = Gamedata::getInstance()->getXmlInt(name+"SrcX");
	        Uint16 srcLeftY = scale * Gamedata::getInstance()->getXmlInt(name+"LeftSrcY");
          	Uint16 srcY = Gamedata::getInstance()->getXmlInt(name+"SrcY");
		std::ostringstream sin;
    		sin << scale;
   	        std::string sval = sin.str();
	 

		for (unsigned i = 0; i < num1; ++i) 
   	 	{
			unsigned frameX = i*pwidth + srcX;
			Frame* newFrame = new Frame( scaledSurface1, pwidth, pheight, frameX, srcLeftY) ;
			framesLeft.push_back(newFrame);
			
		}
 	
	


    		for (unsigned i = 0; i < num2; i++)
       		 {
         		unsigned frameX = i*pwidth + srcX;
			Frame* newFrame = new Frame(scaledSurface2, pwidth, pheight, frameX, srcY);
         		framesRight.push_back(newFrame); 
        	}   
	 
	
	frames = framesRight;
 }

void ScaledTwowaySprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void ScaledTwowaySprite::update(Uint32 ticks) { 
	

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
  if ( X() > worldWidth-frameHeight) {
    velocityX( -abs( velocityX() ) );
  }  

  if ( velocityX() < 0 ) {

	frames = framesLeft;
	
   }
  if ( velocityX() > 0 ){
	
	 frames = framesRight;
  }

}

unsigned ScaledTwowaySprite::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frames[currentFrame]->getSurface()->pixels);
  return pixels[ ( y * frames[currentFrame]->getWidth() ) + x ];
}

