#include"rr-stdinc.h"
#include"rr-sdlinc.h"
#include"rr-menu.h"

#include<math.h>

extern char* g_command;
extern const char* RL_VERNUM;
extern rr_keybindset* bindstorage;
// This should probably be in `rr_main.h`, but I'm not known
// for being a proper programmer.

rr_menu::rr_menu(){
  selected = 0;
  numSelections = 0;
}
rr_menu::~rr_menu(){}
void rr_menu::render(SDL_Surface* sf){}
void rr_menu::onAction(int sel){}
void rr_menu::onRawEvent(SDL_Event* e){};

void rr_menu::onInput(SDL_Event* e){
  if(reqsRawEvents){
    onRawEvent(e);
  }
  else{
    if(e->type == SDL_KEYDOWN){
      switch(e->key.keysym.sym){
      case SDLK_w:
      case SDLK_UP:
	selected --;
	if(selected < 0){
	  selected = numSelections;
	  selected--;
	}
	break;
      case SDLK_s:
      case SDLK_DOWN:
	selected ++;
	if(selected >= numSelections) selected = 0;
	break;
      case SDLK_RETURN:
	onAction(selected);
	break;
      }
    }
    else if(e->type == SDL_JOYBUTTONDOWN){
      if(e->jbutton.button == 6 || e->jbutton.button == 9) onAction(selected);
    }
    else if(e->type == SDL_JOYAXISMOTION){
    }
  }
}

rr_mainmenu::rr_mainmenu(){
  selected = 0;
  numSelections = 3;
}
rr_mainmenu::~rr_mainmenu(){}

void rr_mainmenu::render(SDL_Surface* sf){
  TTF_Font* font = rr_ttf::load("ttf/djvm.ttf",36);
  SDL_Color colors[] = {
    {128,128,128},
    {0,255,0}
  };
  SDL_Surface* tx;
  SDL_Rect blitdest;
  if(selected == 0)
    tx = TTF_RenderText_Blended(font,"POWERUP",colors[1]);
  else
    tx = TTF_RenderText_Blended(font,"POWERUP",colors[0]);
  blitdest.x = 400-tx->w/2;
  blitdest.y = 300-tx->h;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  if(selected == 1)
    tx = TTF_RenderText_Blended(font,"SETTINGS",colors[1]);
  else
    tx = TTF_RenderText_Blended(font,"SETTINGS",colors[0]);
  blitdest.x = 400-tx->w/2;
  blitdest.y = 300;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  if(selected == 2)
    tx = TTF_RenderText_Blended(font,"EXIT",colors[1]);
  else
    tx = TTF_RenderText_Blended(font,"EXIT",colors[0]);
  blitdest.x = 400-tx->w/2;
  blitdest.y = 300+tx->h;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  font = rr_ttf::load("ttf/djvm.ttf",12);
  tx = TTF_RenderText_Blended(font,"Please note, this game is in active development, there will be issues.",{255,255,255});
  blitdest.x = 12;
  blitdest.y = 12;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  tx = TTF_RenderText_Blended(font,"Please check into <http://pelirrojo.ninja/roboticslite/> for updates.",{255,255,255});
  blitdest.y = 24;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  char* barf = new char[128];
  sprintf(barf,"Currently running version %s",RL_VERNUM);
  tx = TTF_RenderText_Blended(font,barf,{255,255,255});
  blitdest.y = 36;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);
  delete barf;
}

void rr_mainmenu::onAction(int sel){
  switch(sel){
  case 0:
    strcpy(g_command,"powerup");
    break;
  case 1:
    strcpy(g_command,"settings");
    break;
  case 2:
    strcpy(g_command,"exit");
    break;
  }
}

rr_optmenu::rr_optmenu(){
  selected = 0;
  numSelections = 5;
}

rr_optmenu::~rr_optmenu(){

}

void rr_optmenu::render(SDL_Surface* sf){
  TTF_Font* font = rr_ttf::load("ttf/djvm.ttf",36);
  SDL_Surface* tx;
  SDL_Rect blitdest;
  char* barf = new char[128];
  for(int i=0;i<4;i++){
    sprintf(barf,"Player %d Keyboard Binds",i+1);
    if(selected == i)
      tx = TTF_RenderText_Blended(font,barf,{0,255,0});
    else
      tx = TTF_RenderText_Blended(font,barf,{128,128,128});
    blitdest.x = 400-tx->w/2;
    blitdest.y = 200+36*i;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
  }

  if(selected == 4)
    tx = TTF_RenderText_Blended(font,"Exit to Main Menu",{0,255,0});
  else
    tx = TTF_RenderText_Blended(font,"Exit to Main Menu",{128,128,128});
  blitdest.x = 400-tx->w/2;
  blitdest.y = 380; // 200+36*5
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);
  
  delete barf;
}

void rr_optmenu::onAction(int i){
  if(i == 4) 
    strcpy(g_command,"mainmenu");
  else if(i >= 0 && i < 4){
    char* barf = new char[32];
    sprintf(barf,"keybindeditor %d",i);
    strcpy(g_command,barf);
    delete barf;
  }
}

