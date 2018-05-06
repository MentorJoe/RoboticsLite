
#include"rr-stdinc.h"
#include"rr-hscorefile.h"
#include"rl-confdir.h"
#include<SDL2/SDL_endian.h>
#include<stdlib.h>
#include<string>
#include<sstream>
#ifdef __unix__
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#endif
#ifdef _WIN32
#include<Windows.h>
#endif
#ifdef __APPLE__
#include<sys/types.h>
#include<sys/stat.h>
#endif

/* DEPRECATED */
std::string getHSFileName(){
#ifdef __APPLE__ // Potatoe UNIX
  std::stringstream ostream;
  ostream << getenv("HOME") << "/Library/Robotics Lite";
  if(mkdir(ostream.str().c_str(),0755) != 0){
    if(errno != EEXIST){
      return ".rl-highscore";
    }
  }
  ostream << "/rl-highscore";
  return ostream.str();
#elif defined(__unix__) // Sensible Linuxes and UNIXes
  std::stringstream ostream;
  const char* home;
  if((home = getenv("HOME")) == NULL)
    home = getpwuid(getuid())->pw_dir;
  ostream << home << "/.roboticslite";
  if(mkdir(ostream.str().c_str(),0755) != 0){
    if(errno != EEXIST){
      return ".rl-highscore";
    }
  }
  ostream << "/highscores";
  return ostream.str();
#elif defined(_WIN32) // The Swiss-army Chainsaw Butterfly of Computer Bodges
  std::stringstream ostream;
  const char* appdata;
  if((appdata = getenv("APPDATA")) == NULL)
    return "rr-highscores";
  ostream << appdata << "roborumble";
  if(!CreateDirectory((LPCTSTR) ostream.str().c_str(),NULL)){
    if(GetLastError() == ERROR_PATH_NOT_FOUND){
      return "rr-highscores";
    }
  }
  ostream << "/rr-highscores";
  return ostream.str();
#else // Undefined, probably using DOS, you hipster, hence why this is 8.3 compatible
  return "rr_hisco.res";
#endif
}

rr_highScoreDisplay::rr_highScoreDisplay(){
#ifdef DEBUG
  printf("If attempting to open teh actual HS File, I'd open %s\n",rl_confdir::getHSFileName().c_str());
#endif
  tickcount = 0;
  FILE* fp = fopen(rl_confdir::getHSFileName().c_str(),"rb");
  list.reserve(9);
  if(fp == NULL){
    // Fallback to actually have sample high scores displayed
    sHighScoreEntry entry;
    entry.color = 17;
    strcpy(entry.name,"TEAM9001");
    entry.score = 300;
    list.push_back(entry);
    entry.color = 18;
    strcpy(entry.name,"TEAM9002");
    entry.score = 275;
    list.push_back(entry);
    entry.color = 17;
    strcpy(entry.name,"TEAM9003");
    entry.score = 250;
    list.push_back(entry);
    entry.color = 18;
    strcpy(entry.name,"TEAM9004");
    entry.score = 225;
    list.push_back(entry);
    entry.color = 17;
    strcpy(entry.name,"TEAM9005");
    entry.score = 200;
    list.push_back(entry);
    entry.color = 18;
    strcpy(entry.name,"TEAM9006");
    entry.score = 175;
    list.push_back(entry);
    entry.color = 17;
    strcpy(entry.name,"TEAM9007");
    entry.score = 150;
    list.push_back(entry);
    entry.color = 18;
    strcpy(entry.name,"TEAM9008");
    entry.score = 100;
    list.push_back(entry);
  }
  else if(fp != NULL){
    for(int i=0;i<8;i++){
      sHighScoreEntry entry;
      bzero(&entry,sizeof(sHighScoreEntry));
      fread(&entry,sizeof(sHighScoreEntry),1,fp);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
      entry.score = SDL_SwapBE32(entry.score);
#endif
      list.push_back(entry);
    }
    fclose(fp);
  }
}

rr_highScoreDisplay::~rr_highScoreDisplay(){
  if(hasChanged){
    FILE* fp = fopen(rl_confdir::getHSFileName().c_str(),"wb");
    if(fp != NULL){
#ifdef DEBUG
      printf("Opened High Scores File for writing\n");
#endif
      for(int i=0;i<8;i++){
	sHighScoreEntry entry = list[i];
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	entry.score = SDL_SwapBE32(entry.score);
#endif
	fwrite(&entry,sizeof(sHighScoreEntry),1,fp);
      }
      fclose(fp);
    }
    else{
      fprintf(stderr,"Failed to open high score file for writing.\n");
    }
  }
}

void rr_highScoreDisplay::insert(sHighScoreEntry e){
  printf("In insert\n");
  std::vector<sHighScoreEntry>::iterator iterator = list.begin();
  do{
#ifdef DEBUG
    printf("%x %s %d\n",iterator->color,iterator->name,iterator->score);
#endif
    if(e.score > iterator->score){
      hasChanged = true;
      list.insert(iterator,e);
      break;
    }
    iterator++;
  }while(iterator != list.end());
}

void rr_highScoreDisplay::render(SDL_Surface* sf){
  TTF_Font* font = rr_ttf::load("ttf/profont.ttf",48);
  SDL_Color colors[] = {
    {255,255,255}, // 0
    {255,0,0}, // 1
    {0,0,255}, // 2
    {0,0,0}, // 3
    {0,0,0}, // 4
    {0,0,0}, // 5
    {0,0,0}, // 6
    {0,0,0}, // 7
    {0,0,0},
    {0,0,0}, // 9
    {0,0,0}, // 10
    {0,0,0}, // 11
    {0,0,0}, // 12
    {0,0,0}, // 13
    {0,0,0}, // 14
    {0,0,0}, // 15
    {0,0,0}, // 16
    {128,0,0}, // 17
    {0,0,128} // 18
  };
  SDL_Surface* tx;
  SDL_Rect blitdest;
  char* barf = new char[64];
  if(tickcount % 60 < 30){
    tx = TTF_RenderText_Blended(font,"HIGH SCORES",colors[0]);
    blitdest.x = 400-tx->w/2;
    blitdest.y = 48;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
  }

  for(int i=0;i<8;i++){
    sprintf(barf,"%8s %5d",list[i].name,list[i].score);
    int color = list[i].color;
    if((color & 0xFF) == 0xFF) color = 1;
    tx = TTF_RenderText_Blended(font,barf,colors[color]);
    blitdest.x = 400-tx->w/2;
    blitdest.y = 132 + 48*i;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
  }

  delete barf;
}

void rr_highScoreDisplay::logicTick(){
  tickcount++;
}

int rr_highScoreDisplay::getTickCount(){
  return tickcount;
}

bool rr_highScoreDisplay::isDone(){
  return tickcount > 300;
}

int getHighScoreQualifyingScore(){
  FILE* fp = fopen(rl_confdir::getHSFileName().c_str(),"rb");
  std::vector<sHighScoreEntry> list;
  list.reserve(8);
  if(fp == NULL){
    return 100; // Default List yields a lowest of 100
  }
  else if(fp != NULL){
    for(int i=0;i<8;i++){
      sHighScoreEntry entry;
      bzero(&entry,sizeof(sHighScoreEntry));
      fread(&entry,1,sizeof(sHighScoreEntry),fp);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
      entry.score = SDL_SwapBE32(entry.score);
#endif
      list[i] = entry;
    }
    fclose(fp);
    return list[7].score;
  }
  else{
    return -1;
  }
}
