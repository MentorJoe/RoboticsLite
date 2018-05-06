#include<stdlib.h>
#include<time.h>
#include<memory>
#include"rr-sdlinc.h"
#include"rr-stdinc.h"

// Macro to get the one-dimentional coordinate given x, y, and the width
#define lvcordw(x,y,w)(y*w+x)

SDL_Surface* redPowerupHud = NULL;
SDL_Surface* bluPowerupHud = NULL;

// TODO remove playerconfig from this constructor
rr_level_powerup::rr_level_powerup(int playerconfig):rr_level(){
#ifndef RR_ACE
  rr_input::clearInputs();
#endif
  // Seed the RNG and generate the random seed for the platform randomization
  srand(time(NULL));
  leveldata = rand(); // Moved up here so it can be wrote to a demo file.
  // Set the various variables
  width = 23; // The Map Width (used in rendering)
  height = 15; // The Map Height
  RSwitchSway = 0;
  ScaleSway = 0;
  BSwitchSway = 0;
  RInVault = 0;
  BTPortal = 7;
  BBPortal = 7;
  BInVault = 0;
  RTPortal = 7;
  RBPortal = 7;
  lState = 1; // see the #defines at the top of rr_level.h
  powsInQueue = 0;
  powCountdown = -1;
  levitatePowerupUsed = 0;
  bzero(&rpow,sizeof(rr_powerup_set)); // Empty the vault structs.
  bzero(&bpow,sizeof(rr_powerup_set)); // Should be replaced to memset(...)
  tilemap = new char[width*height];
  //  ^^ The actual board tiles, dynamic memory allocation
  // You need to be careful with this, because if you don't deallocate this
  // when you're done (with `delete $(VARIABLE)`), it'll keep the string in
  // memory wasting space and unable to be removed until the program ends,
  // a Ctrl-Alt-Delete on DOS, an Alt-SysRQ-B on Linux, the universe explodes,
  // or other things of the sort.
  //  vv Additional data
  tilesubdata = new char[width*height];
  bzero(tilemap,sizeof(char)*width*height); // Empty out moar variables
  bzero(tilesubdata,sizeof(char)*width*height);
  bzero(&activePowerup,sizeof(rr_powerup_pow));
  bzero(&powerupInQueue,sizeof(rr_powerup_pow));
  // Draw the border, then lay all the extra tiles
  for(int y=0;y<height;y++){
    for(int x=0;x<width;x++){
      if(y == 0){
	if(x == 0 || x == width-1) continue;
	else tilemap[y*width+x] = 0x01; // I don't think I need the `else`s here, but just for clarity
      }
      else if(y == height-1){
	if(x == 0 || x == width-1) continue;
	else tilemap[y*width+x] = 0x03;
      }
      else if(x == 0) tilemap[y*width+x] = 0x04;
      else if(x == width-1) tilemap[y*width+x] = 0x02;
      else tilemap[y*width+x] = 0x00;
    }
  }
  // ==== Extra Tiles ====
  // Portals
  tilemap[lvcordw(0,1,23)] = 0xF0; // Set the tile at (0,1) to 0xF0 (15 tiles Y, 0 tiles X) (See tile image sheet)
  tilemap[lvcordw(1,1,23)] = 0xD0;
  tilemap[lvcordw(2,1,23)] = 0xD1;
  tilemap[lvcordw(0,13,23)] = 0xF0;
  tilemap[lvcordw(1,13,23)] = 0xE0;
  tilemap[lvcordw(2,13,23)] = 0xE1;
  tilemap[lvcordw(20,1,23)] = 0xB0;
  tilemap[lvcordw(21,1,23)] = 0xB1;
  tilemap[lvcordw(22,1,23)] = 0xF1;
  tilemap[lvcordw(20,13,23)] = 0xC0;
  tilemap[lvcordw(21,13,23)] = 0xC1;
  tilemap[lvcordw(22,13,23)] = 0xF1;

  // Exchanges
  tilemap[lvcordw(0,7,23)] = 0x90;
  tilemap[lvcordw(1,7,23)] = 0xA0;
  tilemap[lvcordw(21,7,23)] = 0xA1;
  tilemap[lvcordw(22,7,23)] = 0x91;

  // Red-side Switch
  tilemap[lvcordw(6,4,23)] = 0x81;// Platform
  tilemap[lvcordw(6,5,23)] = 0x1B;
  tilemap[lvcordw(6,6,23)] = 0x12;
  tilemap[lvcordw(6,7,23)] = 0x12;
  tilemap[lvcordw(6,8,23)] = 0x12;
  tilemap[lvcordw(6,9,23)] = 0x19;
  tilemap[lvcordw(6,10,23)] = 0x81;// Platform

  // Blue-side Switch
  tilemap[lvcordw(16,4,23)] = 0x81;// Platform
  tilemap[lvcordw(16,5,23)] = 0x1B;
  tilemap[lvcordw(16,6,23)] = 0x12;
  tilemap[lvcordw(16,7,23)] = 0x12;
  tilemap[lvcordw(16,8,23)] = 0x12;
  tilemap[lvcordw(16,9,23)] = 0x19;
  tilemap[lvcordw(16,10,23)] = 0x81;// Platform

  // Scale
  tilemap[lvcordw(11,3,23)] = 0x81; // Platform
  tilemap[lvcordw(11,4,23)] = 0x1B;
  tilemap[lvcordw(11,5,23)] = 0x12;
  tilemap[lvcordw(11,6,23)] = 0x12;
  tilemap[lvcordw(11,7,23)] = 0x12;
  tilemap[lvcordw(11,8,23)] = 0x12;
  tilemap[lvcordw(11,9,23)] = 0x12;
  tilemap[lvcordw(11,10,23)] = 0x19;
  tilemap[lvcordw(11,11,23)] = 0x81; // Platform
  tilemap[lvcordw(10,5,23)] = 0x43;
  tilemap[lvcordw(9,5,23)] = 0x42;
  tilemap[lvcordw(9,6,23)] = 0x52;
  tilemap[lvcordw(9,7,23)] = 0x52;
  tilemap[lvcordw(9,8,23)] = 0x52;
  tilemap[lvcordw(9,9,23)] = 0x62;
  tilemap[lvcordw(10,9,23)] = 0x63;
  tilemap[lvcordw(12,5,23)] = 0x44;
  tilemap[lvcordw(13,5,23)] = 0x45;
  tilemap[lvcordw(13,6,23)] = 0x55;
  tilemap[lvcordw(13,7,23)] = 0x55;
  tilemap[lvcordw(13,8,23)] = 0x55;
  tilemap[lvcordw(13,9,23)] = 0x65;
  tilemap[lvcordw(12,9,23)] = 0x64;

  // Null Zone Top
  tilemap[lvcordw(10,3,23)] = 0xF3;
  tilemap[lvcordw(9,3,23)] = 0xF2;
  tilemap[lvcordw(9,2,23)] = 0xE2;
  tilemap[lvcordw(9,1,23)] = 0xE2;
  tilemap[lvcordw(12,3,23)] = 0xF3;
  tilemap[lvcordw(13,3,23)] = 0xF4;
  tilemap[lvcordw(13,2,23)] = 0xE4;
  tilemap[lvcordw(13,1,23)] = 0xE4;

  // Null Zone Bottom
  tilemap[lvcordw(10,11,23)] = 0xD3;
  tilemap[lvcordw(9,11,23)] = 0xD2;
  tilemap[lvcordw(9,12,23)] = 0xE2;
  tilemap[lvcordw(9,13,23)] = 0xE2;
  tilemap[lvcordw(12,11,23)] = 0xD3;
  tilemap[lvcordw(13,11,23)] = 0xD4;
  tilemap[lvcordw(13,12,23)] = 0xE4;
  tilemap[lvcordw(13,13,23)] = 0xE4;

  // Block piles at Y=7 X=5 && 17
  tilemap[lvcordw(5,7,23)] = 0x61;
  tilemap[lvcordw(17,7,23)] = 0x60;

  // Individual blocks (Scale-side of Switch)
  // Done as entitites instead of tiles
  int ytp = 18;
  for(int i=0;i<6;i++){ // `i` is just for an iterator counter
    std::shared_ptr<rr_entity> blockr = std::shared_ptr<rr_entity>(new rr_entity(NULL,2));
    std::shared_ptr<rr_entity> blockb = std::shared_ptr<rr_entity>(new rr_entity(NULL,2));
    blockr->setPosition(28,ytp);
    blockb->setPosition(60,ytp);
    entitiez.push_back(std::shared_ptr<rr_entity>(blockr));
    entitiez.push_back(std::shared_ptr<rr_entity>(blockb));
    ytp += 4;
  }

  // ==== Sway Huds ====
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,320,172,&RSwitchSway,true)));
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,400,172,&ScaleSway,true)));
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,480,172,&BSwitchSway,true)));

  // ==== Portal Huds ====
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,560,172,&RTPortal,false)));
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,560,412,&RBPortal,false)));
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,240,172,&BTPortal,false)));
  addGameHud(std::shared_ptr<rr_hud>(new rr_counthud(1,16,240,412,&BBPortal,false)));

  // ==== Powerup Huds ====
  addGameHud(std::shared_ptr<rr_hud>(new powerupHud(&rpow,1,&RInVault)));
  addGameHud(std::shared_ptr<rr_hud>(new powerupHud(&bpow,2,&BInVault)));
}

