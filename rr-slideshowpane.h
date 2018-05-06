/* -*- C++ -*- */
#ifndef _rr_slideshowpane_h_
#define _rr_slideshowpane_h_

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"rr-renderable.h"

class rr_slideshowpane:public rr_renderable{
public:
  rr_slideshowpane(const char*);
  ~rr_slideshowpane();
  void render(SDL_Surface*);
  void logicTick();
  int getTickCount();
  bool isDone();
  void update();
private:
  SDL_Surface* paneSfrsw = NULL;
  int tickCount = 0;
};

#endif
