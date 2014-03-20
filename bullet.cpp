#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"


extern int keyPCheck;

bool Bullet::collidedWith(const Drawable* obj) const {
    return TwowayMultiframeSprite::collidedWith(obj);
   
}
 
void Bullet::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;

	if ( getVelocity().magnitude() > 1.0 ) {
  		advanceFrame(ticks);
  	}
  
 
       if(keyPCheck == 1){
       		frames = framesLeft;
   	} 
        else if(keyPCheck == 2){
	      velocityX(-abs(velocityV[0]));
	      frames = framesRight; 
	}else{
	      frames = framesLeft;
	}

	   Vector2f pos = getPosition();
	  setPosition(getPosition() + incr); 

  	distance += ( hypot(X()-pos[0], Y()-pos[1]) );
  	if (distance > maxDistance) tooFar = true;
   
}

