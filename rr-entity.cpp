
#include"rr-sdlinc.h"
#include"rr-entity.h"

SDL_Surface* entityTilesheet = NULL;

rr_entity::rr_entity(int* sp,int id){
  stateptr = sp;
  etypeid = id;
  xp = 0;
  yp = 0;
}

rr_entity::~rr_entity(){

}

void rr_entity::render(SDL_Surface* sf,int mapxsp,int mapysp){
  if(entityTilesheet == NULL){
    entityTilesheet = IMG_Load("imgs/entities.png");
  }
  SDL_Rect blitsrc,blitdest;
  blitsrc.w = 16;
  blitsrc.h = 16;
  blitdest.w = 16;
  blitdest.h = 16;
  if(etypeid == 2){
    blitsrc.y = 0;
    blitsrc.x = 32;
  }
  blitdest.x = xp*4+mapxsp;
  blitdest.y = yp*4+mapysp;
  if(etypeid == 2){
    SDL_BlitSurface(entityTilesheet,&blitsrc,sf,&blitdest);
  }
}

void rr_entity::setPosition(int x,int y){
  xp = x;
  yp = y;
}

void rr_entity::onOverlap(rr_entity* e){

}

void rr_entity::getPosition(int* x,int* y){
  *x = xp;
  *y = yp;
}

int rr_entity::getType(){
  return etypeid;
}
