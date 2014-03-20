#include "ioManager.h"
#include "lsystemSprite.h"
#include "frameFactory.h"

LsystemSprite::LsystemSprite( const string& name ) :
  Sprite( name, 
         FrameFactory::getInstance().getLsystemFrame(name)
        ), 
  lsystem(FrameFactory::getInstance().getLsystem(name))
{ }
