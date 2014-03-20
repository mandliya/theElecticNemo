#include <vector>
#include <list>
#include "chunk.h"
#include "drawable.h"

class ExplodingSprite : public Drawable{
public:
  ExplodingSprite(const Drawable*);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual  int getType() const {return 6; }
  virtual const Frame* getFrame() const { return frame;}
  unsigned getPixel(Uint32, Uint32) const;

  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  const Frame* frame;
  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};
