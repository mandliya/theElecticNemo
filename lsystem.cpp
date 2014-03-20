#include <cstdlib>
#include <cmath>
#include <fstream>
#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include "aaline.h"
#include "lsystem.h"
#include "ioManager.h"
using std::cout; using std::endl;

bool isUpper (char ch) { return (ch >= 'A' && ch <= 'Z'); }

bool isLetter (char ch) { 
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

LSystem::LSystem(const string &filename) :
  screen(  IOManager::getInstance().getScreen() ), 
  lsystemData( Lsystemdata::getInstance(filename) ),
  xmlData( lsystemData.getXmlData() ),
  spriteSurface(NULL),
  width(0), height(0),

  state(State()),

  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  MAGENTA( SDL_MapRGB(screen->format, 0xff, 0x00, 0xff) ),
  WHITE( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  levels(0),
  angle(0),
  startX(0),
  startY(0),
  strokeLength(1),
  strokeWidth(1),
  facing(0),
  strokeColor(BLACK),
  startSymbol('F'),
  grammar(),
  stateStack( ),
  sentence( ) 
  {
    //cout << "Loading L-System Spec for " + filename + " ..." << endl;
    loadSpecification();
    //displaySystem();
    //cout << "Building a sentence in the language ..." << endl;
    buildSentence();
    //cout << "The sentence: " << sentence << endl;
    //cout << "Making an image ..." << endl;
    fillSprite();

}

LSystem::~LSystem() {
  //std::cout << "Good bye Lindenmeyer" << std::endl;
}

void LSystem::changeColor(Uint32 newColor) {
  strokeColor = newColor;
  fillSprite();
}

unsigned LSystem::findXmlVal( const string& val ) const {
  std::map<string, string>::const_iterator ptr = xmlData.find(val);
  string data  = ptr->second;
  if ( ptr == xmlData.end() ) {
    throw string("Couldn't find ")+val;
  }
  return atoi( data.c_str() );
}

void LSystem::loadSpecification() {
  width = findXmlVal(string("width"));
  height = findXmlVal(string("height"));
  spriteSurface = 
    SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0,0,0,0); 

  levels = findXmlVal("levels");

  angle = findXmlVal("angle");
  startX = findXmlVal("startX");
  startY = findXmlVal("startY");
  strokeLength = findXmlVal("strokeLength");
  strokeWidth = findXmlVal("strokeWidth");

  unsigned red = findXmlVal("red");
  unsigned blue = findXmlVal("blue");
  unsigned green = findXmlVal("green");
  strokeColor = SDL_MapRGB(screen->format, red, green, blue);
  try {
    facing = findXmlVal("facing");
    state.facing = facing;
  }
  catch( const string& msg ) {
  }

  std::map<string, string>::const_iterator ptr = 
    xmlData.find("startSymbol");
  if ( ptr != xmlData.end() ) {
    string data  = ptr->second;
    startSymbol = data[0];
  }
  else throw string("Couldn't find start symbol!");
  grammar = lsystemData.getGrammar(); 
}

void LSystem::fillSprite() { 
  SDL_LockSurface(spriteSurface);
  // WHITE is the background that looks good with most other colors,
  // but feel free to use another color for your background.
  // Above, I've defined BLACK and MAGENTA, as well as WHITE.
  SDL_FillRect(spriteSurface, NULL, MAGENTA);
  state.x = startX;
  state.y = startY;
  for (unsigned int i = 0; i < sentence.size(); ++i) {
    switch ( sentence[i] ) {
      case '-' :  state.facing -= angle; break;
      case '+' :  state.facing += angle; break;
      case '[' :  stateStack.push(state); break;
      case ']' :  {
        state = stateStack.top();
        stateStack.pop();
        break;
      }
      case 'X' : case 'Y' : case 'f' :  {
        int x1 = state.x +
            static_cast<int>(strokeLength * cos(state.facing*M_PI/180));
        int y1 = state.y + 
            static_cast<int>(strokeLength * sin(state.facing*M_PI/180));
        state.x = state.x + (x1-state.x);
        state.y = state.y + (y1-state.y);
        break;
      }
      case 'F' :  case 'A' : case 'B' : {
        int x1 = state.x + 
            static_cast<int>(strokeLength * cos(state.facing*M_PI/180));
        int y1 = state.y + 
            static_cast<int>(strokeLength * sin(state.facing*M_PI/180));
        if ( state.x < 0 || state.y < 0 || x1 < 0 || y1 < 0 ) {
          throw string("x, y, x1, or x2 negative; length too long?");
        }
        Draw_AALine(spriteSurface, state.x,state.y, x1,y1, strokeWidth,
                    strokeColor);
        state.x = state.x + (x1-state.x);
        state.y = state.y + (y1-state.y);
        break;
      }
      default : {
        cout << "sentence size is: " << sentence.size() << endl;
        cout << "i is: " << i << endl;
        throw string("Unhandled switch/case: ")+sentence[i];
      }
    }
  }
  SDL_UnlockSurface(spriteSurface);
}

void LSystem::draw() const { 
  SDL_Rect src = { 0, 0, width, height };    
  SDL_Rect dest = {0, 0, width, height };
  SDL_BlitSurface(spriteSurface, &src, screen, &dest);
}

void LSystem::buildSentence() {
  int count = 0;
  sentence = startSymbol;
  while ( count < levels ) {
    string new_sentence;
    for (unsigned int i = 0; i < sentence.size(); ++i) {
      char ch = sentence[i];
      if ( isUpper(ch) ) {
        new_sentence += grammar.find(ch)->second;
      }
      else if ( ch == '+' || ch == '-' ) new_sentence+= ch;
      else if ( ch == '[' || ch == ']' ) new_sentence+= ch;
      else if ( ch == 'f' ) new_sentence+= ch;
    }
    sentence = new_sentence;
    ++count;
  }
}

void LSystem::printGrammar() const {
  cout << endl << "Grammar:" << endl;
  cout << "Non-T" << '\t' << "RHS" << endl;
  std::map<char, string>::const_iterator it = grammar.begin();
  while ( it != grammar.end() ) {
    cout << it->first << "\t" << it->second << endl;
    ++it;
  }
  cout << endl;
}

void LSystem::displaySystem() const {
  cout << "levels: " << levels << endl;
  cout << "startSymbol: " << startSymbol << endl;
  cout << "startX: " << startX << endl;
  cout << "startY: " << startY << endl;
  cout << "angle: " << angle << endl;
  cout << "facing: " << state.facing << endl;
  cout << "strokeLength: " << strokeLength << endl;
  cout << "strokeWidth: " << strokeWidth << endl;
  cout << "strokecolor: " << strokeColor << endl;
  printGrammar();
}
