#include <string>
#include <iostream>

#include "twowayMultisprite.h"

class DyingTwo: public TwowayMultiframeSprite {
public:
  DyingTwo(const std::string&);
  DyingTwo(const DyingTwo& s);
  virtual ~DyingTwo() { } 
  DyingTwo& operator=(const DyingTwo&);
  virtual void update(Uint32 ticks);
  bool isDead() const { return dead; }

private:
  const std::string& dyingname;
  bool dead;
  float timeSinceLastFrame;
  int dieInterval;
  void advanceFrame(Uint32 ticks);
};
