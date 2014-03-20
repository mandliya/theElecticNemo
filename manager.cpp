#include <cmath>
#include "manager.h"
#include "frameFactory.h"
#include "scaledTwowaySprite.h"
#include "scaledSprite.h"
#include "aaline.h"
#include<algorithm>
#include "random.h"
#include "sound.h"
#include "dying.h"
#include "dyingtwo.h"



Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
	std::list<Drawable*>::iterator oIt=objects.begin();
  	while ( oIt != objects.end() ) {
   		 delete *oIt;
    		++oIt;
  	}
	std::list<Drawable*>::iterator dIt=deadObjects.begin();
  	while ( dIt != deadObjects.end() ) {
   		 delete *dIt;
    		++dIt;
  	}

	std::list<ExplodingSprite*>::iterator expIt=explodingObjects.begin();
  	while ( expIt != explodingObjects.end() ) {
    		delete *expIt;
    		++expIt;
 	 }

	std::list<ScaledTwowaySprite*>::iterator It=scaledFish.begin();
  		while ( It != scaledFish.end() ) {
    			delete *It;
    			++It;
  	}
	  SDL_FreeSurface(SmallFishSurface);
	  delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  player(),
  screen( io.getScreen() ),
  SmallFishSurface( io.loadAndSet(gdata->getXmlStr("sfishFile"), true) ),
  worldBack(FrameFactory::getInstance().getFrame("background"),3),
  worldFront(FrameFactory::getInstance().getFrame("front")),
  viewport( Viewport::getInstance() ),
  currObject(0),
  objects(),
  deadObjects(),
  scaledFish(),
  explodingObjects(),
  collisionCount(0),
  TICK_INTERVAL( 1* 1000/(gdata->getXmlInt("fps")) ),
  nextTime(0),
  bar(screen),
  Sharkbar(screen,20),
  collcount(0),
  level(0),
  gameOver(false),
  youWon(false),
  safeDistance(30),
  leveladd(30)
 {
  	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
   	 throw string("Unable to initialize SDL: ");
     	}
 
  	atexit(SDL_Quit);
	makeSmallFish();
  	viewport.setObjectToTrack(player.getSprite());
	clock.pause();
 }


void Manager::setDifficultyLevel(){

	unsigned n = gdata->getXmlInt("numberOfObjects");
	unsigned octn = gdata->getXmlInt("octoNumber");
	unsigned ngfish = gdata->getXmlInt("numberOfgFish");
	
	


	std::cout<<"level:"<<level<<std::endl;
	if(level ==1 || level == 0){
		octn = octn + 0;
		ngfish = ngfish + 0;
		leveladd = 30;
		
	}
	else if(level == 2){
		octn = octn + 0;
		ngfish = ngfish + 0;	
		leveladd = 15;	
		
	}
	else{
		octn = octn + 0;
		ngfish = ngfish + 0;
		leveladd = 0;
	}
	

	objects.push_back(new TwowayMultiframeSprite("shark"));
	objects.push_back(new LsystemSprite("fractaltree"));
	objects.push_back(new MultiframeSprite("net"));
        objects.push_back(new LsystemSprite("fractaltree"));
        for(unsigned i=0;i < n;i++){
		objects.push_back(new TwowayMultiframeSprite("smallfish"));
	}
	for(unsigned i=0;i < octn;i++){
		objects.push_back(new SmartSprite("octo", player));
	}
	for(unsigned i=0;i < ngfish;i++){
		objects.push_back(new TwowayMultiframeSprite("gFish"));
	}
}


void Manager::makeSmallFish() {
  unsigned numberOfSmallFish= gdata->getXmlInt("numberOfSmallFish");
  
  for (unsigned i = 0; i < numberOfSmallFish; ++i) {
         scaledFish.push_back(new ScaledTwowaySprite("sfish",SmallFishSurface) );
  }
  scaledFish.sort(ALess());
 
  
std::list<ScaledTwowaySprite*>::iterator scIt=scaledFish.begin();
  		while ( scIt != scaledFish.end() ) {
    			std::cout<<(*scIt)->getScale()<<std::endl;
    			++scIt;
  	}
}