rr_kbdmenu::rr_kbdmenu(int kbd){
  selected = 0;
  keyboard = kbd;
  numSelections = 12;
  edit = false;
}

rr_kbdmenu::~rr_kbdmenu(){
  rr_input::writeKeybinds();
}

std::string keyname2keytext(SDL_Keycode kc){
  switch(kc){
  case SDLK_RETURN: return "RETURN";
  case SDLK_ESCAPE: return "ESCAPE";
  case SDLK_BACKSPACE: return "BACKSPACE";
  case SDLK_TAB: return "TAB";
  case SDLK_SPACE: return "SPACEBAR";
  case SDLK_CAPSLOCK: return "CAPS LOCK";
  case SDLK_LSHIFT: return "LEFT SHIFT";
  case SDLK_LCTRL: return "LEFT CONTROL";
  case SDLK_RSHIFT: return "RIGHT SHIFT";
  case SDLK_F1: return "F1";
  case SDLK_F2: return "F2";
  case SDLK_F3: return "F3";
  case SDLK_F4: return "F4";
  case SDLK_F5: return "F5";
  case SDLK_F6: return "F7";
  case SDLK_F8: return "F8";
  case SDLK_F9: return "F9";
  case SDLK_F10: return "F10";
  case SDLK_F11: return "F11";
  case SDLK_F12: return "F12";
  case SDLK_PRINTSCREEN: return "PRINTSCR";
  case SDLK_SCROLLLOCK: return "SCROLL LOCK";
  case SDLK_PAUSE: return "PAUSE";
  case SDLK_INSERT: return "INSERT";
  case SDLK_HOME: return "HOME";
  case SDLK_PAGEUP: return "PAGE UP";
  case SDLK_DELETE: return "DELETE";
  case SDLK_END: return "END";
  case SDLK_PAGEDOWN: return "PAGE DOWN";
  case SDLK_RCTRL: return "RIGHT CONTROL";
  case SDLK_RIGHT: return "ARROW RIGHT";
  case SDLK_LEFT: return "ARROW LEFT";
  case SDLK_UP: return "ARROW UP";
  case SDLK_DOWN: return "ARROW DOWN";
  case SDLK_NUMLOCKCLEAR: return "NUMLOCK";
  case SDLK_KP_1: return "KP 1";
  case SDLK_KP_2: return "KP 2";
  case SDLK_KP_3: return "KP 3";
  case SDLK_KP_4: return "KP 4";
  case SDLK_KP_5: return "KP 5";
  case SDLK_KP_6: return "KP 6";
  case SDLK_KP_7: return "KP 7";
  case SDLK_KP_8: return "KP 8";
  case SDLK_KP_9: return "KP 9";
  case SDLK_KP_0: return "KP 0";
  case SDLK_KP_PERIOD: return "KP .";

  default:
    return std::string(((char*) &kc));
  }
}

const char* keynames[] = {
  "Move Up",
  "Move Left",
  "Move Down",
  "Move Right",
  "Strafe",
  "Drop Cube",
  "Powerup 1",
  "Powerup 2",
  "Powerup 3",
  "Taunt",
  "\"Start\""
};

void rr_kbdmenu::render(SDL_Surface* sf){
  SDL_Surface* tx;
  SDL_Keycode* lbinds = (SDL_Keycode*) &(bindstorage[keyboard]);
  TTF_Font* font = rr_ttf::load("ttf/djvm.ttf",24);
  SDL_Rect blitdest;
  char* barf = new char[512];
  SDL_Color colors[] = {
    {128,128,128},
    {0,255,0},
    {255,0,0}
  };

  for(int i=0;i<11;i++){
    sprintf(barf,"%s: %s",
	    keynames[i],
	    keyname2keytext(lbinds[i]).c_str());
    if(selected == i && !edit)
      tx = TTF_RenderText_Blended(font,barf,colors[1]);
    else if(selected == i && edit)
      tx = TTF_RenderText_Blended(font,barf,colors[2]);
    else
      tx = TTF_RenderText_Blended(font,barf,colors[0]);
    blitdest.x = 64;
    blitdest.y = 64 + i*24;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
  }

  if(selected == 11)
    tx = TTF_RenderText_Blended(font,"Save Keybinds & Exit",colors[1]);
  else
    tx = TTF_RenderText_Blended(font,"Save Keybinds & Exit",colors[0]);
  blitdest.x = 64;
  blitdest.y = 324;
  blitdest.w = tx->w;
  blitdest.h = tx->h;
  SDL_BlitSurface(tx,NULL,sf,&blitdest);
  SDL_FreeSurface(tx);

  delete barf;
}

void rr_kbdmenu::onAction(int i){
  if(i == 11)
    strcpy(g_command,"settings");
  else if(!edit){
    edit = true;
    reqsRawEvents = true;
  }
}

void rr_kbdmenu::onRawEvent(SDL_Event* e){
  if(edit && e->type == SDL_KEYDOWN){
    SDL_Keycode* lbinds = (SDL_Keycode*) &(bindstorage[keyboard]);
    lbinds[selected] = e->key.keysym.sym;
    edit = false;
    reqsRawEvents = false;
    rr_input::writeKeybinds();
  }
}
