#include"rr-sdlinc.h"
#include"rr-stdinc.h"
#include"rr-hud.h"
#include"rr-ttf.h"

#include<math.h>

rr_hud::rr_hud(){
  // Temporary Fixer
}

rr_thud::rr_thud(int* g,int* c,int* t){
  gameptr = g;
  ctdnptr = c;
  timeptr = t;
}

rr_hud::~rr_hud(){
}

rr_schud::rr_schud(int c,int p,int* vpt,int* r){
  color = c;
  position = p;
  valueptr = vpt;
  rdyptr = r;
}

void rr_hud::render(SDL_Surface* sf){
  /*
   * To prevent the computer from crashing and letting programmers know
   * where there's an issue.
   */
  printf("In rr_hud::render() ... Should Not be here, Debug and set Breakpoint\n");
}

void rr_thud::render(SDL_Surface* sf){
  TTF_Font* ssf = rr_ttf::load("ttf/7seg.ttf",32);
  SDL_Surface* tx;
  SDL_Rect blitdest;
  char* barf = new char[64];
  if(gameptr == NULL){
    tx = TTF_RenderText_Blended(ssf,"NULL",{192,0,0});
    blitdest.x = sf->w/2-tx->w/2;
    blitdest.y = 12;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
  }
  else{
    switch(*gameptr){
    case LEVELSTATE_WAITING:
      tx = TTF_RenderText_Blended(ssf,"-----",{0,255,0});
      blitdest.x = sf->w/2-tx->w/2;
      blitdest.y = 12;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);
      break;
    case LEVELSTATE_COUNTDOWN:
      if(*ctdnptr != 0){
	sprintf(barf,"- %d -",*ctdnptr);
	tx = TTF_RenderText_Blended(ssf,barf,{0,255,0});
      }
      else{
	tx = TTF_RenderText_Blended(ssf,"START",{0,255,0});
      }
      blitdest.x = sf->w/2-tx->w/2;
      blitdest.y = 12;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);
      break;
    case LEVELSTATE_PSEUDOAUTO:
    case LEVELSTATE_TELEOP:
      sprintf(barf,"%03d",*timeptr);
      if(*timeptr <= 30)
	tx = TTF_RenderText_Blended(ssf,barf,{255,128,0});
      else
	tx = TTF_RenderText_Blended(ssf,barf,{255,255,255});
      blitdest.x = sf->w/2-tx->w/2;
      blitdest.y = 12;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);
      break;
    }
  }
  delete barf;
}

void rr_schud::render(SDL_Surface* sf){
  SDL_Rect blitdest;
  char* barf = new char[64];
  blitdest.w = 100;
  blitdest.h = 10;
  blitdest.x = 400 + 100*position-50;
  blitdest.y = sf->h-10;
  switch(color){
  case 0:
    SDL_FillRect(sf,&blitdest,SDL_MapRGB(sf->format,255,0,0));
    break;
  case 1:
    SDL_FillRect(sf,&blitdest,SDL_MapRGB(sf->format,0,0,255));
    break;
  case 2:
    SDL_FillRect(sf,&blitdest,SDL_MapRGB(sf->format,0,128,0));
    break;
  case 3:
    SDL_FillRect(sf,&blitdest,SDL_MapRGB(sf->format,255,255,0));
    break;
  default:
    SDL_FillRect(sf,&blitdest,SDL_MapRGB(sf->format,128,128,128));
    break;
  }

  TTF_Font* ft = rr_ttf::load("ttf/7seg.ttf",36);
  sprintf(barf,"%05d",*valueptr);
  SDL_Surface* txt = TTF_RenderText_Blended(ft,barf,{255,255,255});
  blitdest.y -= 36;
  blitdest.x = 400+100*position-(txt->w/2);
  blitdest.w = txt->w;
  blitdest.h = txt->h;
  SDL_BlitSurface(txt,NULL,sf,&blitdest);
  SDL_FreeSurface(txt);
  delete barf;
}