rr_level_powerup::~rr_level_powerup(){
  // I need to do some looking how the compliler decides which deconstructor is used.
  // Just since the rr_level deconstructor deallocates the tilemap and tilesubdata.
}

void rr_level_powerup::onGameStart(){
  // Assign scale / switch colors
  if(leveldata & 0x01){
    tilemap[lvcordw(6,4,23)] = 0x70;
    tilemap[lvcordw(6,10,23)] = 0x71;
  }
  else{
    tilemap[lvcordw(6,4,23)] = 0x71;
    tilemap[lvcordw(6,10,23)] = 0x70;
  }
  if(leveldata & 0x02){
    tilemap[lvcordw(11,3,23)] = 0x70;
    tilemap[lvcordw(11,11,23)] = 0x71;
  }
  else{
    tilemap[lvcordw(11,3,23)] = 0x71;
    tilemap[lvcordw(11,11,23)] = 0x70;
  }
  if(leveldata & 0x04){
    tilemap[lvcordw(16,4,23)] = 0x70;
    tilemap[lvcordw(16,10,23)] = 0x71;
  }
  else{
    tilemap[lvcordw(16,4,23)] = 0x71;
    tilemap[lvcordw(16,10,23)] = 0x70;
  }
}

void rr_level_powerup::onSecond(){
  for(int i=0;i<playerz.size();i++){
    if(playerz[i]->penaltyLocationTimer.getTicks() > 1500){
      switch(playerz[i]->getTeam()){
      case 1:
	bluScore++;
	break;
      case 2:
	redScore++;
	break;
      }
    }
  }
  // Scoring
  if(activePowerup.team == 0){
    // No Active Powerup. Roll as normal
    if(RSwitchSway > 0) redScore++;
    if(BSwitchSway < 0) bluScore++;
    if(ScaleSway > 0) redScore++;
    else if(ScaleSway < 0) bluScore++;
  }
  if(activePowerup.pow == FORCE){
    if(activePowerup.team == 1){
      if(activePowerup.level & 1){
	// Completely bypass switch logic and score it to the team
	redScore++;
      }
      if(activePowerup.level & 2){
	// Completely bypass scale logic and score it to the team
	redScore++;
      }
      // Blue Switch Scoring as normal
      if(BSwitchSway < 0) bluScore++;
    }
    else if(activePowerup.team == 2){
      if(activePowerup.level & 1){
	bluScore++;
      }
      if(activePowerup.level & 2){
	bluScore++;
      }
      if(RSwitchSway > 0) redScore++;
    }
  }
  else if(activePowerup.pow == BOOST){
    if(activePowerup.team == 1){
      if((activePowerup.level & 1) && (RSwitchSway > 0)) redScore+=2;
      if((activePowerup.level & 2) && (ScaleSway > 0)) redScore+=2;
      else if(ScaleSway < 0) bluScore++;
    }
    else if(activePowerup.team == 2){
      if((activePowerup.level & 1) && (BSwitchSway < 0)) bluScore+=2;
      if((activePowerup.level & 2) && (BSwitchSway < 0)) bluScore+=2;
      else if(ScaleSway > 0) redScore++;
    }
  }
  if(powCountdown <= 0){
    // If the powerup has expired, then
    // Say the team that initialized it can queue a powerup again
    powsInQueue &= ~activePowerup.team;
    // Remove it from play
    activePowerup.team = 0;

    if(powerupInQueue.team != 0){
      powCountdown = 10;
      bcopy(&powerupInQueue,&activePowerup,sizeof(rr_powerup_pow));
      bzero(&powerupInQueue,sizeof(rr_powerup_pow));
    }
  }
  else{
    powCountdown--;
  }
}

