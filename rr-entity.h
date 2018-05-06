/* -*- C++ -*- */

#ifndef _rr_entity_h_
#define _rr_entity_h_
class rr_entity{
public:
  rr_entity(int*,int);
  virtual ~rr_entity();
  virtual void render(SDL_Surface*,int,int);
  virtual void onOverlap(rr_entity*);
  virtual void setPosition(int,int);
  virtual void getPosition(int*,int*);
  virtual int getType();
private:
  int* stateptr;
  int etypeid;
  int xp;
  int yp;
};
#endif

