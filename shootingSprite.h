#include <string>
#include <iostream>
#include "twowayMultisprite.h"
#include "bullets.h"

class ShootingSprite : public TwowayMultiframeSprite {
public:
  ShootingSprite(const std::string& n,std::vector<Frame*>& fmsLeft,
                   std::vector<Frame*>& fmsRight,std::vector<Frame*>& fmsCLeft,
                   std::vector<Frame*>& fmsCRight);
  ShootingSprite(const ShootingSprite& s);
  

  virtual void update(Uint32 ticks);
  virtual void cupdate(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  virtual bool collidedWith(const Drawable*) const;

private:
  std::string bulletName;
  Bullets bullets;
  float minSpeed;
  
  ShootingSprite& operator=(const ShootingSprite&);
};
