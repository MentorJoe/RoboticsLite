#include<vector>
#include<map>
#include<memory>
#include<queue>
#include"rr-sdlinc.h"
#include"rr-input.h"
#include"rr-main.h"
#include"rl-confdir.h"

short* inptrs = NULL;
short* stickbtn = NULL;

rr_keybindset* bindstorage = NULL;

std::vector< std::shared_ptr<rr_inputsys> > inputs;
std::vector< std::shared_ptr<rr_inputsys> > keyinputs;
std::map< SDL_JoystickID, std::shared_ptr<rr_inputsys> > joyinputs;
std::vector<SDL_Joystick*> rawsticks;
bool kbdRaw = false;
std::queue<SDL_Event> kbdRawQueue;

#ifdef RR_ACE
std::vector<std::shared_ptr<rr_inputsys>> ace_joyinputs;
std::map<SDL_JoystickID,std::shared_ptr<rr_inputsys>> ace_joymap;
#endif

int keys = 0;
int joys = 0;

bool rr_input::clearInputs(){
  if(inputs.size() == 0 &&
     joyinputs.size() == 0 &&
     keyinputs.size() == 0)
    return false;
  if(inputs.size() != 0) inputs.clear();
  if(joyinputs.size() != 0) joyinputs.clear();
  if(keyinputs.size() != 0) keyinputs.clear();
  keys = 0;
  joys = 0;
  return true;
}

void rr_input::tickReset(){
#ifdef RR_ACE
  for(auto i = ace_joyinputs.begin(); i != ace_joyinputs.end(); ++i){
    (*i)->tickReset();
  }
#endif
}

bool rr_input::reqsReset(){
#ifdef RR_ACE
  bool status = false;
  for(auto i = ace_joyinputs.begin(); i != ace_joyinputs.end(); ++i){
    if((*i)->reqsReset())
      status = true;
  }
  if(status){
    for(auto i = ace_joyinputs.begin(); i != ace_joyinputs.end(); ++i){
      (*i)->resetReset();
    }
  }
  return status;
#else
  return false;
#endif
}

bool rr_input::setKbdRawEvent(bool status){
#ifndef RR_ACE
  if(kbdRaw == status) return false;
  kbdRaw = status;
#else
  /*
   * There are no keyboards on an arcade cabinet.
   */
  return false;
#endif
}

int rr_input::pollKbdRawEvent(SDL_Event* e){
#ifndef RR_ACE
  if(!kbdRaw) return 0;
  int size = kbdRawQueue.size();
  if(size == 0) return 0;
  if(!kbdRawQueue.empty()){
    memcpy(e,&(kbdRawQueue.front()),sizeof(SDL_Event));
    kbdRawQueue.pop();
  }
  return size;
#else
  return 0;
#endif
}

std::shared_ptr<rr_inputsys> rr_input::newKbd(){
  std::shared_ptr<rr_inputsys> sys = std::shared_ptr<rr_inputsys>(new rr_kbdinput(keys));
  inputs.push_back(std::shared_ptr<rr_inputsys>(sys));
  keyinputs.push_back(std::shared_ptr<rr_inputsys>(sys));
  keys++;
  return sys;
}

std::shared_ptr<rr_inputsys> rr_input::newJoy(int n){
  std::shared_ptr<rr_inputsys> sys = std::shared_ptr<rr_inputsys>(new rr_joyinput(n));
  joyinputs[n] = std::shared_ptr<rr_inputsys>(sys);
  inputs.push_back(std::shared_ptr<rr_inputsys>(sys));
  joys++;
  return sys;
}

void rr_input::writeKeybinds(){
#ifdef DEBUG
  printf("Writing keybinds\n");
#endif
  FILE* fo = fopen(rl_confdir::getKeybindFileName().c_str(),"wb");
  if(fo != NULL){
    fwrite(bindstorage,sizeof(rr_keybindset),4,fo);
    fclose(fo);
  }
  else{
    printf("Unable to open keyconf file for writing\n");
  }
}

