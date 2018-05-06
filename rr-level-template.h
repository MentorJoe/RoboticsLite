/* -*- C++ -*- */
#ifndef _rr_level_template_
#define _rr_level_template_
// So the .h files aren't included multiple times

#include"rr-level.h"

class rr_level_template:public rr_level{
public:
  rr_level_template();
  ~rr_level_template();
  void logicTick();
  void endGame();
  void render(SDL_Surface*);
};

#endif
