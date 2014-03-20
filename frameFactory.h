#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "gamedata.h"
#include "lsystem.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
  Frame* getFrame(const std::string&);
  std::vector<Frame *> getMultiFrame(const std::string&);
  std::vector<Frame *> getTwoWayMultiFrame(const std::string&);
  Frame* getLsystemFrame(const string&);
  
  LSystem* getLsystem(const string&) const;

private:
  Gamedata* gdata;
  std::vector<SDL_Surface*> surfaces;
  std::map<std::string, Frame*> frames;
  std::map<std::string,std::vector<Frame *> > multiFrames;
  std::map<std::string,std::vector<Frame *> > twoWayMultiFrames;
  std::map<std::string, Frame*> lsysFrames;
  std::map<std::string, LSystem*> lsystems;

  
  FrameFactory() : 
    gdata( Gamedata::getInstance() ), surfaces(), frames(), multiFrames(), twoWayMultiFrames(),lsysFrames(),lsystems()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);
};
