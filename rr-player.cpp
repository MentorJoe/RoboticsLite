
#include"rr-sdlinc.h"
#include"rr-entity.h"
#include"rr-stdinc.h"

SDL_Surface* playerSprite = NULL;

rr_player::rr_player(int* sp,int pn,rr_level* pl):rr_entity(sp,1){
  playernum = pn;
  color = 0;
  direction = 0;
  inptr = NULL;
  gStatePtr = sp;
  readyptr = NULL;
  readyptrval = 0;
  hasItem = false;
  parent = pl;
  taunttick = -1;
}

int rr_player::pollEvent(rr_event* e){
  input->pollEvent(e);
}

short* rr_player::getInptr(){
  return inptr;
}

int rr_player::getTeam(){
  switch(color){
  case 0: case 4: case 6: case 8: return 1;
  case 1: case 5: case 7: case 9: return 2;
  }
}

bool rr_player::isCaptain(){
  return (color == 0 || color == 1);
}

void rr_player::setColor(int c){
  color = c;
}

bool rr_player::setHasItem(bool b){
  if(hasItem == b) return false;
  hasItem = b;
  return true;
}

bool rr_player::getHasItem(){
  return hasItem;
}

void rr_player::getPosition(int* x,int* y){
  *x = xp;
  *y = yp;
}

void rr_player::getDirection(int* d){
  *d = direction;
}

void rr_player::setPosition(int x,int y){
  xp = x;
  yp = y;
}

void rr_player::setDirection(int d){
  direction = d;
}

void rr_player::setInput(std::shared_ptr<rr_inputsys> i){
  input = i;
}

int rr_player::getpNum(){
  return playernum;
}

void rr_player::render(SDL_Surface* sf,int xsp,int ysp){
  if(playerSprite == NULL){
    playerSprite = IMG_Load("imgs/player.png");
  }
  SDL_Rect blitsrc,blitdest;
  blitsrc.w = 32;
  blitsrc.h = 32;
  blitdest.w = 32;
  blitdest.h = 32;
  blitsrc.x = direction*32;
  blitsrc.y = color*32;
  blitdest.x = (xsp+xp*4)-8;
  blitdest.y = (ysp+yp*4)-8;
  SDL_BlitSurface(playerSprite,&blitsrc,sf,&blitdest);

  if(hasItem){
    blitsrc.x = direction*32 + 128;
    blitsrc.y = parent->itemSpriteIndex()*32;
    SDL_BlitSurface(playerSprite,&blitsrc,sf,&blitdest);
  }

#ifdef DEBUG
  char* dbg = new char[256];
  sprintf(dbg,"(%d, %d, %d)",xp,yp,direction);
  TTF_Font* smf = rr_ttf::load("ttf/profont.ttf",12);
  SDL_Surface* tx = TTF_RenderText_Solid(smf,dbg,{255,255,255});
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  delete dbg;
#endif
}

void rr_player::taunt(){
  taunttick = 0;
}

void rr_player::setInptr(short* ptr){
  inptr = ptr;
}

void rr_player::logicTick(rr_level* parent){
  if(*gStatePtr == LEVELSTATE_WAITING){
    if(inptr != NULL && readyptr != NULL){
      if(*inptr != 0){
	*readyptr |= readyptrval;
      }
    }
  }
  else if(*gStatePtr > LEVELSTATE_COUNTDOWN){
    if(inptr != NULL && taunttick >= 0){
      direction++;
      if(direction == 4)
	direction = 0;
      taunttick++;
      if(taunttick == 8)
	taunttick = -1;
    }
    if(inptr != NULL && taunttick == -1){
      // Actual Movement
      if(*inptr & 0x0001) if(parent->tileCollissionDetect(xp,yp,0) == 1 &&
			     parent->playerCollissionDetect(xp,yp,0,0) == 1) yp--;
      if(*inptr & 0x0002) if(parent->tileCollissionDetect(xp,yp,1) == 1 &&
			     parent->playerCollissionDetect(xp,yp,1,0) == 1) xp--;
      if(*inptr & 0x0004) if(parent->tileCollissionDetect(xp,yp,2) == 1 &&
			     parent->playerCollissionDetect(xp,yp,2,0) == 1) yp++;
      if(*inptr & 0x0008) if(parent->tileCollissionDetect(xp,yp,3) == 1 &&
			     parent->playerCollissionDetect(xp,yp,3,0) == 1) xp++;
      
      //   0
      // 1   3
      //   2
      if(*inptr == 0x0000){
	// Reset direction setter if keys are not set.
	setdir = 0;
      }
      if((*inptr & 0x10) != 0x10){
	// Do not change direction if strafe is enganged
	if(setdir == 0){
	  if     (*inptr & 0x0001){ setdir = 1; direction = 0; }
	  else if(*inptr & 0x0002){ setdir = 4; direction = 3; }
	  else if(*inptr & 0x0004){ setdir = 3; direction = 2; }
	  else if(*inptr & 0x0008){ setdir = 2; direction = 1; }
	}
	else if(setdir == 1 && *inptr & 0x0004){ setdir = 3; direction = 2; }
	else if(setdir == 3 && *inptr & 0x0001){ setdir = 1; direction = 0; }
	else if(setdir == 4 && *inptr & 0x0008){ setdir = 2; direction = 1; }
	else if(setdir == 2 && *inptr & 0x0002){ setdir = 4; direction = 3; }
      }
    }
  }
}

void rr_player::setReadyPtr(int* ptr,int val){
  readyptr = ptr;
  readyptrval = val;
}