void rr_level_powerup::render(SDL_Surface* sf){
  rr_level::render(sf); // Call the level renderer, render the players and entities
  if(lState > LEVELSTATE_CONFIG){
    // Level-specific rendering
    SDL_Rect blitdest;
    SDL_Color colorz[] = {
      {128,128,128},
      {255,0,0},
      {0,0,255}
    };
    TTF_Font* ft = rr_ttf::load("ttf/djvm.ttf",16);
    char* barf = new char[512];
    SDL_Surface* txt;
    if(lState < LEVELSTATE_POSTGAME && activePowerup.team > 0){
      // If the game is still active (between LEVELSTATE_CONFIG and LEVELSTATE_POSTGAME)
      // And there's an active powerup
      // Then render the powerup information.
      switch(activePowerup.pow){
      case FORCE:
	sprintf(barf,"Active powerup: FORCE (%d) [%2d]",activePowerup.level,powCountdown);
	break;
      case BOOST:
	sprintf(barf,"Active powerup: BOOST (%d) [%2d]",activePowerup.level,powCountdown);
	break;
      default:
	sprintf(barf,"Well, this shouldn't be here.");
	break;
      }
      txt = TTF_RenderText_Blended(ft,barf,colorz[activePowerup.team]);
      blitdest.x = 400-txt->w/2;
      blitdest.y = 64;
      blitdest.w = txt->w;
      blitdest.h = txt->h;
      SDL_BlitSurface(txt,NULL,sf,&blitdest);
      SDL_FreeSurface(txt);
    }
    // I think I also need to do some more clean up stuff here.
    else if(lState < LEVELSTATE_POSTGAME){
      txt = TTF_RenderText_Blended(ft,"No Active Powerup",colorz[0]);
      blitdest.x = 400-txt->w/2;
      blitdest.y = 64;
      blitdest.w = txt->w;
      blitdest.h = txt->h;
      SDL_BlitSurface(txt,NULL,sf,&blitdest);
      SDL_FreeSurface(txt);
    }
    if(lState < LEVELSTATE_POSTGAME && powerupInQueue.team > 0){
      switch(powerupInQueue.pow){
      case FORCE:
	sprintf(barf,"Powerup in queue: FORCE (%d)",powerupInQueue.level);
	break;
      case BOOST:
	sprintf(barf,"Powerup in queue: BOOST (%d)",powerupInQueue.level);
	break;
      }
      txt = TTF_RenderText_Blended(ft,barf,colorz[powerupInQueue.team]);
      blitdest.x = 400-txt->w/2;
      blitdest.y = 96;
      blitdest.w = txt->w;
      blitdest.h = txt->h;
      SDL_BlitSurface(txt,NULL,sf,&blitdest);
      SDL_FreeSurface(txt);
    }
    delete barf;
  }
}

