#include <sstream>
#include "menu.h"
#include "parseXML.h"


Menu::~Menu() {
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
}

Menu::Menu() : 
  io( IOManager::getInstance() ),
  words(),
  wordsub(), 
  position(),
  space(),
  parser("xmlSpec/menu.xml"),
  surfaceOff(),
  surfaceOn(),
  clicks(),
  nextIcon(0),
  click(0),
  nextIconsub(0),
  viewport( Viewport::getInstance() )
{ 
  parser.parseXML();
  surfaceOff = io.loadAndSet( parser.getXmlStr("clickoffFile"), true);  
  surfaceOn =  io.loadAndSet( parser.getXmlStr("clickonFile"), true);
  unsigned int n = parser.getXmlInt("wordsNumber");
  unsigned int n2 = parser.getXmlInt("wordsubNumber");
  
 position[0] = parser.getXmlInt("wordsX");
 position[1] = parser.getXmlInt("wordsY");
   
  std::stringstream strm;
  for ( unsigned i = 0; i < n; ++i ) {
    strm << "words" << i;
    words.push_back( parser.getXmlStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

  for ( unsigned i = 0; i < n2; ++i ) {
    strm << "wordsub" << i;
    wordsub.push_back( parser.getXmlStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

 
  space[0] = parser.getXmlInt("wordsSpaceX");
  space[1] = parser.getXmlInt("wordsSpaceY");


  Frame clickoff(surfaceOff, 
              parser.getXmlInt("clickoffWidth"), 
              parser.getXmlInt("clickoffHeight"), 
              parser.getXmlInt("clickoffSrcX"), 
              parser.getXmlInt("clickoffSrcX")); 
  clicks.push_back( clickoff );

  Frame clickon(surfaceOn, 
              parser.getXmlInt("clickonWidth"), 
              parser.getXmlInt("clickonHeight"), 
              parser.getXmlInt("clickonSrcX"), 
              parser.getXmlInt("clickonSrcX")); 
  clicks.push_back( clickon );

}

void Menu::draw() const {
  viewport.update();
  int x =position[0];
  int y =position[1];
  for (unsigned i = 0; i < words.size(); ++i) {
    io.printMessageAt(words[i], x, y);
    y += space[1];
  }
  y = position[1] + nextIcon*space[1];
  clicks[click].draw( position[0]-space[0], y);
}


void Menu::drawsub() const { 
   viewport.update();
  int x =position[0];
  int y =position[1];
  for (unsigned i = 0; i < wordsub.size(); ++i) {
    io.printMessageAt(wordsub[i], x, y);
    y += space[1];
  }
  
  y = position[1] + nextIconsub*space[1];
  clicks[click].draw( position[0]-space[0], y);
}

const string& Menu::getIconClicked() const { 
  return words[nextIcon]; 
} 

const string& Menu::getsubIconClicked() const { 
  return wordsub[nextIconsub]; 
} 

