#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include <iostream>
#include "drawable.h"

class CollisionStrategy;
class Sprite : public Drawable {
public:
  Sprite(const std::string& n, const Frame*);
  Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel,
         const Frame*);
  Sprite(const std::string& n);
  Sprite(const Sprite& s);
  virtual ~Sprite();
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(const Frame* f) { frame = f; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned getPixel(Uint32, Uint32) const;
  virtual bool collidedWith(const Sprite*) const;
  virtual int getType() const {return 1; }

  Vector2f getCenter() const { 
    return Vector2f( X()+frame->getWidth()/2, Y()+frame->getHeight()/2 );
  }

protected:
  const Frame * frame;
  CollisionStrategy* strategy;

  int frameHeight;
  int frameWidth;

  int worldHeight;
  int worldWidth;

  int getDistance(const Sprite*) const;
};
#endif
