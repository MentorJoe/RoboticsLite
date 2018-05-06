/* -*- C++ -*- */
#ifndef _rr_team_h_
#define _rr_team_h_

#include<vector>
#include<memory>

class rr_team{
public:
  rr_team();
  ~rr_team();
  void addMember(std::shared_ptr<rr_player>);
  void setScorePtr(int*);
  void onTick();
private:
  int* scoreptr;
  std::vector<std::shared_ptr<rr_player>> playerz;
};

#endif