void rr_input::setup(){
  bindstorage = new rr_keybindset[4];
  bzero(bindstorage,sizeof(rr_keybindset)*4);
  std::string keyconf = rl_confdir::getKeybindFileName();
#ifdef DEBUG
  printf("Attempting to open %s for key configurations.\n",keyconf.c_str());
#endif
  FILE* fp = fopen(keyconf.c_str(),"rb");
  if(fp != NULL){
    fread(bindstorage,sizeof(rr_keybindset),4,fp);
    fclose(fp);
  }
  else{
#ifdef DEBUG
    printf("No keybind file found, populating from scratch.\n");
#endif
    // Manually populate key configuration options if there isn't a file found
    // Also write it out to the file
    bindstorage[0].moveN = SDLK_w;
    bindstorage[0].moveW = SDLK_a;
    bindstorage[0].moveS = SDLK_s;
    bindstorage[0].moveE = SDLK_d;
    bindstorage[0].strafeLock = SDLK_LSHIFT;
    bindstorage[0].action = SDLK_SPACE;
    bindstorage[0].powerup1 = SDLK_q;
    bindstorage[0].powerup2 = SDLK_e;
    bindstorage[0].powerup3 = SDLK_f;
    bindstorage[0].taunt = SDLK_r;
    bindstorage[0].start = SDLK_RETURN;

    bindstorage[1].moveN = SDLK_UP;
    bindstorage[1].moveW = SDLK_LEFT;
    bindstorage[1].moveS = SDLK_DOWN;
    bindstorage[1].moveE = SDLK_RIGHT;
    bindstorage[1].strafeLock = SDLK_RCTRL;
    bindstorage[1].action = SDLK_RSHIFT;
    bindstorage[1].powerup1 = SDLK_DELETE;
    bindstorage[1].powerup2 = SDLK_END;
    bindstorage[1].powerup3 = SDLK_PAGEDOWN;
    bindstorage[1].taunt = SDLK_PAGEUP;

    bindstorage[2].moveN = SDLK_i;
    bindstorage[2].moveW = SDLK_j;
    bindstorage[2].moveS = SDLK_k;
    bindstorage[2].moveE = SDLK_l;
    bindstorage[2].strafeLock = SDLK_n;
    bindstorage[2].action = SDLK_SEMICOLON;
    bindstorage[2].powerup1 = SDLK_u;
    bindstorage[2].powerup2 = SDLK_o;
    bindstorage[2].powerup3 = SDLK_p;
    bindstorage[2].taunt = SDLK_y;

    bindstorage[3].moveN = SDLK_KP_5;
    bindstorage[3].moveW = SDLK_KP_1;
    bindstorage[3].moveS = SDLK_KP_2;
    bindstorage[3].moveE = SDLK_KP_3;
    bindstorage[3].strafeLock = SDLK_KP_4;
    bindstorage[3].action = SDLK_KP_0;
    bindstorage[3].powerup1 = SDLK_KP_7;
    bindstorage[3].powerup2 = SDLK_KP_8;
    bindstorage[3].powerup3 = SDLK_KP_9;
    bindstorage[3].taunt = SDLK_KP_PERIOD;

    rr_input::writeKeybinds();
  }
  inputs.reserve(8);
  keyinputs.reserve(8);
  rawsticks.reserve(8);
#ifdef RR_ACE
  ace_joyinputs.reserve(8);
#endif
  for(int i=0;i<SDL_NumJoysticks();i++){
    rawsticks.push_back(SDL_JoystickOpen(i));
    // That'll be the only place those SDL_Joystick*s will be,
    // No need for the overhead of smart pointers
  }
#ifdef RR_ACE
  for(int i=0;i<SDL_NumJoysticks();i++){
    std::shared_ptr<rr_inputsys> stick(new rr_joyinput(i));
    ace_joyinputs.push_back(std::shared_ptr<rr_inputsys>(stick));
    inputs.push_back(std::shared_ptr<rr_inputsys>(stick));
    joyinputs[i] = std::shared_ptr<rr_inputsys>(stick);
    ace_joymap[i] = std::shared_ptr<rr_inputsys>(stick);
  }
#endif
}