void Manager::draw() const {
  worldBack.draw();
  
 
  std::list<ScaledTwowaySprite*>::const_iterator scIt=scaledFish.begin();
  		while ( scIt != scaledFish.end() ) {
    			(*scIt)->draw();
    			++scIt;
  	}
   
  worldFront.draw();
  std::list<ExplodingSprite*>::const_iterator expIt=explodingObjects.begin();
  while ( expIt != explodingObjects.end() ) {
    (*expIt)->draw();
    ++expIt;
  }
  player.draw();
  bar.draw();
  Sharkbar.draw();
   std::list<Drawable*>::const_iterator it = objects.begin();
  	while ( it != objects.end() ) {
    		(*it)->draw();
    		++it;
  	}
   std::list<Drawable*>::const_iterator dIt=deadObjects.begin();
  	while ( dIt != deadObjects.end() ) {
   		 (*dIt)->draw();
    		++dIt;
  	}
  if(gameOver)io.printMessageAt("GAME OVER",390 ,300); 
  if(youWon)io.printMessageAt("You Won!",390 ,300); 
  
}


void Manager::update(Uint32 ticks){

    worldBack.update();
    worldFront.update();
    player.update(ticks);   
    std::list<Drawable*>::iterator itu = objects.begin();
    while ( itu != objects.end() ) {
		(*itu)->update(ticks);
    		if(player.collision((*itu)) && (*itu)->getName() != "fractaltree" && (*itu)->getName() !="smallfish") {
				std::cout<<"Collision happend with "<<(*itu)->getName()<<std::endl;
				if((*itu)->getName() =="shark") bar.update(ticks, true, 0);
				else bar.update(ticks, false, 0);
				player.cupdate(ticks);
		}
 		
		++itu;
	    }
   
   

    	
    
    std::list<Drawable*>::iterator it = objects.begin();
  	while ( it != objects.end() ) {
    		(*it)->update(ticks);
		
		if ( player.collidedWith((*it)) ) {
			
			if((*it)->getName() != "octo" &&  (*it)->getName() != "gFish" && (*it)->getName() != "shark"){
				
      					explodingObjects.push_back(new ExplodingSprite(*it));
      					delete *it;
      					it = objects.erase(it);
      					++collisionCount;

			}else{
					
    					if ( (*it)->getName() == "octo") {
						Drawable* sprite = dynamic_cast<MultiframeSprite*>(*it);
      						MultiframeSprite* dyingOcto = 
         						new Dying("dyingocto");
      						deadObjects.push_back(dyingOcto);
      						dyingOcto->setPosition( sprite->getPosition() );
						delete sprite;
      						it = objects.erase(it);
     				        	return;
					}
					if ( (*it)->getName() == "gFish") {
						Drawable* sprite = dynamic_cast<TwowayMultiframeSprite*>(*it);
      						TwowayMultiframeSprite* dyingFish = 
         						new DyingTwo("dyingFish");
      						deadObjects.push_back(dyingFish);
      						dyingFish->setPosition( sprite->getPosition() );
						delete sprite;
      						it = objects.erase(it);
     				        	return;
					}
					if ( (*it)->getName() == "shark") {
						collcount++;
 						Sharkbar.update(ticks, false, leveladd);
						std::cout<<collcount<<std::endl;
						if(Sharkbar.getCurrentHealth() <= 0 ){
      							explodingObjects.push_back(new ExplodingSprite(*it));
							delete *it;
      							it = objects.erase(it);
							return;
    						}
						else ++it;
					}				
			}
    		}
    		else ++it; 
    		
  	}
        
    std::list<ExplodingSprite*>::iterator expIt=explodingObjects.begin();
  		while ( expIt != explodingObjects.end() ) {
    		(*expIt)->update(ticks);
		ExplodingSprite* exsprite = dynamic_cast<ExplodingSprite*>(*expIt);
		if(exsprite->chunkCount() == 0 && exsprite->getName() == "net"){
			std::cout<<"NoChunk"<<std::endl;
			objects.push_back(new MultiframeSprite("net"));
			std::cout<<"Net Created"<<std::endl;	
			delete exsprite;
      			expIt = explodingObjects.erase(expIt);
			return;
		}
		else if(exsprite->chunkCount() == 0 && exsprite->getName() == "shark"){
			std::cout<<"NoChunk"<<std::endl;
			delete exsprite;
      			expIt = explodingObjects.erase(expIt);
			youWon =  true;			
			return;
		}	
		else{			
		 ++expIt;
		}
 	 }
     
    std::list<ScaledTwowaySprite*>::const_iterator scIt=scaledFish.begin();
  		while ( scIt != scaledFish.end() ) {
    			(*scIt)->update(ticks);
    			++scIt;
  	}
    std::list<Drawable*>::iterator dIt=deadObjects.begin();
    while ( dIt != deadObjects.end() ) {
		if(dynamic_cast<Dying*>(*dIt) && static_cast<Dying*>(*dIt)->isDead()){
    				delete (*dIt);
				objects.push_back(new SmartSprite("octo", player));
				dIt = deadObjects.erase(dIt);
 		}

        	else{
   				 (*dIt)->update(ticks);
				 ++dIt;
        	 }
  	}
   std::list<Drawable*>::iterator d2It=deadObjects.begin();
    while ( d2It != deadObjects.end() ) {
		if(dynamic_cast<DyingTwo*>(*d2It) && static_cast<DyingTwo*>(*d2It)->isDead()){
    				delete (*d2It);
				objects.push_back(new TwowayMultiframeSprite("gFish"));
				d2It = deadObjects.erase(d2It);
 		}

        	else{
   				 (*d2It)->update(ticks);
				 ++d2It;
        	 }
  	}
	
    
    viewport.update();
    
}

