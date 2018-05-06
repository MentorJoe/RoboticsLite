/* -*- C++ -*- */
#ifndef _rr_hscorefile_h_
#define _rr_hscorefile_h_

#include"rr-renderable.h"
#include<stdio.h>
#include<vector>

struct sHighScoreEntry{
  /*
   * 1 == RED
   * 2 == BLUE
   *
   * +16 = Sample
   */
  char color;
  char name[11];
  int score;
};

int getHighScoreQualifyingScore();
std::string getHSFileName();

class rr_highScoreDisplay:public rr_renderable{
public:
  rr_highScoreDisplay();
  ~rr_highScoreDisplay();
  void render(SDL_Surface*);
  void insert(sHighScoreEntry);
  void logicTick();
  int getTickCount();
  bool isDone();
private:
  std::vector<sHighScoreEntry> list;
  bool hasChanged;
  int tickcount;
};

#endif
