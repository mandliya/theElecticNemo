#include <vector>
#include <string>
#include "frame.h"
#include "ioManager.h"
#include "vector2f.h"
#include "viewport.h"

class Menu {
public:
  Menu();
  ~Menu();
  void draw() const;
  void drawsub() const;
  void lightOn() { click = 1; }
  void lightOff() { click = 0; }
  void increment() { nextIcon = (nextIcon + 1) % words.size(); }
  void incrementsub() { nextIconsub = (nextIconsub + 1) % wordsub.size(); }
  void decrement() { 
    nextIcon = (nextIcon + words.size()-1) % words.size(); 
  }
  void decrementsub() { 
    nextIconsub = (nextIconsub + wordsub.size()-1) % wordsub.size(); 
  }
  const string& getIconClicked() const;
  const string& getsubIconClicked() const;
private:
  const IOManager& io;
  std::vector<std::string> words; 
  std::vector<std::string> wordsub; 
  Vector2f position;
  Vector2f space;

  ParseXML parser;
  SDL_Surface* surfaceOff;
  SDL_Surface* surfaceOn;
  std::vector<Frame> clicks;
  unsigned nextIcon;
  unsigned click;
  unsigned nextIconsub;
  Viewport& viewport;
 

  Menu(const Menu&);
  Menu& operator=(const Menu&);
};
