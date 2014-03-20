// Brian Malloy        Game Construction

#include "menuManager.h"
Gamedata* Gamedata::instance;
 MenuManager game_manager;
float SmartSprite::safeDistance = game_manager.getSafeDistance();

int keyPCheck=0; 


int main(int, char*[]) {
   srand((unsigned)time(0));
   try {
     
      game_manager.play();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
