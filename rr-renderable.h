/* -*- C++ -*- */
#ifndef _rr_renderable_
#define _rr_renderable_

#include"rr-sdlinc.h"

class rr_renderable{
public:
  virtual ~rr_renderable();
  virtual void render(SDL_Surface*);
  virtual void logicTick();
  virtual int getTickCount();
  virtual bool isDone();
  virtual void update();
  // Change the name of update to reset... more accurate discription of
  // what it really does.
private:
  int tickCount = 0;
};

#endif
