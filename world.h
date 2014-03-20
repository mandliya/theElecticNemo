#include "frame.h"
#include "viewport.h"

class World {
public:
  World(Frame* const frame, int fact = 1) : 
      frame1(*frame), frame2(*frame), 
      frameWidth( frame1.getWidth() ),
      worldWidth( Gamedata::getInstance()->getXmlInt("worldWidth") ),
      factor(fact),
      viewX(0.0), viewY(0.0), 
      view(Viewport::getInstance()) 
  {}
  void update();
  void draw() const;
private:
  Frame const frame1;
  Frame const frame2;
  unsigned frameWidth;
  unsigned worldWidth;
  int factor;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
