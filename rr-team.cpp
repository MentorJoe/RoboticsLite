#include"rr-stdinc.h"

rr_team::rr_team(){
  scoreptr = NULL;
}

rr_team::~rr_team(){

}

void rr_team::addMember(std::shared_ptr<rr_player> member){
  playerz.push_back(std::shared_ptr<rr_player>(member));
}
void rr_team::setScorePtr(int* ptr){
  scoreptr = ptr;
}

void rr_team::onTick(){

}