void rr_input::cleanup(){
#ifdef DEBUG
  printf("In rr_input::cleanup()\n");
#endif
  // Write Keybinds to file
  rr_input::writeKeybinds();
  // Actual end-of-process cleanup
  if(inptrs != NULL) delete inptrs;
  if(stickbtn != NULL) delete stickbtn;
  if(bindstorage != NULL) delete bindstorage;
  inputs.clear();
  joyinputs.clear();
  keyinputs.clear();
  for(int i=0;i<rawsticks.size();i++){
    SDL_JoystickClose(rawsticks[i]);
  }
}

void rr_input::joyreset(){
  for(int i=0;i<rawsticks.size();i++){
    SDL_JoystickClose(rawsticks[i]);
  }
  rawsticks.clear();
  for(int i=0;i<SDL_NumJoysticks();i++){
    rawsticks.push_back(SDL_JoystickOpen(i));
  }
}

#ifdef RR_ACE
void rr_input::ace_clearall(){
  ace_joyinputs.clear();
}

void rr_input::ace_assign(SDL_JoystickID i){
  ace_joyinputs.push_back(std::shared_ptr<rr_inputsys>(new rr_joyinput(i)));
}

int rr_input::ace_numJoysticks(){
  return ace_joyinputs.size();
}

std::shared_ptr<rr_inputsys> rr_input::ace_getStick(int i){
  if(i >= ace_joyinputs.size())
    return std::shared_ptr<rr_inputsys>(NULL);
  else
    return std::shared_ptr<rr_inputsys>(ace_joyinputs[i]);
}

void rr_input::ace_setSWStick(std::shared_ptr<rr_inputsys> stick){
  ace_joyinputs.push_back(std::shared_ptr<rr_inputsys>(stick));
}
#endif

bool rr_input::onEvent(SDL_Event* e){
#ifdef RR_ACE
  if(e->type == SDL_JOYAXISMOTION ||
     e->type == SDL_JOYHATMOTION ||
     e->type == SDL_JOYBUTTONDOWN ||
     e->type == SDL_JOYBUTTONUP){
    if(ace_joymap.count(e->jaxis.which) == 1){
      return ace_joymap[e->jaxis.which]->onSDLEvent(e);
    }
  }
  return false;
#else
  if(e->type == SDL_JOYAXISMOTION ||
     e->type == SDL_JOYHATMOTION ||
     e->type == SDL_JOYBUTTONDOWN ||
     e->type == SDL_JOYBUTTONUP){
    if(joyinputs.count(e->jaxis.which) == 1){
      return joyinputs[e->jaxis.which]->onSDLEvent(e);
    }
    else return false;
  }
  else{
    if(!kbdRaw){
      for(int i=0;i<keyinputs.size();i++){
	// Don't change this to `return keyinputs[i]->onSDLEvent(e)`
	if(keyinputs[i]->onSDLEvent(e)) return true;
      }
    }
    else if(e->type == SDL_KEYDOWN){
      // Raw Keyboard reading is requested (Text Input)
      kbdRawQueue.push(*e);
    }
  }
  return false;
#endif
}

rr_inputsys::rr_inputsys(){
  // eventqueue.reserve(8); // TODO
}

// --== Filler stuffs ==--
rr_inputsys::~rr_inputsys(){}
short rr_inputsys::getMovementDir(){return 0;}
short* rr_inputsys::getDirPtr(){return NULL;}
rr_inputtype rr_inputsys::getType(){return NONE;}
bool rr_inputsys::onSDLEvent(SDL_Event* e){return false;}
SDL_Surface* rr_inputsys::get_ico(){return NULL;}
bool rr_inputsys::wantsRebind(){return false;}
void rr_inputsys::renderRebind(SDL_Surface* sf){}
bool rr_inputsys::reqsReset(){return false;}
void rr_inputsys::tickReset(){}
void rr_inputsys::resetReset(){}
#ifdef RR_ACE
bool rr_inputsys::ace_hasInput(){return false;}
#endif

int rr_inputsys::pollEvent(rr_event* e){
  if(this->eventqueue.size() == 0)
    return 0;
  else{
    rr_event r = eventqueue.front();
    memcpy(e,&r,sizeof(rr_event));
    eventqueue.pop();
    return eventqueue.size()+1;
  }
}

void rr_inputsys::putEvent(rr_event* e){
  eventqueue.push(*e);
}

