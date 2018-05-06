
#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<assert.h>
#include<time.h>
#include<math.h>

#include"rr-sdlinc.h"
#include"rr-stdinc.h"

SDL_Surface* tiles = NULL;

#ifdef RR_ACE
SDL_Surface* sixtyfouricos = NULL;
#endif

void rr_level::onSecond(){
  // Filler
}

int rr_level::getState(){
  return lState;
}

void rr_level::endGame(){

}

void rr_level::cleanup(){
  rr_input::clearInputs();
}

bool rr_level::isDone(){
  return (lState == LEVELSTATE_DONE);
}

rr_level::rr_level(){
  tilemap = NULL;
  tilesubdata = NULL;
  hsinputname = new char[32];
  strcpy(hsinputname,"0000AAAA");
  hsinputcursor0 = 0;
  hsinputcursor1 = 7;
  width = 0;
  height = 0;
  playerz.reserve(8);
  entitiez.reserve(8);
  hudz.reserve(16);
  addGameHud(std::shared_ptr<rr_hud>(new rr_thud(&lState,&countdown,&leveltime)));
  lState = 1;
  lInputs.reserve(8);
  lInputSides.reserve(8);
}

rr_level::~rr_level(){
  if(tilemap != NULL) delete tilemap;
  if(tilesubdata != NULL) delete tilesubdata;
  if(hudz.size() > 0) hudz.clear();
  if(playerz.size() > 0) playerz.clear();
  if(entitiez.size() > 0) entitiez.clear();
  if(hsinputname != NULL) delete hsinputname;
  if(hsDisplay != NULL) delete hsDisplay;
}

int rr_level::getWidth(){
  return width;
}

int rr_level::getHeight(){
  return height;
}

int rr_level::testLevel(int varia){
  if(tilemap != NULL){
    delete tilemap;
    tilemap = NULL;
  }
  if(tilesubdata != NULL){
    delete tilesubdata;
    tilesubdata = NULL;
  }
  tilemap = new char[24*24];
  bzero(tilemap,sizeof(char)*24*24);
  tilesubdata = new char[24*24];
  bzero(tilemap,sizeof(char)*24*24);
  width = 24;
  height = 24;
  switch(varia){

  }

  // Lay the border of the board, wow, that sentence was so boring.
  for(int y=0;y<24;y++){
    for(int x=0;x<24;x++){
      if(y == 0){
	if(x == 0 || x == 23) continue;
	tilemap[y*24+x] = 0x01;
      }
      else if(y == 23){
	if(x == 0 || x == 23) continue;
	tilemap[y*24+x] = 0x03;
      }
      else if(x == 0) tilemap[y*24+x] = 0x04;
      else if(x == 23) tilemap[y*24+x] = 0x02;
      else tilemap[y*24+x] = 0x00;
    }
  }
}

int rr_level::load(const char* fn){

}

void rr_level::record(const char* fn){

}

void rr_level::addEntity(std::shared_ptr<rr_entity> e){
  for(int i=0;i<entitiez.size();i++){
    if(entitiez[i] == NULL){
      entitiez[i] = std::shared_ptr<rr_entity>(e);
      return;
    }
  }
  entitiez.push_back(std::shared_ptr<rr_entity>(e));
}

void rr_level::onGameStart(){
  // Cuz it's a filler---- filler block.
}

void rr_level::addGameHud(std::shared_ptr<rr_hud> h){
  hudz.push_back(h);
}

void rr_level::logicTick(){
  logicTick(true);
}

