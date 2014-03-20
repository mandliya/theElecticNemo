#ifndef SINGLE__H
#define SINGLE__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
using std::string;
#include "gamedata.h"

class IOManager {
public:
  static IOManager& getInstance();
  SDL_Surface * getScreen() const { return screen; }
  ~IOManager() { 
    TTF_CloseFont(font);
  }
  SDL_Surface* loadAndSet(const string& filename, bool setcolorkey) const;

  void printMessageAt(const string& msg, Uint32 x, Uint32 y) const;
  void printMessageCenteredAt(const string& msg, Uint32 y) const;
  void printStringAfterMessage(const string&, Uint32 x, Uint32 y) const;

  template <typename T>
  void printMessageValueAt(const string& msg, T value, 
         Uint32 x, Uint32 y) const;

  void buildString(SDL_Event);
  void clearString() { inputString = ""; }
  const string& getString() const { return inputString; }
private:
  IOManager();
  IOManager(const IOManager&);
  IOManager& operator=(const IOManager&);
  const Gamedata* gdata;
  int viewWidth;
  int viewHeight;
  const unsigned MAX_STRING_SIZE;
  SDL_Surface * screen;
  TTF_Font *font;
  SDL_Color color;
  SDL_Color color1;
  string title;
  string inputString;
};
#endif