rr_kbdinput::rr_kbdinput(int defaultset):rr_inputsys(){
  if(defaultset < 0 || defaultset >= 4)
    defaultset = 0;
  bindsetico = defaultset;
  binds = bindstorage[defaultset];
  keydownmask = 0;
  movemask = 0;
  icon = NULL;
}

rr_kbdinput::~rr_kbdinput(){
  if(icon != NULL) SDL_FreeSurface(icon);
}

bool rr_kbdinput::onSDLEvent(SDL_Event* e){
  if(e->type == SDL_KEYDOWN){
    if(e->key.keysym.sym == binds.moveN){
      movemask |= 0x01;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 0;
      putEvent(&e);
    }
    else if(e->key.keysym.sym == binds.moveW){
      movemask |= 0x02;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 1;
      putEvent(&e);
    }
    else if(e->key.keysym.sym == binds.moveS){
      movemask |= 0x04;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 2;
      putEvent(&e);
    }
    else if(e->key.keysym.sym == binds.moveE){
      movemask |= 0x08;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 3;
      putEvent(&e);
    }
    else if(e->key.keysym.sym == binds.strafeLock) movemask |= 0x10;
    else if(e->key.keysym.sym == binds.action){
      if((keydownmask & 0x01) != 0x01){
	keydownmask |= 0x01;
	rr_event r;
	r.type = DROP_CUBE;
	putEvent(&r);
      }
    }
    else if(e->key.keysym.sym == binds.powerup1){
      if((keydownmask & 0x02) != 0x02){
	keydownmask |= 0x02;
	rr_event r;
	r.type = CUE_POWERUP;
	r.subtype = 1;
	putEvent(&r);
      }
    }
    else if(e->key.keysym.sym == binds.powerup2){
      if((keydownmask & 0x04) != 0x04){
	keydownmask |= 0x04;
	rr_event r;
	r.type = CUE_POWERUP;
	r.subtype = 2;
	putEvent(&r);
      }
    }
    else if(e->key.keysym.sym == binds.powerup3){
      if((keydownmask & 0x08) != 0x08){
	keydownmask |= 0x08;
	rr_event r;
	r.type = CUE_POWERUP;
	r.subtype = 3;
	putEvent(&r);
      }
    }
    else if(e->key.keysym.sym == binds.taunt){
      if((keydownmask & 0x10) != 0x10){
	keydownmask |= 0x10;
	rr_event r;
	r.type = ETAUNT;
	putEvent(&r);
      }
    }
    else if(e->key.keysym.sym == binds.start){
      if((keydownmask & 0x20) != 0x20){
	keydownmask |= 0x20;
	rr_event r;
	r.type = START;
	putEvent(&r);
      }
    }
    else return false;
    return true;
  }
  else if(e->type == SDL_KEYUP){
    if     (e->key.keysym.sym == binds.moveN)       movemask &= ~0x01;
    else if(e->key.keysym.sym == binds.moveW)       movemask &= ~0x02;
    else if(e->key.keysym.sym == binds.moveS)       movemask &= ~0x04;
    else if(e->key.keysym.sym == binds.moveE)       movemask &= ~0x08;
    else if(e->key.keysym.sym == binds.strafeLock)  movemask &= ~0x10;
    else if(e->key.keysym.sym == binds.action)	 keydownmask &= ~0x01;
    else if(e->key.keysym.sym == binds.powerup1) keydownmask &= ~0x02;
    else if(e->key.keysym.sym == binds.powerup2) keydownmask &= ~0x04;
    else if(e->key.keysym.sym == binds.powerup3) keydownmask &= ~0x08;
    else if(e->key.keysym.sym == binds.taunt)	 keydownmask &= ~0x10;
    else if(e->key.keysym.sym == binds.start)	 keydownmask &= ~0x20;
    else return false;
    return true;
  }
  return false;
}

short rr_kbdinput::getMovementDir(){
  return movemask;
}

short* rr_kbdinput::getDirPtr(){
  return &movemask;
}

rr_inputtype rr_kbdinput::getType(){
  return KEYBOARD;
}

