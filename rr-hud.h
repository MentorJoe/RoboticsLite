/* -*- C++ -*- */

#ifndef _rr_hud_h_
#define _rr_hud_h_

class rr_hud;
class rr_thud;
class rr_schud;
class rr_geschud;
class rr_counthud;
class rr_mp_rdyhud;

class rr_hud{
public:
  rr_hud();
  virtual ~rr_hud();
  virtual void render(SDL_Surface*);
};

class rr_thud:public rr_hud{
public:
  rr_thud(int*,int*,int*);
  void render(SDL_Surface*);
private:
  int* gameptr;
  int* ctdnptr;
  int* timeptr;
};

class rr_schud:public rr_hud{
public:
  rr_schud(int,int,int*,int*);
  void render(SDL_Surface*);
private:
  int color;
  int position;
  int* valueptr;
  int* rdyptr;
};

class rr_geschud:public rr_hud{
public:
  rr_geschud(int*,int*);
  ~rr_geschud();
  void render(SDL_Surface*);
private:
  int* totr;
  int* totb;
  int cdr;
  int cdb;
};

class rr_counthud:public rr_hud{
public:
  rr_counthud(int,int,int,int,int*,bool);
  void render(SDL_Surface*);
private:
  int* countptr;
  int width;
  int fontsize;
  int cntrx;
  int basey;
  bool colorsign;
};

class rr_mp_readyhud:public rr_hud{
public:
  rr_mp_readyhud(int,int*,int*);
  void render(SDL_Surface*);
private:
  int side;
  int* rdyptr;
  int* gameptr;
};

#endif
