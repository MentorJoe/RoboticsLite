#include"rr-renderable.h"

#include<stdio.h>

rr_renderable::~rr_renderable(){}

void rr_renderable::render(SDL_Surface*){
  fprintf(stderr,"Should not be here. Debug and set Breakpoint. rr-renderable.cpp\n");
}

void rr_renderable::logicTick(){
  tickCount++;
}

int rr_renderable::getTickCount(){
  return tickCount;
}

bool rr_renderable::isDone(){
  return true;
}

void rr_renderable::update(){}
