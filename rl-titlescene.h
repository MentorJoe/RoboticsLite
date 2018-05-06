/* -*- C++ -*- */
#ifndef _rl_titlescene_h_
#define _rl_titlescene_h_

#include"rr-renderable.h"
#include<vector>
#include<memory>

class rl_titlescene:public rr_renderable{
public:
  rl_titlescene();
  ~rl_titlescene();
  void render(SDL_Surface*);
  void logicTick();
  int getTickCount();
  bool isDone();
  void update();
private:
  int tickCount;
  std::vector< std::shared_ptr<rr_entity> > entityList;
  std::shared_ptr<rr_player>
};

#endif
