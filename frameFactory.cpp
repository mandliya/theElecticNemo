#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"
#include "deleteT.h"
#include "algorithm"
#include "SDL/SDL_rotozoom.h"
#include <sstream>

FrameFactory::~FrameFactory() {
  std::cout << "The FrameFactory is down sizing!" << std::endl;
  
 for(unsigned i = 0 ; i < surfaces.size(); ++i){
	SDL_FreeSurface(surfaces[i]);
}
  
  std::map<std::string, Frame*>::iterator itFrame = frames.begin();
  while ( itFrame != frames.end() ) {
    delete itFrame->second;
    ++itFrame;
  }
  std::map<std::string, std::vector<Frame*> >::iterator iterateMultiFrame = multiFrames.begin();
  while ( iterateMultiFrame != multiFrames.end() ) {
    for_each((iterateMultiFrame->second).begin(),(iterateMultiFrame->second).end(),DeleteT());

		iterateMultiFrame->second.clear();
    ++iterateMultiFrame;
  }
  std::map<std::string, Frame*>::iterator lsysIt1 = lsysFrames.begin();
  while ( lsysIt1 != lsysFrames.end() ) {
    delete lsysIt1->second;
    ++lsysIt1;
  }




  std::map<std::string, std::vector<Frame*> >::iterator iterateTwoWayMultiFrame = twoWayMultiFrames.begin();
   while ( iterateTwoWayMultiFrame != twoWayMultiFrames.end() ) {
    for_each((iterateTwoWayMultiFrame->second).begin(),(iterateTwoWayMultiFrame->second).end(),DeleteT());

		iterateTwoWayMultiFrame->second.clear();
    ++iterateTwoWayMultiFrame;
        
  }


 
  std::map<std::string, LSystem*>::iterator lsysIt = lsystems.begin();
  while ( lsysIt != lsystems.end() ) {
    delete lsysIt->second;
    ++lsysIt;
  }



}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata->getXmlStr(name+"File"),
          gdata->getXmlBool(name+"Transparency"));
    surfaces.push_back(surface);

    Frame * const frame =new Frame(surface,
                gdata->getXmlInt(name+"Width"), 
                gdata->getXmlInt(name+"Height"),
                gdata->getXmlInt(name+"SrcX"), 
                gdata->getXmlInt(name+"SrcY")); 
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}


std::vector<Frame *> FrameFactory::getMultiFrame(const std::string& name) {

  std::map<std::string, std::vector<Frame *> >::const_iterator pos = multiFrames.find(name); 

  if ( pos == multiFrames.end() ) {

    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata->getXmlStr(name+"File"),
          gdata->getXmlBool(name+"Transparency"));
    surfaces.push_back(surface);
  
    std::vector<Frame *> vecFrames;
    unsigned num = gdata->getXmlInt(name+"Frames"); 
    Uint16 pwidth = gdata->getXmlInt(name+"Width");
    Uint16 pheight = gdata->getXmlInt(name+"Height");
    Uint16 srcX = gdata->getXmlInt(name+"SrcX");
    Uint16 srcY = gdata->getXmlInt(name+"SrcY");

    for (unsigned i = 0; i < num; i++)
        {
         unsigned frameX = i*pwidth + srcX;
         vecFrames.push_back(new Frame(surface, pwidth, pheight, frameX, srcY) ); 
        }                          
 
    multiFrames[name] = vecFrames;
    return vecFrames;
  }
  else {
    return pos->second;
  }
}




std::vector<Frame *> FrameFactory::getTwoWayMultiFrame(const std::string& name) {

 std::map<std::string, std::vector<Frame *> >::const_iterator pos = twoWayMultiFrames.find(name); 

 if ( pos == twoWayMultiFrames.end() ) {

    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata->getXmlStr(name+"File"),
          gdata->getXmlBool(name+"Transparency"));
    surfaces.push_back(surface);
  
    std::vector<Frame *> multiFramesLeft;
    unsigned num = gdata->getXmlInt(name+"LeftFrames"); 
    Uint16 pwidth = gdata->getXmlInt(name+"Width");
    Uint16 pheight = gdata->getXmlInt(name+"Height");
    Uint16 srcX = gdata->getXmlInt(name+"SrcX");
    Uint16 srcLeftY = gdata->getXmlInt(name+"LeftSrcY");


    for (unsigned i = 0; i < num; ++i) 
    {
       unsigned frameX = i * pwidth + srcX;
       multiFramesLeft.push_back( 
       new Frame(surface, pwidth, pheight, frameX, srcLeftY) );
    }
                         
 
    twoWayMultiFrames[name] = multiFramesLeft;
    return multiFramesLeft;
  }
  else {
    return pos->second;
  }
}

LSystem* FrameFactory::getLsystem(const string& name) const {
  std::map<string, LSystem*>::const_iterator pos = lsystems.find(name); 
  if ( pos == lsystems.end() ) {
    throw string("Couldn't find L-System")+name+string("in getLsystem");
  }
  else {
    return pos->second;
  }
}



Frame* FrameFactory::getLsystemFrame(const string& name) {
  std::map<string, Frame*>::const_iterator pos = lsysFrames.find(name); 
  if ( pos == lsysFrames.end() ) {
    LSystem* lsystem = new LSystem(gdata->getXmlStr(name+"File"));
    lsystems[name] = lsystem;
    SDL_Surface * const surface = lsystem->getSurface();

    bool transparency = gdata->getXmlBool(name+"Transparency"); 
    if ( transparency ) {
      // We need to make the L-System surface transparent for Magenta:
      Uint32 colorkey = SDL_MapRGB(surface->format, 255, 0, 255);
      // Note that we're violating const with a C function:
      SDL_SetColorKey(surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
    }

    surfaces.push_back(surface);
    Frame * const frame =new Frame(surface,
                gdata->getXmlInt(name+"Width"), 
                gdata->getXmlInt(name+"Height"), 
                gdata->getXmlInt(name+"SrcX"), 
                gdata->getXmlInt(name+"SrcY")); 
    lsysFrames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}