void Manager::displayHud(){
			 //viewport.draw();

			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr(viewport.getObjectToTrack()->getName()+"Caption"),
    				gdata->getXmlInt("messageY"));
   			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr("hubCaption1"),
    				(gdata->getXmlInt("messageY")+20));
			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr("hubCaption2"),
    				(gdata->getXmlInt("messageY")+35));
			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr("hubCaption3"),
    				(gdata->getXmlInt("messageY")+50));
			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr("hubCaption4"),
    				(gdata->getXmlInt("messageY")+65));
			IOManager::getInstance().printMessageCenteredAt(
    				gdata->getXmlStr("hubCaption5"),
    				(gdata->getXmlInt("messageY")+80));
			
     			Sint16 y1 = (gdata->getXmlInt("messageY")-5);
			Sint16 y2 = (gdata->getXmlInt("messageY")+120);
    		        const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);

     		
    		        Sint16 x1 = 30, x2 = gdata->getXmlInt("viewWidth")-30;
    		        Draw_AALine(screen, x1,y1, x2,y1, RED);
			Draw_AALine(screen, x1,y1, x1,y2, RED);
               		Draw_AALine(screen, x1,y2, x2,y2, RED);
			Draw_AALine(screen, x2,y1, x2,y2, RED);
			io.printMessageValueAt("FPS: ", clock.getFps(),(gdata->getXmlInt("messageY")) , (gdata->getXmlInt("messageY")+10)); 

}

	
	
	




void Manager::play() {

   viewport.setObjectToTrack(player.getSprite());
   setDifficultyLevel();
  
  clock.unpause();
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  bool helpKey = false;
  int n;
  SDLSound sound;

  while ( ! done ) {
    if(gameOver || youWon) done = true;
    
    draw();
    if(helpKey){
	displayHud();
    }
    
    
    io.printMessageAt("Fish HEALTH:",10 ,5); 
    io.printMessageAt("Shark HEALTH:",400 ,5); 

    SDL_Flip(screen);
   
    ++clock;

    Uint32 ticks = clock.getElapsedTicks();
    update(ticks);
    if ( nextTime > SDL_GetTicks( ) ) SDL_Delay( nextTime - SDL_GetTicks( ) );
    nextTime = SDL_GetTicks( ) + TICK_INTERVAL ;    

    if(bar.getCurrentHealth() <= 0 ){
	 
	 sound[3];
	 gameOver=true;
         SDL_Delay(2500);
	 
     }    	
     if(youWon){
	
	 sound[4];
	 youWon=true;
	 SDL_Delay(2500);
	 
     }    

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYUP) { keyCatch = false;}
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE : {
          clock.pause(); 
          done = true; 
          break;
        }
        case SDLK_q      : done = true; break;
	case SDLK_t      : {
          if (!keyCatch) {
            keyCatch = true;
	    if (viewport.getObjectToTrack()->getName() == "fish") {
              currObject = (currObject+1) % 2;
	      std::list<Drawable*>::iterator it = objects.begin();
    		std::advance(it, currObject);
              viewport.setObjectToTrack(*it);
            }
            else {
              viewport.setObjectToTrack(player.getSprite());
            }
            
          }
          break;
        }
	case SDLK_F1	:{
		if(!keyCatch){
			helpKey=!helpKey;
			keyCatch=true;
		}
	  break;
	}
	case SDLK_0 : case SDLK_1 :  {
          if (!keyCatch) {
            keyCatch = true;
            n = event.key.keysym.sym - SDLK_0;
            sound[n];
            break;
           }
	}
        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
	case SDLK_SPACE      : {
          player.shoot();
	  sound[0];
          break;
        }
        default          : break;
      }
    }
    if (keystate[SDLK_LEFT]) {
      player.left();
     }
    if (keystate[SDLK_RIGHT]) {
      player.right();
    }
    if (keystate[SDLK_UP]) {
      player.up();
    }
    if (keystate[SDLK_DOWN]) {
      player.down();
    }    
    



  }
}
