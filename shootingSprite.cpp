#include "shootingSprite.h"
#include "gamedata.h"

extern int keyPCheck;

ShootingSprite::ShootingSprite(const std::string& n, std::vector<Frame*>& fmsLeft,
                   std::vector<Frame*>& fmsRight,std::vector<Frame*>& fmsCLeft,
                   std::vector<Frame*>& fmsCRight) :
  TwowayMultiframeSprite(n, fmsLeft, fmsRight,fmsCLeft, fmsCRight),
  bulletName( Gamedata::getInstance()->getXmlStr(n+"Bullet") ),
  bullets( bulletName ),
  minSpeed( Gamedata::getInstance()->getXmlInt(bulletName+"SpeedX") )
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  TwowayMultiframeSprite(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed)
{ }

void ShootingSprite::shoot() { 
  float x,y;
  if(velocityX() < Gamedata::getInstance()->getXmlInt("fishSpeedX") || velocityX() == 0 ) {
   if(velocityX() == 0 && (keyPCheck==0 || keyPCheck==1) ){ 

         x = X()+(Gamedata::getInstance()->getXmlFloat("adjustingFactor1")*frameWidth);
   	 y = Y()+(Gamedata::getInstance()->getXmlFloat("adjustingFactor2")*frameHeight/2);
   }else{
   	x = X()-(Gamedata::getInstance()->getXmlFloat("adjustingFactor3")*frameWidth);
   	y = Y()+frameHeight/2;
    }
 }
  else{
    x = X()+(Gamedata::getInstance()->getXmlFloat("adjustingFactor1")*frameWidth);
   	 y = Y()+(Gamedata::getInstance()->getXmlFloat("adjustingFactor2")*frameHeight/2);
 }



  // I'm not adding minSpeed to y velocity:
  bullets.shoot( Vector2f(x, y), 
    Vector2f(minSpeed+velocityX(), velocityY()));
}

bool ShootingSprite::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void ShootingSprite::draw() const { 
  TwowayMultiframeSprite::draw();
  bullets.draw();
}

void ShootingSprite::update(Uint32 ticks) { 
  TwowayMultiframeSprite::update(ticks);
  bullets.update(ticks);
}

void ShootingSprite::cupdate(Uint32 ticks) { 
  TwowayMultiframeSprite::cupdate(ticks);
  bullets.update(ticks);
}

