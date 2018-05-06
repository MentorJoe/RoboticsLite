#include"rr-slideshowpane.h"
#include"rr-stdinc.h"
#include"rr-sdlinc.h"

rr_slideshowpane::rr_slideshowpane(const char* def){
  if(strStartsWith("img:",def)){
    paneSfrsw = IMG_Load(def+4);
  }
}

rr_slideshowpane::~rr_slideshowpane(){
  if(paneSfrsw != NULL)
    SDL_FreeSurface(paneSfrsw);
}

void rr_slideshowpane::render(SDL_Surface* sf){
  SDL_BlitSurface(paneSfrsw,NULL,sf,NULL);
}

void rr_slideshowpane::logicTick(){
  tickCount++;
}

int rr_slideshowpane::getTickCount(){
  return tickCount;
}

bool rr_slideshowpane::isDone(){
  return tickCount > 150;
}

void rr_slideshowpane::update(){
  tickCount = 0;
}