void rr_level::logicTick(bool handleGameEvents){
  if(lState == LEVELSTATE_CONFIG){ // Assign inputs to sides
#ifdef RR_ACE
    /*
     * Arcade Cabinet Edition
     *
     * Check which controllers are enabled and set them up to play.
     */
    rr_event e;
    for(int i=0;i<lInputs.size();i++){
      if(lInputs[i]->pollEvent(&e)){
	if(e.type == ACE_AB){
	  ace_ctrlrs_enabled |= (int) pow(2,i);
	}
	if(e.type == START && ace_ctrlrs_enabled != 0){
	  int r = 0,b = 0;
	  losRojos = std::shared_ptr<rr_team>(new rr_team());
	  losAzuls = std::shared_ptr<rr_team>(new rr_team());
	  for(int j=0;j<4;j++){
	    if(ace_ctrlrs_enabled & ((int) pow(2,j))){
	      std::shared_ptr<rr_player> plr(new rr_player(&lState,j,this));
	      plr->setInput(lInputs[j]);
	      plr->setInptr(lInputs[j]->getDirPtr());
	      playerz.push_back(std::shared_ptr<rr_player>(plr));
	      if(j == 0 || j == 1){
		// Red Team
		switch(r){
		case 0:
		  plr->setColor(0);
		  plr->setPosition(4,24);
		  plr->setReadyPtr(&rReadyUp,0b000100);
		  break;
		case 1:
		  plr->setColor(4);
		  plr->setPosition(4,44);
		  plr->setReadyPtr(&rReadyUp,0b001000);
		  break;
		}
		r++;
		plr->setDirection(1);
		losRojos->addMember(plr);
	      }
	      else if(j == 2 || j == 3){
		// Blue Team
		switch(b){
		case 0:
		  plr->setColor(1);
		  plr->setPosition(84,32);
		  plr->setReadyPtr(&bReadyUp,0b000100);
		  break;
		case 1:
		  plr->setColor(5);
		  plr->setPosition(84,12);
		  plr->setReadyPtr(&bReadyUp,0b001000);
		  break;
		}
		b++;
		plr->setDirection(3);
		losAzuls->addMember(plr);
	      }
	    }
	  }
	  rReadyUp = r - 1;
	  bReadyUp = b - 1;
	  addGameHud(std::shared_ptr<rr_hud>(new rr_mp_readyhud(1,&rReadyUp,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_mp_readyhud(2,&bReadyUp,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_schud(0,-2,&redScore,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_schud(1,2,&bluScore,&lState)));
	  lState = LEVELSTATE_WAITING;
	}
      }
    }
#else
    /*
     * Standard Edition
     *
     * Assign inputs to sides. Standard, de-facto method.
     */
    rr_event e;
    for(int i=0;i<lInputs.size();i++){
      int inmask = lInputs[i]->getMovementDir();
      if(inmask & 0x02) lInputSides[i] = 1;
      if(inmask & 0x04) lInputSides[i] = 0;
      if(inmask & 0x08) lInputSides[i] = 2;
      if(lInputs[i]->pollEvent(&e)){
	if(e.type == START){ // Physically assign inputs and start the game
	  lState++;
	  int rcol = 0;
	  int bcol = 0;
	  losRojos = std::shared_ptr<rr_team>(new rr_team());
	  losAzuls = std::shared_ptr<rr_team>(new rr_team());
	  for(int i=0;i<lInputs.size();i++){
	    // Assign Inputs Players && Teams
	    if(lInputSides[i] == 0) continue;
	    std::shared_ptr<rr_player> plr(new rr_player(&lState,i,this));
	    plr->setInput(lInputs[i]);
	    plr->setInptr(lInputs[i]->getDirPtr());
	    playerz.push_back(std::shared_ptr<rr_player>(plr));
	    if(true){ // TODO change to if Team Mode is needed
	      if(lInputSides[i] == 1){
		// Rojo
		switch(rcol){
		case 0:
		  plr->setColor(0);
		  plr->setPosition(4,24);
		  plr->setReadyPtr(&rReadyUp,0b000100);
		  break;
		case 1:
		  plr->setColor(4);
		  plr->setPosition(4,44);
		  plr->setReadyPtr(&rReadyUp,0b001000);
		  break;
		case 2:
		  plr->setColor(6);
		  plr->setPosition(4,12);
		  plr->setReadyPtr(&rReadyUp,0b010000);
		  break;
		case 3:
		  plr->setColor(8);
		  plr->setPosition(4,32);
		  plr->setReadyPtr(&rReadyUp,0b100000);
		  break;
		}
		rcol++;
		losRojos->addMember(plr);
		plr->setDirection(1);
	      }
	      else if(lInputSides[i] == 2){
		// Azul
		switch(bcol){
		case 0:
		  plr->setColor(1);
		  plr->setPosition(84,32);
		  plr->setReadyPtr(&bReadyUp,0b000100);
		  break;
		case 1:
		  plr->setColor(5);
		  plr->setPosition(84,12);
		  plr->setReadyPtr(&bReadyUp,0b001000);
		  break;
		case 2:
		  plr->setColor(7);
		  plr->setPosition(84,44);
		  plr->setReadyPtr(&bReadyUp,0b010000);
		  break;
		case 3:
		  plr->setColor(9);
		  plr->setPosition(84,24);
		  plr->setReadyPtr(&bReadyUp,0b100000);
		  break;
		}
		bcol++;
		losAzuls->addMember(plr);
		plr->setDirection(3);
	      }
	    }
	  }
	  rReadyUp = rcol - 1;
	  bReadyUp = bcol - 1;
	  addGameHud(std::shared_ptr<rr_hud>(new rr_mp_readyhud(1,&rReadyUp,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_mp_readyhud(2,&bReadyUp,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_schud(0,-2,&redScore,&lState)));
	  addGameHud(std::shared_ptr<rr_hud>(new rr_schud(1,2,&bluScore,&lState)));
	  lState = LEVELSTATE_WAITING;
	}
	// TODO add solo match option here
      }
    }
#endif
  }
  bool runSecond = (ticks % 30) == 0;

  if(lState > LEVELSTATE_CONFIG){
    for(int i=0;i<playerz.size();i++){
      if(playerz[i] != NULL)
	playerz[i]->logicTick(this);
    }
  }
  if(lState == LEVELSTATE_WAITING){
    // Check if everybody is ready, and then start the game
    int ready = 0;

    if(rReadyUp != -1) ready += (rReadyUp & 0b000011) + 1;
    if(bReadyUp != -1) ready += (bReadyUp & 0b000011) + 1;

    for(int i=0;i<4;i++){
      int b = (int) pow(2,i+2);
      if(rReadyUp != -1 && rReadyUp & b) ready--;
      if(bReadyUp != -1 && bReadyUp & b) ready--;
    }

    if(ready == 0){
      lState++;
    }
  }
  else if(runSecond && lState == LEVELSTATE_COUNTDOWN){
    countdown--;
    if(countdown == -1){
      lState++;
      onGameStart();
      leveltime = 145;
    }
  }
  else if(runSecond && (lState == LEVELSTATE_PSEUDOAUTO || lState == LEVELSTATE_TELEOP)){
    onSecond();
    if(leveltime > 0) leveltime--;
    else{
      lState = LEVELSTATE_POSTGAME;
      endGame();
      addGameHud(std::shared_ptr<rr_hud>(new rr_geschud(NULL,NULL)));
    }
  }
  if(lState == LEVELSTATE_POSTGAME){
    inHighScoreMode = -1;
    if(inHighScoreMode == 0){
      int highScore = getHighScoreQualifyingScore();
      hsinputcursor0 = 0;
      hsinputcursor1 = 6;
      if(redScore == bluScore){
	// TODO change this so both can add to high score if the tie qualifies.
	//
	// Currently just kicks it out and says screw you.
	inHighScoreMode = -1;
      }
      else if(redScore >= highScore){
#ifndef RR_ACE
	rr_input::setKbdRawEvent(true);
#endif
	inHighScoreMode = 1;
      }
      else if(bluScore >= highScore){
#ifndef RR_ACE
	rr_input::setKbdRawEvent(true);
#endif
	inHighScoreMode = 2;
      }
      else{
	inHighScoreMode = -1;
      }
    }
    else if(inHighScoreMode == 1 || inHighScoreMode == 2){
#ifdef RR_ACE
      for(int i=0;i<playerz.size();i++){
	// Only allow winning team to enter high score.
	//if(inHighScoreMode == 1 && playerz[i].getTeam() == 2) continue;
	//if(inHighScoreMode == 2 && (i == 2 || i == 3)) continue;
	if(inHighScoreMode != playerz[i]->getTeam()) continue;
	rr_event e;
	bzero(&e,sizeof(rr_event));
	if(playerz[i]->pollEvent(&e)){
	  if(e.type == START){
	    hsDisplay = new rr_highScoreDisplay();
	    sHighScoreEntry entry;
	    entry.color = inHighScoreMode;
	    strcpy(entry.name,hsinputname);
	    if(inHighScoreMode == 1) entry.score = redScore;
	    else if(inHighScoreMode == 2) entry.score = bluScore;
	    hsDisplay->insert(entry);
	    inHighScoreMode = -1;
	  }
	  else if(e.type == HSCORE_MOVEMENT){
	    if(!playerz[i]->isCaptain()){ // Orange and Purple
	      if(e.subtype == 0){
		if((hsinputname[hsinputcursor1] >= '0' && hsinputname[hsinputcursor1] < '9') ||
		   (hsinputname[hsinputcursor1] >= 'A' && hsinputname[hsinputcursor1] < 'Z'))
		  hsinputname[hsinputcursor1]++;
		else if(hsinputname[hsinputcursor1] == '9')
		  hsinputname[hsinputcursor1] = '0';
		else if(hsinputname[hsinputcursor1] == 'Z')
		  hsinputname[hsinputcursor1] = 'A';
#ifdef _rr_censor_hpp_
		for(int i=0;;i++){
		  if(banlist[i] == NULL) break;
		  if(strstr(hsinputname,banlist[i]) != 0) hsinputname[hsinputcursor1]++;
		}
		if(hsinputname[hsinputcursor1] == 'Z') // Don't think this'll be an issue
		  hsinputname[hsinputcursor1] = 'A';
#endif
	      }
	      else if(e.subtype == 2){
		if((hsinputname[hsinputcursor1] > '0' && hsinputname[hsinputcursor1] <= '9') ||
		   (hsinputname[hsinputcursor1] > 'A' && hsinputname[hsinputcursor1] <= 'Z'))
		  hsinputname[hsinputcursor1]--;
		else if(hsinputname[hsinputcursor1] == '0')
		  hsinputname[hsinputcursor1] = '9';
		else if(hsinputname[hsinputcursor1] == 'A')
		  hsinputname[hsinputcursor1] = 'Z';
#ifdef _rr_censor_hpp_
		for(int i=0;;i++){
		  if(banlist[i] == NULL) break;
		  if(strstr(hsinputname,banlist[i]) != 0) hsinputname[hsinputcursor1]--;
		}
		if(hsinputname[hsinputcursor1] == 'A')
		  hsinputname[hsinputcursor1] = 'Z';
#endif
	      }
	      else if(e.subtype == 3 && hsinputcursor1 < 7)
		hsinputcursor1++;
	      else if(e.subtype == 1 && hsinputcursor1 > 0)
		hsinputcursor1--;
	    }
	    else{ // Red and Blue
	      if(e.subtype == 0){
		if((hsinputname[hsinputcursor0] >= '0' && hsinputname[hsinputcursor0] < '9') ||
		   (hsinputname[hsinputcursor0] >= 'A' && hsinputname[hsinputcursor0] < 'Z'))
		  hsinputname[hsinputcursor0]++;
		else if(hsinputname[hsinputcursor0] == '9')
		  hsinputname[hsinputcursor0] = '0';
		else if(hsinputname[hsinputcursor0] == 'Z')
		  hsinputname[hsinputcursor0] = 'A';
#ifdef _rr_censor_hpp_
		for(int i=0;;i++){
		  if(banlist[i] == NULL) break;
		  if(strstr(hsinputname,banlist[i]) != 0) hsinputname[hsinputcursor0]++;
		}
		if(hsinputname[hsinputcursor0] == 'Z') // Don't think this'll be an issue
		  hsinputname[hsinputcursor0] = 'A';
#endif
	      }
	      else if(e.subtype == 2){
		if((hsinputname[hsinputcursor0] > '0' && hsinputname[hsinputcursor0] <= '9') ||
		   (hsinputname[hsinputcursor0] > 'A' && hsinputname[hsinputcursor0] <= 'Z'))
		  hsinputname[hsinputcursor0]--;
		else if(hsinputname[hsinputcursor0] == '0')
		  hsinputname[hsinputcursor0] = '9';
		else if(hsinputname[hsinputcursor0] == 'A')
		  hsinputname[hsinputcursor0] = 'Z';
#ifdef _rr_censor_hpp_
		for(int i=0;;i++){
		  if(banlist[i] == NULL) break;
		  if(strstr(hsinputname,banlist[i]) != 0) hsinputname[hsinputcursor0]--;
		}
		if(hsinputname[hsinputcursor0] == 'A')
		  hsinputname[hsinputcursor0] = 'Z';
#endif
	      }
	      else if(e.subtype == 3 && hsinputcursor0 < 7)
		hsinputcursor0++;
	      else if(e.subtype == 1 && hsinputcursor0 > 0)
		hsinputcursor0--;
	    }
	  }
	}
      }
#else
      SDL_Event kbde;
      while(rr_input::pollKbdRawEvent(&kbde)){
	if(kbde.key.keysym.sym == SDLK_LEFT && hsinputcursor0 > 0)
	  hsinputcursor0--;
	else if(kbde.key.keysym.sym == SDLK_BACKSPACE && hsinputcursor0 > 0){
	  if(hsinputcursor0 == 7 && hsinputname[7] != 0x20){
	    hsinputname[7] = 0x20;
	  }
	  else{
	    hsinputcursor0--;
	    hsinputname[hsinputcursor0] = 0x20;
	  }
	}
	else if(kbde.key.keysym.sym == SDLK_RIGHT && hsinputcursor0 < 7)
	  hsinputcursor0++;
	else if(kbde.key.keysym.sym == SDLK_RETURN){
	  hsDisplay = new rr_highScoreDisplay();
	  sHighScoreEntry entry;
	  entry.color = inHighScoreMode;
	  strcpy(entry.name,hsinputname);
	  if(inHighScoreMode == 1) entry.score = redScore;
	  else if(inHighScoreMode == 2) entry.score = bluScore;
	  hsDisplay->insert(entry);
	  inHighScoreMode = -1;
	  rr_input::setKbdRawEvent(false);
	}
	else if(((kbde.key.keysym.sym >= SDLK_a && kbde.key.keysym.sym <= SDLK_z) ||
	    (kbde.key.keysym.sym >= SDLK_0 && kbde.key.keysym.sym <= SDLK_9)) &&
	   hsinputcursor0 < 8){ // Basically if typing a digit or a numeral and length won't exceed the high score entry size.
	  char key = kbde.key.keysym.sym;
	  if(key >= SDLK_a && key <= SDLK_z){
	    key &= 0b01011111;
	  }
	  hsinputname[hsinputcursor0] = key;
	  if(hsinputcursor0 < 7)
	    hsinputcursor0++;
	}
      }
#endif
    }
    else if(inHighScoreMode == -1){
      for(int i=0;i<playerz.size();i++){
	rr_event ev;
	if(playerz[i]->pollEvent(&ev)){
	  if(ev.type == START){
	    lState = LEVELSTATE_DONE;
	  }
	}
      }
    }
  }
  if(lState > LEVELSTATE_CONFIG){
    ticks++;
  }
}

void rr_level::setup(){
  // Also a filler block
}

// Get what sides of a tile have a wall on them
// To ease collission detection

int getTileWallSides(unsigned char tile){
  switch(tile){
    //    01
    // 02 04 08
  case 0x01: return 0x04; break;
  case 0x02: return 0x02; break;
  case 0x03: return 0x01; break;
  case 0x04: return 0x08; break;
  case 0x05: return 0x06; break;
  case 0x06: return 0x03; break;
  case 0x07: return 0x09; break;
  case 0x08: return 0x0C; break;
  case 0x09: return 0x0E; break;
  case 0x0A: return 0x07; break;
  case 0x0B: return 0x0B; break;
  case 0x0C: return 0x0D; break;
  case 0x0D: return 0x0E; break;
  case 0x0E: return 0x0F; break;
  case 0x0F: return 0x0F; break;

  case 0x12: return 0x0A; break;
  case 0x13: return 0x05; break;
    
  case 0x15: return 0x06; break;
  case 0x16: return 0x03; break;
  case 0x17: return 0x09; break;
  case 0x18: return 0x0C; break;
  case 0x19: return 0x0E; break;
  case 0x1A: return 0x07; break;
  case 0x1B: return 0x0F; break;
  case 0x1C: return 0x0D; break;
  case 0x1D: return 0x0E; break;
  case 0x1E: return 0x0F; break;
  case 0x1F: return 0x0F; break;

  case 0x90: case 0xF0: return 0x08; break;
  case 0x91: case 0xF1: return 0x02; break;

  case 0x72: case 0x73: return 0x0F; break;
  case 0x82: case 0x83: return 0x0F; break;
  case 0x92: case 0x93: return 0x0F; break;
  case 0xA2: case 0xA3: return 0x0F; break;
  case 0xB2: case 0xB3: return 0x0F; break;
  case 0xC2: case 0xC3: return 0x0F; break;

  default: return 0; break;
  }
}

int rr_level::collissionDetect(int xp,int yp,int dir,int pn){
  int a = tileCollissionDetect(xp,yp,dir);
  int b = playerCollissionDetect(xp,yp,dir,pn);
  return a | b;
}

// If I am on (xp,yp) and attempt to move in (dir).
int rr_level::tileCollissionDetect(int xp,int yp,int dir){
  int xmgp = xp/4;
  int ymgp = yp/4;

  //   0          0x01
  // 1   3   0x02      0x08
  //   2          0x04

  // ==== Tile Collissions ====
  // If we're not centered upon a square, allow movement in any direction to realign to square.
  // Also because you'll be centered upon an axis when an adjacent block check needs to be done anyways.
  if(xp % 4 != 0 && yp % 4 != 0) return 1;
  else if(xp % 4 == 0 && yp % 4 == 0){
    if(dir == 0 && getTileWallSides(tilemap[(ymgp-1)*width+xmgp]) & 0x04) return 0;
    else if(dir == 2 && getTileWallSides(tilemap[(ymgp+1)*width+xmgp]) & 0x01) return 0;
    else if(dir == 1 && getTileWallSides(tilemap[ymgp*width+xmgp-1]) & 0x08) return 0;
    else if(dir == 3 && getTileWallSides(tilemap[ymgp*width+xmgp+1]) & 0x02) return 0;
    else return 1;
  }
  else if(xp % 4 != 0 && dir == 0 && (getTileWallSides(tilemap[(ymgp-1)*width+xmgp]) & 0x04 ||
				      getTileWallSides(tilemap[(ymgp-1)*width+xmgp+1]) & 0x04)) return 0;
  else if(xp % 4 != 0 && dir == 2 && (getTileWallSides(tilemap[(ymgp+1)*width+xmgp]) & 0x01 ||
				      getTileWallSides(tilemap[(ymgp+1)*width+xmgp+1]) & 0x04)) return 0;
  else if(yp % 4 != 0 && dir == 1 && (getTileWallSides(tilemap[ymgp*width+xmgp-1]) & 0x08 ||
				      getTileWallSides(tilemap[(ymgp+1)*width+xmgp-1]) & 0x08)) return 0;
  else if(yp % 4 != 0 && dir == 3 && (getTileWallSides(tilemap[ymgp*width+xmgp+1]) & 0x02 ||
				      getTileWallSides(tilemap[(ymgp+1)*width+xmgp+1]) & 0x02)) return 0;
  else return 1;
}

int rr_level::playerCollissionDetect(int myX,int myY,int dir,int pn){
  int chkX,chkY;
  for(int i=0;i<playerz.size();i++){
    // If playerz[i].position == xp,yp,dir, then we're assuming
    // That they're the same player
    playerz[i]->getPosition(&chkX,&chkY);
    if(chkX == myX && chkY == myY) continue;

    //   0
    // 1   3
    //   2
    if((int) abs(chkX-myX) < 4){
      if(dir == 0 && chkY == myY - 4) return 0;
      if(dir == 2 && chkY == myY + 4) return 0;
    }
    else if((int) abs(chkY-myY) < 4){
      if(dir == 1 && chkX == myX - 4) return 0;
      if(dir == 3 && chkX == myX + 4) return 0;
    }
  } // for(int i=0;i<playerz.size();i++)
  return 1;
}

bool rr_level::requestsUndeterminedInput(){
  return(lState == 1);
}

bool rr_level::onUndeterminedInput(SDL_Event* e){
#ifndef RR_ACE
  if(lState == LEVELSTATE_CONFIG){
    if(numKbdInputs < 4 && e->type == SDL_KEYDOWN){
      std::shared_ptr<rr_inputsys> input = rr_input::newKbd();
      numKbdInputs++;
      lInputs.push_back(input);
    }
    if(numJoyInputs < 4 && (e->type == SDL_JOYBUTTONDOWN ||
			    e->type == SDL_JOYAXISMOTION ||
			    e->type == SDL_JOYHATMOTION)){
      std::shared_ptr<rr_inputsys> input;
      if(e->type == SDL_JOYAXISMOTION)
	input = std::shared_ptr<rr_inputsys>(rr_input::newJoy(e->jaxis.which));
      else if(e->type == SDL_JOYHATMOTION)
	input = std::shared_ptr<rr_inputsys>(rr_input::newJoy(e->jhat.which));
      else
	input = std::shared_ptr<rr_inputsys>(rr_input::newJoy(e->jbutton.which));
      numJoyInputs++;
      lInputs.push_back(input);
    }
  }
#endif
}

void rr_level::writeLevelDataDemo(FILE* f){}

#ifdef RR_ACE
bool rr_level::doBkgSlideshow(){
  return (lState == LEVELSTATE_CONFIG);
}

void rr_level::ace_addJoystick(std::shared_ptr<rr_inputsys> j){
  lInputs.push_back(j);
}
#endif

void rr_level::render(SDL_Surface* sf){
  if(lState == LEVELSTATE_CONFIG){
#ifdef RR_ACE
    TTF_Font* iconFont = rr_ttf::load("ttf/profont.ttf",24);
    SDL_Surface* txt;
    SDL_Rect blitsrc,blitdest;
    if(sixtyfouricos == NULL)
      sixtyfouricos = IMG_Load("imgs/sixtyfourctrlrs.png");
    // 800 px wide
    // 200 px wide controller columns
    // 100 300 500 700 Center for controller columns

    // Blue: X = 32
    // Red: X = 0

    // Filled Icon Y=0
    // Greyed Icon Y=34
    // X (No need) 
    // B+A: 102
    txt = TTF_RenderText_Blended(iconFont,"Press START when ready to begin game",{255,255,255});
    blitdest.x = 400-txt->w/2;
    blitdest.y = 512-24;
    blitdest.w = txt->w;
    blitdest.h = txt->h;
    SDL_BlitSurface(txt,NULL,sf,&blitdest);
    SDL_FreeSurface(txt);

    txt = TTF_RenderText_Blended(iconFont,"Hold A+B+L+R for 3 seconds at any time to reset the game",{255,255,255});
    blitdest.x = 400-txt->w/2;
    blitdest.y = 512;
    blitdest.w = txt->w;
    blitdest.h = txt->h;
    SDL_BlitSurface(txt,NULL,sf,&blitdest);
    SDL_FreeSurface(txt);

    for(int i=0;i<4;i++){
      blitdest.w = 32;
      blitdest.h = 34;
      blitsrc.w = 32;
      blitsrc.h = 34;
      if(ace_ctrlrs_enabled & ((int) pow(2,i)) || (i < lInputs.size() && lInputs[i]->ace_hasInput()))
	blitsrc.y = 0;
      else
	blitsrc.y = 34;
      switch(i){
      case 0:
	blitsrc.x = 0;
	blitdest.x = 66;
	blitdest.y = 536;
	break;
      case 1:
	blitsrc.x = 0;
	blitdest.x = 266;
	blitdest.y = 536;	
	break;
      case 2:
	blitsrc.x = 32;
	blitdest.x = 466;
	blitdest.y = 536;
	break;
      case 3:
	blitsrc.x = 32;
	blitdest.x = 666;
	blitdest.y = 536;
	break;
      }
      SDL_BlitSurface(sixtyfouricos,&blitsrc,sf,&blitdest);
      if(i >= lInputs.size()){
	blitsrc.y = 70;
	blitsrc.h = 32;
	switch(i){
	case 0:
	  blitdest.x = 100;
	  blitdest.y = 537;
	  break;
	case 1:
	  blitdest.x = 300;
	  blitdest.y = 537;
	  break;
	case 2:
	  blitdest.x = 500;
	  blitdest.y = 537;
	  break;
	case 3:
	  blitdest.x = 700;
	  blitdest.y = 537;
	  break;
	}
	SDL_BlitSurface(sixtyfouricos,&blitsrc,sf,&blitdest);
      }
      else{
	if(ace_ctrlrs_enabled & ((int) pow(2,i)) || time(NULL) & 1)
	  blitsrc.x = 32;
	else
	  blitsrc.x = 0;
	blitsrc.y = 102;
	switch(i){
	case 0:
	  blitdest.x = 100;
	  blitdest.y = 537;
	  break;
	case 1:
	  blitdest.x = 300;
	  blitdest.y = 537;
	  break;
	case 2:
	  blitdest.x = 500;
	  blitdest.y = 537;
	  break;
	case 3:
	  blitdest.x = 700;
	  blitdest.y = 537;
	  break;
	}
	SDL_BlitSurface(sixtyfouricos,&blitsrc,sf,&blitdest);
      }
    }
#else
    TTF_Font* ltf = rr_ttf::load("ttf/profont.ttf",24);
    SDL_Surface* tx = TTF_RenderText_Blended(ltf,"Press any key to enable input",{255,255,255});
    SDL_Rect blitdest;
    blitdest.x = 400-tx->w/2;
    blitdest.y = 24;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
    tx = TTF_RenderText_Blended(ltf,"And choose a side once your controller is on the screen",{255,255,255});
    blitdest.x = 400-tx->w/2;
    blitdest.y = 48;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

    tx = TTF_RenderText_Blended(ltf,"RED",{255,0,0});
    blitdest.x = 200-tx->w/2;
    blitdest.y = 96;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

    tx = TTF_RenderText_Blended(ltf,"BLUE",{0,0,255});
    blitdest.x = 600-tx->w/2;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

    if(lInputs.size() > 0){
      for(int i=0;i<lInputs.size();i++){
	tx = lInputs[i]->get_ico();
	//tx = NULL;
	if(tx != NULL){
	  blitdest.w = tx->w;
	  blitdest.h = tx->h;
	  blitdest.x = 400-tx->w/2;
	  blitdest.y = 128+60*i;
	  if(lInputSides[i] == 1) blitdest.x -= 200;
	  if(lInputSides[i] == 2) blitdest.x += 200;
	  SDL_BlitSurface(tx,NULL,sf,&blitdest);
	}
      }
    }
#endif
  }
  else if(lState > LEVELSTATE_CONFIG){
    if(tiles == NULL){
      tiles = IMG_Load("imgs/maptiles.png");
    }
    // Tiles are 16x16px
    // ------------------
    // Starting Position on pixel grid to start drawing map tiles
    // screen_width/2 - (width(tiles) * 16) / 2
    int mapxsp = sf->w/2 - width * 8;
    int mapysp = sf->h/2 - height * 8;
    
    // Drawing of tiles
    SDL_Rect blitsrc,blitdest;
    blitsrc.w = 16;
    blitsrc.h = 16;
    blitdest.w = 16;
    blitdest.h = 16;
    for(int y = 0;y<height;y++){
      for(int x = 0;x<width;x++){
	blitsrc.x = (tilemap[y*width+x] & 0x0F)*16;
	blitsrc.y = ((tilemap[y*width+x] & 0xF0)>>4)*16;
	blitdest.x = x*16+mapxsp;
	blitdest.y = y*16+mapysp;
	SDL_BlitSurface(tiles,&blitsrc,sf,&blitdest);
      }
    }
    if(entitiez.size() != 0){
      for(int i=0;i<entitiez.size();i++){
	if(entitiez[i] != NULL)
	  entitiez[i]->render(sf,mapxsp,mapysp);
      }
    }
    if(playerz.size() != 0){
      for(int i=0;i<playerz.size();i++){
	playerz[i]->render(sf,mapxsp,mapysp);
      }
    }
    if(hudz.size() != 0){
      for(int i=0;i<hudz.size();i++){
	hudz[i]->render(sf);
      }
    }
    if(lState == LEVELSTATE_POSTGAME){
      if(hsDisplay != NULL)
	hsDisplay->render(sf);
      if(inHighScoreMode > 0){
	TTF_Font* ft = rr_ttf::load("ttf/profont.ttf",48);
	SDL_Color c,calt;
	switch(inHighScoreMode){
	case 1:
	  c = {255,0,0};
	  calt = {255,128,0};
	  break;
	case 2:
	  c = {0,0,255};
	  calt = {128,0,128};
	  break;
	default:
	  c = {128,128,128};
	  calt = {192,192,192};
	  break;
	}
	SDL_Surface* txt;
	SDL_Rect blitdest;
	if(ticks % 60 < 30){
	  // Blinking Colourized High Score
	  txt = TTF_RenderText_Blended(ft,"HIGH SCORE!",c);
	  blitdest.x = 400-txt->w/2;
	  blitdest.y = 48;
	  blitdest.w = txt->w;
	  blitdest.h = txt->h;
	  SDL_BlitSurface(txt,NULL,sf,&blitdest);
	  SDL_FreeSurface(txt);
	}
	txt = TTF_RenderText_Blended(ft,hsinputname,c);
	blitdest.x = 400-txt->w/2;
	blitdest.y = 96;
	blitdest.w = txt->w;
	blitdest.h = txt->h;
	SDL_BlitSurface(txt,NULL,sf,&blitdest);
	SDL_FreeSurface(txt);
	
	txt = TTF_RenderText_Blended(ft,"^",c);
	blitdest.w = txt->w;
	blitdest.h = txt->h;
	blitdest.y = 132;
	blitdest.x = (400-txt->w*4)+(txt->w*hsinputcursor0);
	SDL_BlitSurface(txt,NULL,sf,&blitdest);
	SDL_FreeSurface(txt);
#ifdef RR_ACE
	// Somebody please call Red Forman. I need to stop making mistakes like this.
	txt = TTF_RenderText_Blended(ft,"^",calt);
	blitdest.x = (400-txt->w*4)+(txt->w*hsinputcursor1);
	SDL_BlitSurface(txt,NULL,sf,&blitdest);
	SDL_FreeSurface(txt);
#endif
      }
      else{
	TTF_Font* ft = rr_ttf::load("ttf/profont.ttf",24);
	SDL_Surface* txt = TTF_RenderText_Blended(ft,"Game is done, press START or ENTER to exit",{255,255,255});
	SDL_Rect blitdest;
	blitdest.x = 400-txt->w/2;
	blitdest.y = 102;
	blitdest.w = txt->w;
	blitdest.h = txt->h;
	SDL_BlitSurface(txt,NULL,sf,&blitdest);
	SDL_FreeSurface(txt);
      }
    }
  }
}

int rr_level::itemSpriteIndex(){
  return 0;
}