void rr_level_powerup::logicTick(){
  rr_level::logicTick(false);
  int px,py,dir,ex,ey;
  // TODO cleanup and make these look more natural
  for(int i=0;i<playerz.size();i++){
    playerz[i]->getPosition(&px,&py);
    playerz[i]->getDirection(&dir);
    for(int e=0;e<entitiez.size();e++){
      if(entitiez[e] != NULL){
	entitiez[e]->getPosition(&ex,&ey);
	if(ex == px && ey == py && entitiez[e]->getType() == 2 && playerz[i]->setHasItem(true)){
	  entitiez[e] = std::shared_ptr<rr_entity>(NULL);
	}
      }
    }
    //               04
    //               12
    // Null Zones: 36  52
    //               44
    //               52
    if(leveldata & 2){
      // Red Top Blue Bottom
      if(playerz[i]->getTeam() == 1){
	// Red Team
	if(px >= 36 && px <= 52 && py >= 44 && py <= 52){
	  if(!(playerz[i]->penaltyLocationTimer.esStarted())){
	    playerz[i]->penaltyLocationTimer.start();
	  }
	}
	else{
	  if(playerz[i]->penaltyLocationTimer.esStarted()){
	    playerz[i]->penaltyLocationTimer.stop();
	  }
	}
      }
      else if(playerz[i]->getTeam() == 2){
	// Blue Team
	if(px >= 36 && px <= 52 && py >= 04 && py <= 12){
	  if(!(playerz[i]->penaltyLocationTimer.esStarted())){
	    playerz[i]->penaltyLocationTimer.start();
	  }
	}
	else{
	  if(playerz[i]->penaltyLocationTimer.esStarted()){
	    playerz[i]->penaltyLocationTimer.stop();
	  }
	}
      }
    }
    else{
      // Blue Top Red Bottom
      if(playerz[i]->getTeam() == 1){
	// Red Team
	if(px >= 36 && px <= 52 && py >= 04 && py <= 12){
	  if(!(playerz[i]->penaltyLocationTimer.esStarted())){
	    playerz[i]->penaltyLocationTimer.start();
	  }
	}
	else{
	  if(playerz[i]->penaltyLocationTimer.esStarted()){
	    playerz[i]->penaltyLocationTimer.stop();
	  }
	}
      }
      else if(playerz[i]->getTeam() == 2){
	// Blue Team
	if(px >= 36 && px <= 52 && py >= 44 && py <= 52){
	  if(!(playerz[i]->penaltyLocationTimer.esStarted())){
	    playerz[i]->penaltyLocationTimer.start();
	  }
	}
	else{
	  if(playerz[i]->penaltyLocationTimer.esStarted()){
	    playerz[i]->penaltyLocationTimer.stop();
	  }
	}
      }
    }
    if(py == 7*4 && px == 5*4){
      // Cube Pyramid (Red)
      // The if statement being > 0x12 is because reasons. I dunno.
      // Look at the tilesheet if you have questions.
      if(!playerz[i]->getHasItem() && tilemap[lvcordw(5,7,23)] > 0x12){
	playerz[i]->setHasItem(true);
	if(playerz[i]->getTeam() == 2){
	  // Penalize if Blue
	  redScore += 20;
	}
	tilemap[lvcordw(5,7,23)] -= 0x10;
      }
    }
    else if(py == 7*4 && px == 17*4){
      // Cube Pyramid (Blue)
      if(!playerz[i]->getHasItem() && tilemap[lvcordw(17,7,23)] > 0x12){
	if(playerz[i]->getTeam() == 1){
	  // Penalize if Red
	  bluScore += 20;
	}
	playerz[i]->setHasItem(true);
	tilemap[lvcordw(17,7,23)] -= 0x10;
      }
    }
    else if(px == 11*4 && py == 3*4){
      // Top scale
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(11,3,23)] == 0x70) ScaleSway++;
	else if(tilemap[lvcordw(11,3,23)] == 0x71) ScaleSway--;
      }
    }
    else if(px == 11*4 && py == 11*4){
      // Bottom scale
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(11,11,23)] == 0x70) ScaleSway++;
	else if(tilemap[lvcordw(11,11,23)] == 0x71) ScaleSway--;
      }
    }
    else if(px == 6*4 && py == 4*4){
      // Red Switch
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(6,4,23)] == 0x70) RSwitchSway++;
	else if(tilemap[lvcordw(6,4,23)] == 0x71) RSwitchSway--;
      }
    }
    else if(px == 6*4 && py == 10*4){
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(6,10,23)] == 0x70) RSwitchSway++;
	else if(tilemap[lvcordw(6,10,23)] == 0x71) RSwitchSway--;
      }
    }
    // Blue Switch
    else if(px == 16*4 && py == 4*4){
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(16,4,23)] == 0x70) BSwitchSway++;
	else if(tilemap[lvcordw(16,4,23)] == 0x71) BSwitchSway--;
      }
    }
    else if(px == 16*4 && py == 10*4){
      if(playerz[i]->setHasItem(false)){
	if(tilemap[lvcordw(16,10,23)] == 0x70) BSwitchSway++;
	else if(tilemap[lvcordw(16,10,23)] == 0x71) BSwitchSway--;
      }
    }
    // Exchanges
    else if(px == 4 && py == 28 && dir == 3){
      if(playerz[i]->setHasItem(false)){
	RInVault++;
      }
    }
    else if(px == 4*21 && py == 28 && dir == 1){
      if(playerz[i]->setHasItem(false)){
	BInVault++;
      }
    }
    rr_event re;
    bzero(&re,sizeof(rr_event)); // rr2018.01 fix
    if(playerz[i]->pollEvent(&re)){
      if(re.type == ETAUNT){
	playerz[i]->taunt();
      }
      if(re.type == DROP_CUBE){
	if(playerz[i]->getHasItem()){
	  bool placeblock = false; // Any OOG Fans here? PLACE BLOCKS!!!! PLACEM!
	  std::shared_ptr<rr_entity> block = std::shared_ptr<rr_entity>(new rr_entity(NULL,2));
	  switch(dir){
	  case 0:
	    if(py >= 8){
	      block->setPosition(px,py-4);
	      placeblock = true;
	    }
	    break;
	  case 1:
	    if(px <= 80){
	      block->setPosition(px+4,py);
	      placeblock = true;
	    }
	    break;
	  case 2:
	    if(py <= 48){
	      block->setPosition(px,py+4);
	      placeblock = true;
	    }
	    break;
	  case 3:
	    if(px >= 8){
	      block->setPosition(px-4,py);
	      placeblock = true;
	    }
	    break;
	  }
	  if(placeblock){
	    addEntity(block);
	    playerz[i]->setHasItem(false);
	  }
	}
	else{
	  if(playerz[i]->getTeam() == 1){
	    // Red taking blocks out of the portal
	    if(px == 84 && py == 4 && dir == 1 && RTPortal > 0 && playerz[i]->setHasItem(true)){
	      RTPortal--;
	    }
	    else if(px == 84 && py == 52 && dir == 1 && RBPortal > 0 && playerz[i]->setHasItem(true)){
	      RBPortal--;
	    }
	  }
	  else if(playerz[i]->getTeam() == 2){
	    // Blue taking blocks out of the portal
	    if(px == 4 && py == 4 && dir == 3 && BTPortal > 0 && playerz[i]->setHasItem(true)){
	      BTPortal--;
	    }
	    else if(px == 4 && py == 52 && dir == 3 && BBPortal > 0 && playerz[i]->setHasItem(true)){
	      BBPortal--;
	    }
	  }
	}
      }
      else if(playerz[i]->isCaptain() && re.type == CUE_POWERUP){
	if(*(playerz[i]->getInptr()) & 0x10){
	  if(playerz[i]->getTeam() == 1 && !powInQueue(1)){
	    // Red Team Cue Powerup
	    rr_powerup_pow pow;
	    pow.team = 1;
	    switch(re.subtype){
	    case 1:
	      pow.pow = FORCE;
	      pow.level = rpow.force;
	      pow.lvsetr = &(rpow.force);
	      break;
	    case 2:
	      pow.pow = LEVITATE;
	      pow.level = rpow.levitate;
	      pow.lvsetr = &(rpow.levitate);
	      break;
	    case 3:
	      pow.pow = BOOST;
	      pow.level = rpow.boost;
	      pow.lvsetr = &(rpow.boost);
	      break;
	    default:
	      pow.level = 7;
	      // I was going to set this to 9001, to force the game to not cue
	      // the powerup, but then I remembered unsigned char, and it really
	      // doesn't have to be unsigned, and really it could just be a
	      // shared integer, and I'm just rambling at this point...
	      break;
	    }
	    if(pow.pow == LEVITATE && pow.level == 3){
	      // Activate levitate
	      levitatePowerupUsed |= 0x01;
	      *(pow.lvsetr) = 4;
	    }
	    else if(pow.pow != LEVITATE && pow.level < 4){
	      // Powerup hasn't been used, put it in the queue.
	      if(cuePowerup(&pow)){
		*(pow.lvsetr) = 4;
		powsInQueue |= 0x01;
	      }
	    }
	  }
	  else if(playerz[i]->getTeam() == 2 && !powInQueue(2)){
	    // Blue Team Cue Powerup
	    // Copy-Paste and edit
	    rr_powerup_pow pow;
	    pow.team = 2;
	    switch(re.subtype){
	    case 1:
	      pow.pow = FORCE;
	      pow.level = bpow.force;
	      pow.lvsetr = &(bpow.force);
	      break;
	    case 2:
	      pow.pow = LEVITATE;
	      pow.level = bpow.levitate;
	      pow.lvsetr = &(bpow.levitate);
	      break;
	    case 3:
	      pow.pow = BOOST;
	      pow.level = bpow.boost;
	      pow.lvsetr = &(bpow.boost);
	      break;
	    default:
	      pow.level = 7;
	      break;
	    }
	    if(pow.pow == LEVITATE && pow.level == 3){
	      levitatePowerupUsed |= 0x02;
	      *(pow.lvsetr) = 4;
	    }
	    if(pow.pow != LEVITATE && pow.level < 4){
	      if(cuePowerup(&pow)){
		*(pow.lvsetr) = 4;
		powsInQueue |= 0x02;
	      }
	    }
	  }
	}
	else{
	  if(playerz[i]->getTeam() == 1){
	    // Red Team Cube to Vault
	    if(RInVault > 0){
	      switch(re.subtype){
	      case 1:
		if(rpow.force < 3){
		  rpow.force++;
		  RInVault--;
		  redScore += 10;
		}
		break;
	      case 2:
		if(rpow.levitate < 3){
		  rpow.levitate++;
		  RInVault--;
		  redScore += 10;
		}
		break;
	      case 3:
		if(rpow.boost < 3){
		  rpow.boost++;
		  RInVault--;
		  redScore += 10;
		}
		break;
	      }
	    }
	  }
	  else if(playerz[i]->getTeam() == 2){
	    // Blue Team Cube to Vault
	    if(BInVault > 0){
	      switch(re.subtype){
	      case 1:
		if(bpow.force < 3){
		  bpow.force++;
		  BInVault--;
		  bluScore += 10;
		}
		break;
	      case 2:
		if(bpow.levitate < 3){
		  bpow.levitate++;
		  BInVault--;
		  bluScore += 10;
		}
		break;
	      case 3:
		if(bpow.boost < 3){
		  bpow.boost++;
		  BInVault--;
		  bluScore += 10;
		}
		break;
	      }
	    }
	  }
	}
      }
    } // if(playerz[i]->pollEvent(&re))
  } // for(playerz[i])
}