rr_counthud::rr_counthud(int w,int s,int cx,int by,int* c,bool cs){
  width = w;
  fontsize = s;
  cntrx = cx;
  basey = by;
  countptr = c;
  colorsign = cs;
}

void rr_counthud::render(SDL_Surface* sf){
  TTF_Font* fnt = rr_ttf::load("ttf/7seg.ttf",fontsize);
  SDL_Color c = {255,255,255};
  int display = *countptr;
  char* barf = new char[8];
  if(colorsign){
    if(display > 0) c = {255,0,0};
    else if(display < 0){
      c = {0,0,255};
      display *= -1;
    }
  }
  sprintf(barf,"%d",display);
  SDL_Rect blitdest;
  SDL_Surface* tx = TTF_RenderText_Blended(fnt,barf,c);
  blitdest.x = cntrx-tx->w/2;
  blitdest.y = basey;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);
  
  delete barf;
}

const SDL_Color rdcolors[] = {
  {255,0,0},
  {255,128,0},
  {255,64,192},
  {128,0,0}
};

const SDL_Color blucolors[] = {
  {0,0,255},
  {128,0,128},
  {0,192,192},
  {0,0,128}
};

const SDL_Color fourpcolors[] = {
  {255,0,0},
  {0,0,255},
  {0,128,0},
  {255,255,0} // Grrrrrrrrrrr
};

rr_mp_readyhud::rr_mp_readyhud(int s,int* p,int* g){
  side = s;
  rdyptr = p;
  gameptr = g;
}

void rr_mp_readyhud::render(SDL_Surface* sf){
  if(*gameptr == LEVELSTATE_WAITING){
    TTF_Font* txt = rr_ttf::load("ttf/djvm.ttf",16);
    TTF_Font* svs = rr_ttf::load("ttf/7seg.ttf",24);
    SDL_Surface* tx;
    SDL_Rect blitdest;
    if(side == 0 || side == 1){
      tx = TTF_RenderText_Blended(txt,"Press Any Button to Ready Up",{0,0,0});
      blitdest.x = 400-tx->w/2;
      blitdest.y = 100;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);
    }
    if(*rdyptr != -1){
      if(side == 0){
	// TODO
      }
      else if(side == 1 || side == 2){
	int ctmax = (*rdyptr&0x03)+1;
	for(int i=0;i<ctmax;i++){
	  int blah = (int) pow(2,i+2);
	  SDL_Color c;
	  if(side == 1) c = rdcolors[i];
	  if(side == 2) c = blucolors[i];
	  char* barf = new char[32];
	  if(*rdyptr & blah){
	    switch(side){
	    case 1: sprintf(barf,"%d -RDY-",i+1); break;
	    case 2: sprintf(barf,"-RDY- %d",i+1); break;
	    }
	  }
	  else{
	    switch(side){
	    case 1: sprintf(barf,"%d -   -",i+1); break;
	    case 2: sprintf(barf,"-   - %d",i+1); break;
	    }
	  }
	  tx = TTF_RenderText_Blended(svs,barf,c);
	  blitdest.w = tx->w;
	  blitdest.h = tx->h;
	  if(side == 1) blitdest.x = 50;
	  if(side == 2) blitdest.x = 800-50-tx->w;
	  blitdest.y = 200+64*i;
	  SDL_BlitSurface(tx,NULL,sf,&blitdest);
	  SDL_FreeSurface(tx);
	  delete barf;
	}
      }
    }
  }
  // Side 0: Center (Four four all)
  // Side 1: Left (Red Team, also implies the Press Any Button to ready up text)
  // Side 2: Right (Blue Team)
  // Rdyptr: 0b00000000
  //             ||||++- Count of Members (0 = 1)
  //             ++++--- Bitmask of members that are ready
}

rr_geschud::rr_geschud(int* r,int* b){
  totr = r;
  totb = b;
}

rr_geschud::~rr_geschud(){

}

void rr_geschud::render(SDL_Surface* sf){

}
