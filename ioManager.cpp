#include <iostream>
#include <iomanip>
#include "ioManager.h"
using std::string;

IOManager& IOManager::getInstance() {
  static IOManager io;
  return io;
}

IOManager::IOManager( ) :
  gdata( Gamedata::getInstance() ),
  viewWidth( gdata->getXmlInt("viewWidth") ),
  viewHeight( gdata->getXmlInt("viewHeight") ),
  MAX_STRING_SIZE( gdata->getXmlInt("maxStringSize") ),
    // The 3rd and 4th parameters are just as important as the first 2!
    screen(SDL_SetVideoMode(viewWidth, viewHeight, 32, SDL_DOUBLEBUF)),
    font( NULL ), 
    color(),
    color1(),
    title( gdata->getXmlStr("screenTitle") ),
    inputString("")  
{
  if (screen == NULL) { 
    throw string("Unable to set video mode; screen is NULL in IOMAnager"); 
  }
  if ( TTF_Init() == -1 ) {
    throw string("TTF_Init failed: ") + TTF_GetError();
  }
  font = TTF_OpenFont(
         Gamedata::getInstance()->getXmlStr("fontFile").c_str(), 
         Gamedata::getInstance()->getXmlInt("fontSize")
         );
  if ( !font ) {
    throw string("TTF_OpenFont failed: ") + TTF_GetError();
  }
  color.r = Gamedata::getInstance()->getXmlInt("fontRed");
  color.g = Gamedata::getInstance()->getXmlInt("fontGreen");
  color.b = Gamedata::getInstance()->getXmlInt("fontBlue");
  color1.r = Gamedata::getInstance()->getXmlInt("font1Red");
  color1.g = Gamedata::getInstance()->getXmlInt("font1Green");
  color1.b = Gamedata::getInstance()->getXmlInt("font1Blue");
  color.unused = Gamedata::getInstance()->getXmlInt("fontUnused");
  SDL_EnableUNICODE( SDL_ENABLE );
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(TTF_Quit);
}

SDL_Surface* IOManager::loadAndSet(const string& filename, bool setcolorkey) const {
  SDL_Surface *tmp = IMG_Load(filename.c_str());
  if (tmp == NULL) {
    throw string("Unable to load bitmap ")+filename;
  }
  if ( setcolorkey ) {
    Uint32 colorkey = SDL_MapRGB(tmp->format, 255, 0, 255);
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
  }
  // Optimize the strip for fast display
  SDL_Surface *image = SDL_DisplayFormatAlpha(tmp);
  if (image == NULL) {
    image = tmp;
  } 
  else {
    SDL_FreeSurface(tmp);
  }
  return image;
}

void IOManager::printMessageAt(const string& msg, Uint32 x, Uint32 y) const {
   SDL_Rect dest = {x,y,0,0};
   SDL_Surface * stext = TTF_RenderText_Blended(font, msg.c_str(), color1);
   if (stext) {
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageAt");
   }
}

void IOManager::printMessageCenteredAt( const string& msg, Uint32 y) const {
   SDL_Surface *stext = TTF_RenderText_Blended(font, msg.c_str(), color);
   if (stext) {
     Uint32 x = ( viewWidth - stext->w ) / 2;
     SDL_Rect dest = {x,y,0,0};
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageCenteredAt");
   }
}

template <typename T>
void IOManager::printMessageValueAt(const string& msg, T value, 
     Uint32 x, Uint32 y) const {
   std::stringstream strm;
   std::string message = msg;
   strm << message << value << "\0";
   message = strm.str();
   SDL_Rect dest = {x,y,0,0};
   SDL_Surface *stext = 
       TTF_RenderText_Blended(font, message.c_str(), color);
   if (stext) {
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageValueAt");
   }
}

void IOManager::printStringAfterMessage( const string& msg,
       Uint32 x, Uint32 y ) const {
   printMessageAt(msg+inputString, x, y);
}

void IOManager::buildString(SDL_Event event) {
  if( inputString.size() <= MAX_STRING_SIZE) {
    unsigned ch = event.key.keysym.sym;
    if ( isalpha(ch) || isdigit(ch) || ch == ' ') {
      inputString += char(event.key.keysym.unicode);
    }
  }     
  if( event.key.keysym.sym == SDLK_BACKSPACE
      && inputString.length() > 0 ) {
      // remove a character from the end
      int length = inputString.size();
      inputString.erase( length - 1 );
  }
}

template void IOManager::
printMessageValueAt(const string& msg, float, Uint32, Uint32) const;

template void IOManager::
printMessageValueAt(const string& msg, unsigned, Uint32, Uint32) const;

template void IOManager::
printMessageValueAt(const string& msg, int, Uint32, Uint32) const;