int rr_level_powerup::itemSpriteIndex(){
  return 1;
}

void rr_level_powerup::endGame(){
  // Scoring for "climbing".
  int xp,yp;
  for(int i=0;i<playerz.size();i++){
    playerz[i]->getPosition(&xp,&yp);
    if(playerz[i]->getTeam() == 1){
      if(xp >= 36 && xp <= 40 && yp >= 20 && yp <= 36){
	redScore += 50;
      }
    }
    else if(playerz[i]->getTeam() == 2){
      if(xp >= 48 && xp <= 52 && yp >= 20 && yp <= 36){
	bluScore += 50;
      }
    }
  }
  if(levitatePowerupUsed & 0x01) redScore += 50;
  if(levitatePowerupUsed & 0x02) bluScore += 50;
}

bool rr_level_powerup::powInQueue(int team){
  switch(team){
  case 1:
    return (bool) (powsInQueue & 0x01);
    break;
  case 2:
    return (bool) (powsInQueue & 0x02);
    break;
  default:
    return false;
    break;
  }
}

bool rr_level_powerup::cuePowerup(rr_powerup_pow* p){
  if(powInQueue(p->team)) return false;
  if(activePowerup.team == 0){
    activePowerup = *p;
    powCountdown = 10;
    return true;
  }
  else if(powerupInQueue.team == 0){
    powerupInQueue = *p;
    return true;
  }
  else return false;
}

