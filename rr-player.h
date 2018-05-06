/* -*- C++ -*- */

#ifndef _rr_player_h_
#define _rr_player_h_

#include"rr-entity.h"

class rr_player:public rr_entity{
public:
  rr_player(int*,int,rr_level*);
  void render(SDL_Surface*,int,int);
  void setInptr(short*);
  short* getInptr();
  void setColor(int);
  void setPosition(int,int);
  void setDirection(int);
  void setInput(std::shared_ptr<rr_inputsys>);
  void logicTick(rr_level*);
  int getpNum();
  void setReadyPtr(int*,int);
  void getPosition(int*,int*);
  void getDirection(int*);
  bool setHasItem(bool);
  bool getHasItem();
  int pollEvent(rr_event*);
  int getTeam();
  bool isCaptain();
  void taunt();
  rr_subtimer penaltyLocationTimer;
private:
  std::shared_ptr<rr_inputsys> input;
  short* inptr;
  int* readyptr;
  int* gStatePtr;
  int readyptrval;
  int color;
  int playernum;
  int direction;
  int xp;
  int yp;
  bool hasItem;
  rr_level* parent;
  // Set Direction:
  unsigned char setdir;
  int taunttick;
};
#endif
