#include <iostream>
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "sprite.h"

class Bullet : public TwowayMultiframeSprite{
public:
  explicit Bullet(const string& name, 
                   const Vector2f& pos, const Vector2f vel, 
                   std::vector<Frame*>& fmsL, std::vector<Frame*>& fmsR) :
    TwowayMultiframeSprite(name,pos,vel,fmsL,fmsR),
    distance(0), 
    maxDistance(Gamedata::getInstance()->getXmlInt(name+"Distance")), 
    tooFar(false),
    positionP(pos),
    velocityV(vel),
    name(name)
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  bool collidedWith(const Drawable* obj) const;

private:
  float distance;
  float maxDistance;
  bool tooFar;
  Vector2f positionP;
  Vector2f velocityV;
 
  const string& name;
};
