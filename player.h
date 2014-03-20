#ifndef PLAYER_H
#define PLAYER_H

#include "gamedata.h"
#include "shootingSprite.h"


class Player{
public:
	Player();
	virtual ~Player();
	
	virtual void update(Uint32 ticks);
	virtual void cupdate(Uint32 ticks);
	virtual void draw() const{ fish->draw();}
	const Drawable* getSprite() const { return fish; }
	const Frame* getFrame() const { return fish->getFrame(); }
	bool collidedWith(const Drawable*) const;
	bool collision(const Drawable*) const;
	void stop();
	void shoot();
        void right();
  	void left();
  	void up();
  	void down();
	float X() const;
	float Y() const;

private:
  	const Gamedata* gdata;
  	bool keyPressed;
  	int worldWidth;
  	int worldHeight;
  	Vector2f fishInitialVelocity;
  	float fishWidth;
  	float fishHeight;
  	SDL_Surface *fishSurface;
  	std::vector<Frame*> fishFrames;
  	std::vector<Frame*> fishFramesLeft;
        std::vector<Frame*> fishCFrames;
        std::vector<Frame*> fishCFramesLeft;
  	ShootingSprite* fish;
	CollisionStrategy* strategy;
  	void makefish();
  	Player(const Player&);
  	Player& operator=(const Player&);
	

};
#endif
