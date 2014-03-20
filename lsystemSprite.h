#include "sprite.h"

class LSystem;

class LsystemSprite : public Sprite {
public:
  LsystemSprite(const string& n);
  virtual ~LsystemSprite() {}
private:
  LSystem* lsystem;

  LsystemSprite(const LsystemSprite&);
  LsystemSprite& operator=(const LsystemSprite&);
};
