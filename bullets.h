#include <list>
#include<vector>
#include "bullet.h"
#include "collisionStrategy.h"

class Bullets {
public:
  Bullets(const std::string&);
  Bullets(const Bullets&);
  ~Bullets();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& vel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool shooting() const { return bulletList.empty(); }
  bool collidedWith(const Drawable* obj) const;
private:
  std::string name;
  CollisionStrategy* strategy;
  SDL_Surface *bulletSurface;  
  SDL_Surface *ibulletSurface;

  std::vector<Frame*> bulletFrameLeft;
  std::vector<Frame*> bulletFrameRight;
  
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<Bullet> bulletList; // Bullets is a list of Bullet
  mutable std::list<Bullet> freeList;   // when a Bullet is out of range

  Bullets& operator=(const Bullets&);   // (Item 6)
};
