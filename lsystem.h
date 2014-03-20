#include <map>
#include <string>
#include <stack>
#include <iostream>

#include <SDL/SDL.h>
#include "lsystemdata.h"


class LSystem {
  public:
    LSystem(const std::string &filename);
    ~LSystem();
    SDL_Surface* getSurface() const { return spriteSurface; }
    void changeStrokeLength(int newLength) { strokeLength = newLength; }

    void draw() const;
    void buildSentence();
    void fillSprite();
    void displaySystem() const;
    void changeColor(Uint32);
  private:
    struct State {
      State() : facing(0), x(0), y(0) {}
      int facing;
      int x;
      int y;
    };
  private:
    SDL_Surface * const screen;
    Lsystemdata& lsystemData;
    std::map<std::string, std::string> xmlData;
    SDL_Surface *spriteSurface;
    unsigned width, height;

    State state;

    const Uint32 BLACK;
    const Uint32 MAGENTA;
    const Uint32 WHITE;
    int levels;
    int angle;
    int startX;
    int startY;
    int strokeLength;
    int strokeWidth;
    int facing;
    Uint32 strokeColor;
    char startSymbol;
    std::multimap<char, std::string> grammar;
    std::stack<State>  stateStack;
    std::string sentence;

    unsigned findXmlVal( const std::string& val ) const;
    void printGrammar() const;
    void loadSpecification();
    void drawSprite() const;

    LSystem(const LSystem&);
    LSystem& operator=(const LSystem&);
};
