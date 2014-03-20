#include <string>
#include <iostream>

#include "multisprite.h"

class Dying: public MultiframeSprite {
public:
  Dying(const std::string&);
  Dying(const Dying& s);
  virtual ~Dying() { } 
  Dying& operator=(const Dying&);
  virtual void update(Uint32 ticks);
  bool isDead() const { return dead; }

private:
  const std::string& dyingname;
  bool dead;
  float timeSinceLastFrame;
  int dieInterval;
  void advanceFrame(Uint32 ticks);
};