SDL_Surface* rr_kbdinput::get_ico(){
  if(icon == NULL){
    switch(bindsetico){
    case 0:
      icon = IMG_Load("imgs/wasd.png");
      break;
    case 1:
      icon = IMG_Load("imgs/arrkey.png");
      break;
    case 2:
      icon = IMG_Load("imgs/ijkl.png");
      break;
    case 3:
      icon = IMG_Load("imgs/tenkey.png");
      break;
    default:
      icon = IMG_Load("imgs/wasd.png");
      break;
    }
  }
  return icon;
}


rr_joyinput::rr_joyinput(int stick):rr_inputsys(){
  stickNumber = stick;
  keydownmask = 0;
  movemask = 0;
  icon = NULL;
  
  binds.moveN.axis = 0x00000001;
  binds.moveN.deadzone = 14000;
  binds.moveS.axis = 0x00001001;
  binds.moveS.deadzone = 14000;
  binds.moveW.axis = 0x00000000;
  binds.moveW.deadzone = 14000;
  binds.moveE.axis = 0x00001000;
  binds.moveE.deadzone = 14000;
  binds.strafeLock.axis = 0x00010009;
  binds.action.axis = 0x00010001;

  binds.powerup1.axis = 0x00001003;
  binds.powerup1.deadzone = 18000;
  binds.powerup2.axis = 0x00001002;
  binds.powerup2.deadzone = 18000;
  binds.powerup3.axis = 0x00000003;
  binds.powerup3.deadzone = 18000;

  binds.taunt.axis = 0x00010002;
  binds.start.axis = 0x00010008;

  // 0x00000000
  //      |||++-- Number
  //      ||+---- UNUSED
  //      |+----- Direction (1 == positive, 0 == negative)
  //      +------ Button (1) or Axis(0)
}

rr_joyinput::~rr_joyinput(){
  if(icon != NULL) SDL_FreeSurface(icon);
}

short rr_joyinput::getMovementDir(){
  return movemask;
}

short* rr_joyinput::getDirPtr(){
  return &movemask;
}

rr_inputtype rr_joyinput::getType(){
  return JOYSTICK;
}

