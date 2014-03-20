#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>

#include "drawable.h"

class MultiframeSprite : public Drawable {
public:
  MultiframeSprite(const std::string& n, const std::vector<Frame*>& fms);
  MultiframeSprite(const std::string& n); 
  MultiframeSprite(const MultiframeSprite& s);
  MultiframeSprite( const std::string& name, unsigned r);
  MultiframeSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel, const std::vector<Frame*>& fms);
  virtual ~MultiframeSprite() { } 
  virtual const Frame* getFrame() const { return frames[currentFrame]; }
  virtual int getType() const {return 2; }
  unsigned getPixel(Uint32, Uint32) const;
  virtual void draw() const;
  virtual void update(Uint32 ticks);

protected:
  const std::vector<Frame *> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  void advanceFrame(Uint32 ticks);
};
#endif
