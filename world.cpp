#include <iostream>
#include "world.h"

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % frameWidth;
  viewY = view.Y();
}

void World::draw() const { 
  frame1.draw(viewX, viewY, 0, 0); 
  frame2.draw(0, viewY, frameWidth-viewX, 0); 
}

