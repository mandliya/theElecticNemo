#ifndef TWOWAY_MULTISPRITE__H
#define TWOWAY_MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"

class CollisionStrategy;
class TwowayMultiframeSprite : public Drawable {
public:
  TwowayMultiframeSprite(const std::string& n, 
                   std::vector<Frame*>& fmsLeft,
                   std::vector<Frame*>& fmsRight);
TwowayMultiframeSprite(const std::string& n, 
                   std::vector<Frame*>& fmsLeft,
                   std::vector<Frame*>& fmsRight, std::vector<Frame*>& fmsCLeft,
                   std::vector<Frame*>& fmsCRight);
  TwowayMultiframeSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel,std::vector<Frame*>& fmsLeft,
                    std::vector<Frame*>& fmsRight);
  TwowayMultiframeSprite(const std::string& n);
  TwowayMultiframeSprite(const TwowayMultiframeSprite& s);
  TwowayMultiframeSprite( const std::string& name,float scale);
  virtual int getType() const {return 3; }
  TwowayMultiframeSprite(const std::string& name, const Vector2f&, const Vector2f);

  virtual ~TwowayMultiframeSprite();
  virtual const Frame* getFrame() const { return frames[currentFrame]; }
  virtual void setFrame(Frame* f) { frames[currentFrame] = f; }
  unsigned getPixel(Uint32, Uint32) const;
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void cupdate(Uint32 ticks);
  virtual bool collidedWith(const Drawable*) const;
  float getScale() const{ return scale;}

protected:
  CollisionStrategy* strategy;
  std::vector<Frame *> framesLeft;
  std::vector<Frame *> framesRight;
 std::vector<Frame *> framesCLeft;
  std::vector<Frame *> framesCRight;
  std::vector<Frame *> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float scale;
  void advanceFrame(Uint32 ticks);
  TwowayMultiframeSprite& operator=(const TwowayMultiframeSprite&);
};



#endif
