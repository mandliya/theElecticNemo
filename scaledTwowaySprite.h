#ifndef SCALED_TWOWAY_MULTISPRITE__H
#define SCALED_TWOWAY_MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"
#include <map>

class ScaledTwowaySprite : public Drawable {
public:
  ScaledTwowaySprite(const std::string&, SDL_Surface*);
  ScaledTwowaySprite(const ScaledTwowaySprite& s);
  virtual ~ScaledTwowaySprite();
  virtual  int getType() const {return 5; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { return frames[currentFrame]; }
  
  float getScale() const{ return scale;}

private:
  double scale;
  SDL_Surface* scaledSurface1;
  SDL_Surface* scaledSurface2;
  //std::map<std::string,Frame * > mulFrames;
  std::vector<Frame*> framesLeft;
  std::vector<Frame*> framesRight;
  std::vector<Frame*> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  
  

  void advanceFrame(Uint32 ticks);
  void makeFish(const std::string& name);
  unsigned getPixel(Uint32, Uint32) const;
  ScaledTwowaySprite& operator=(const ScaledTwowaySprite&);


};

class ALess {
public:
  bool operator()(const ScaledTwowaySprite* lhs, const ScaledTwowaySprite* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};
#endif