bool rr_joyinput::onSDLEvent(SDL_Event* e){
  // TODO convert this to actually use the rebinds.
  if(e->type == SDL_JOYAXISMOTION){
    if(e->jaxis.axis == 3 || e->jaxis.axis == 1){
      // Because for some reason the axis maps differently on the rPi than my laptop
      // Why, I don't know.
      // Una cerveza, por favor. KBC.
      if(e->jaxis.value < -14000){
	if(!(movemask & 0x01)){
	  rr_event e;
	  e.type = HSCORE_MOVEMENT;
	  e.subtype = 0;
	  putEvent(&e);
	}
	movemask |= 0x01;
      }
      else
	movemask &= ~0x01;
      if(e->jaxis.value > 14000){
	if(!(movemask & 0x04)){
	  rr_event e;
	  e.type = HSCORE_MOVEMENT;
	  e.subtype = 2;
	  putEvent(&e);
	}
	movemask |= 0x04;
      }
      else
	movemask &= ~0x04;
    }
    if(e->jaxis.axis == 0){
      if(e->jaxis.value < -14000){
	if(!(movemask & 0x02)){
	  rr_event e;
	  e.type = HSCORE_MOVEMENT;
	  e.subtype = 1;
	  putEvent(&e);
	}
	movemask |= 0x02;
      }
      else
	movemask &= ~0x02;
      if(e->jaxis.value > 14000){
	if(!(movemask & 0x08)){
	  rr_event e;
	  e.type = HSCORE_MOVEMENT;
	  e.subtype = 3;
	  putEvent(&e);
	}
	movemask |= 0x08;
      }
      else
	movemask &= ~0x08;
    }
    return true;
  }
  if(e->type == SDL_JOYHATMOTION){
    if(e->jhat.value & SDL_HAT_UP){
      movemask |= 0x01;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 0;
      putEvent(&e);
    }
    else
      movemask &= ~0x01;
    if(e->jhat.value & SDL_HAT_RIGHT){
      movemask |= 0x08;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 3;
      putEvent(&e);
    }
    else
      movemask &= ~0x08;
    if(e->jhat.value & SDL_HAT_DOWN){
      movemask |= 0x04;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 2;
      putEvent(&e);
    }
    else
      movemask &= ~0x04;
    if(e->jhat.value & SDL_HAT_LEFT){
      movemask |= 0x02;
      rr_event e;
      e.type = HSCORE_MOVEMENT;
      e.subtype = 1;
      putEvent(&e);
    }
    else
      movemask &= ~0x02;
    return true;
  }
  if(e->type == SDL_JOYBUTTONDOWN){
    if(e->jbutton.button == 7 || e->jbutton.button == 4)
      movemask |= 0x10;
    else if(e->jbutton.button == 6 && ((keydownmask & 0x01) != 0x01)){
      keydownmask |= 0x01;
      rr_event r;
      r.type = DROP_CUBE;
      putEvent(&r);
    }
    else if(e->jbutton.button == 3 && ((keydownmask & 0x02) != 0x02)){
      // Bloody OCD!
      keydownmask |= 0x02;
      rr_event r;
      r.type = CUE_POWERUP;
      r.subtype = 1;
      putEvent(&r);
    }
    else if(e->jbutton.button == 2 && ((keydownmask & 0x04) != 0x04)){
      keydownmask |= 0x04;
      rr_event r;
      r.type = CUE_POWERUP;
      r.subtype = 2;
      putEvent(&r);
    }
    else if(e->jbutton.button == 1 && ((keydownmask & 0x08) != 0x08)){
      keydownmask |= 0x08;
      rr_event r;
      r.type = CUE_POWERUP;
      r.subtype = 3;
      putEvent(&r);
    }
    else if(e->jbutton.button == 8 && ((keydownmask & 0x10) != 0x10)){
      keydownmask |= 0x10;
      rr_event r;
      r.type = ETAUNT;
      putEvent(&r);
    }
    else if(e->jbutton.button == 9 && ((keydownmask & 0x20) != 0x20)){
      keydownmask |= 0x20;
      rr_event r;
      r.type = START;
      putEvent(&r);
    }
    else return false;
    if(keydownmask & 0x11){
      rr_event r;
      r.type = ACE_AB;
      putEvent(&r);
    }
    return true;
  }
  if(e->type == SDL_JOYBUTTONUP){
    if(e->jbutton.button == 7 || e->jbutton.button == 4)
      movemask &= ~0x10;
    else if(e->jbutton.button == 6) keydownmask &= ~0x01;
    else if(e->jbutton.button == 3) keydownmask &= ~0x02;
    else if(e->jbutton.button == 2) keydownmask &= ~0x04;
    else if(e->jbutton.button == 1) keydownmask &= ~0x08;
    else if(e->jbutton.button == 8) keydownmask &= ~0x10;
    else if(e->jbutton.button == 9) keydownmask &= ~0x20;
    else return false;
    return true;

  }
  return false;
}

SDL_Surface* rr_joyinput::get_ico(){
  if(icon == NULL){ // Soy idiota... please don't look at the change here.
    switch(stickNumber){
    case 0:
      icon = IMG_Load("imgs/ctrlr1.png");
      break;
    case 1:
      icon = IMG_Load("imgs/ctrlr2.png");
      break;
    case 2:
      icon = IMG_Load("imgs/ctrlr3.png");
      break;
    case 3:
      icon = IMG_Load("imgs/ctrlr4.png");
      break;
    default:
      icon = IMG_Load("imgs/controller.png");
      break;
    }
  }
  return icon;
}

bool rr_joyinput::reqsReset(){
  if(resetTimer.getTicks() > 3000)
    return true;
  else
    return false;
}

void rr_joyinput::tickReset(){
  if(!resetTimer.esStarted() &&
     (movemask & 0x10) &&
     (keydownmask & (0x01 | 0x10)))
    resetTimer.start();
}

void rr_joyinput::resetReset(){
  resetTimer.stop();
  movemask = 0;
  keydownmask = 0;
}

#ifdef RR_ACE
bool rr_joyinput::ace_hasInput(){
  // Function reaches fine...
  return (bool) (keydownmask != 0 || movemask != 0);
}
#endif
