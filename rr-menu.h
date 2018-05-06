/* -*- C++ -*- */
#ifndef _rr_menu_h_
#define _rr_menu_h_

#include"rr-sdlinc.h"

class rr_menu{
public:
  rr_menu();
  ~rr_menu();
  virtual void render(SDL_Surface*);
  virtual void onAction(int);
  void onInput(SDL_Event*);
  virtual void onRawEvent(SDL_Event*);
protected:
  int selected;
  int numSelections;
  bool reqsRawEvents = false;
private:
  int stickdone;
};

class rr_mainmenu:public rr_menu{
public:
  rr_mainmenu();
  ~rr_mainmenu();
  void render(SDL_Surface*);
  void onAction(int);
};

class rr_optmenu:public rr_menu{
public:
  rr_optmenu();
  ~rr_optmenu();
  void render(SDL_Surface*);
  void onAction(int);
};

class rr_kbdmenu:public rr_menu{
public:
  rr_kbdmenu(int);
  ~rr_kbdmenu();
  void render(SDL_Surface*);
  void onAction(int);
  void onRawEvent(SDL_Event*);
private:
  int number;
  int keyboard;
  bool edit;
};

#endif