powerupHud::powerupHud(rr_powerup_set* val,int s,int* c){
  pset = val;
  side = s;
  cubecount = c;
}

void powerupHud::render(SDL_Surface* sf){
  TTF_Font* ft = rr_ttf::load("ttf/profont.ttf",36);
  char* barf = new char[32];
  SDL_Rect blitsrc,blitdest;
  SDL_Surface* txt;
  if(side == 1){
    // Red Side
    if(redPowerupHud == NULL) redPowerupHud = IMG_Load("imgs/powerupHudRed.png");
    blitsrc.x = 4;
    blitsrc.y = 4;
    blitsrc.w = 24;
    blitsrc.h = 10;
    blitdest.x = 32;
    blitdest.y = 400;
    blitdest.w = 24;
    blitdest.h = 10;
    SDL_BlitSurface(redPowerupHud,&blitsrc,sf,&blitdest);
    for(int i=0;i<3;i++){
      unsigned char n;
      switch(i){
      case 0: n = pset->force;    break;
      case 1: n = pset->levitate; break;
      case 2: n = pset->boost;    break;
      }
      blitsrc.w = 8;
      blitsrc.h = 10;
      blitdest.w = 8;
      blitdest.h = 10;
      switch(n){
      case 0: blitsrc.x = 4; blitsrc.y = 14; break;
      case 1: blitsrc.x = 12; blitsrc.y = 14; break;
      case 2: blitsrc.x = 20; blitsrc.y = 14; break;
      case 3: blitsrc.x = 4; blitsrc.y = 24; break;
      case 4: blitsrc.x = 12; blitsrc.y = 24; break;
      case 5: blitsrc.x = 20; blitsrc.y = 24; break;
      }
      blitdest.y = 410;
      blitdest.x = 8*i+32;
      SDL_BlitSurface(redPowerupHud,&blitsrc,sf,&blitdest);
    }
    sprintf(barf,"%d",*cubecount);
    txt = TTF_RenderText_Blended(ft,barf,{255,0,0});
    blitdest.w = txt->w;
    blitdest.h = txt->h;
    blitdest.y = 400 - txt->h;
    blitdest.x = 32-12+txt->w/2;
    SDL_BlitSurface(txt,NULL,sf,&blitdest);
    SDL_FreeSurface(txt);
  }
  else if(side == 2){
    // Blue Side
    if(bluPowerupHud == NULL) bluPowerupHud = IMG_Load("imgs/powerupHudBlue.png");
    blitsrc.x = 4;
    blitsrc.y = 4;
    blitsrc.w = 24;
    blitsrc.h = 10;
    blitdest.x = 800-32-24;
    blitdest.y = 400;
    blitdest.w = 24;
    blitdest.h = 10;
    SDL_BlitSurface(bluPowerupHud,&blitsrc,sf,&blitdest);
    for(int i=0;i<3;i++){
      unsigned char n;
      switch(i){
      case 0: n = pset->force;    break;
      case 1: n = pset->levitate; break;
      case 2: n = pset->boost;    break;
      }
      blitsrc.w = 8;
      blitsrc.h = 10;
      blitdest.w = 8;
      blitdest.h = 10;
      switch(n){
      case 0: blitsrc.x = 4; blitsrc.y = 14; break;
      case 1: blitsrc.x = 12; blitsrc.y = 14; break;
      case 2: blitsrc.x = 20; blitsrc.y = 14; break;
      case 3: blitsrc.x = 4; blitsrc.y = 24; break;
      case 4: blitsrc.x = 12; blitsrc.y = 24; break;
      case 5: blitsrc.x = 20; blitsrc.y = 24; break;
      }
      blitdest.y = 410;
      blitdest.x = 800-32-24+(8*i);
      SDL_BlitSurface(redPowerupHud,&blitsrc,sf,&blitdest);
    }
    sprintf(barf,"%d",*cubecount);
    txt = TTF_RenderText_Blended(ft,barf,{0,0,255});
    blitdest.w = txt->w;
    blitdest.h = txt->h;
    blitdest.y = 400 - txt->h;
    blitdest.x = sf->w - 32+12-txt->w/2;
    SDL_BlitSurface(txt,NULL,sf,&blitdest);
    SDL_FreeSurface(txt);
  }
  delete barf;
}
