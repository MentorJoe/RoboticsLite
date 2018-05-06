/* -*- C++ -*- */
#ifndef _rr_input_h_
#define _rr_input_h_

#include<vector>
#include<memory>
#include<queue>

#include"rr-main.h"

enum rr_eventtype{
  DROP_CUBE = 0x01,
  CUE_POWERUP,
  ETAUNT,
  SHOOT_BOULDER,
  START,
  ACE_AB,
  HSCORE_MOVEMENT
};

struct rr_event{
  rr_eventtype type;
  int subtype;
  int player;
};

enum rr_inputtype{
  NONE,
  KEYBOARD,
  JOYSTICK,
  NETWORK
};

struct rr_joyinputb{
  int axis;
  int deadzone;
};

struct rr_joyinputset{
  rr_joyinputb moveN;
  rr_joyinputb moveW;
  rr_joyinputb moveS;
  rr_joyinputb moveE;
  rr_joyinputb strafeLock;
  rr_joyinputb action;
  rr_joyinputb powerup1;
  rr_joyinputb powerup2;
  rr_joyinputb powerup3;
  rr_joyinputb taunt;
  rr_joyinputb start;
};

struct rr_keybindset{
  SDL_Keycode moveN;
  SDL_Keycode moveW;
  SDL_Keycode moveS;
  SDL_Keycode moveE;
  SDL_Keycode strafeLock;
  SDL_Keycode action;
  SDL_Keycode powerup1;
  SDL_Keycode powerup2;
  SDL_Keycode powerup3;
  SDL_Keycode taunt;
  SDL_Keycode start;
};

class rr_inputsys{
public:
  rr_inputsys();
  virtual ~rr_inputsys();
  virtual short getMovementDir();
  virtual short* getDirPtr();
  virtual int pollEvent(rr_event*);
  virtual void putEvent(rr_event*);
  virtual rr_inputtype getType();
  virtual bool onSDLEvent(SDL_Event*);
  virtual SDL_Surface* get_ico();
  virtual bool wantsRebind();
  virtual void renderRebind(SDL_Surface*);
  virtual bool reqsReset();
  virtual void tickReset();
  virtual void resetReset();
#ifdef RR_ACE
  virtual bool ace_hasInput();
#endif
protected:
  std::queue<rr_event> eventqueue;
};

class rr_kbdinput:public rr_inputsys{
public:
  rr_kbdinput(int);
  ~rr_kbdinput();
  void setDefault(int);
  short getMovementDir();
  short* getDirPtr();
  rr_inputtype getType();
  bool onSDLEvent(SDL_Event*);
  SDL_Surface* get_ico();
private:
  rr_keybindset binds;
  long keydownmask;
  short movemask;
  int bindsetico;
  SDL_Surface* icon;
};

class rr_joyinput:public rr_inputsys{
public:
  rr_joyinput(int n);
  ~rr_joyinput();
  short getMovementDir();
  short* getDirPtr();
  rr_inputtype getType();
  bool onSDLEvent(SDL_Event*);
  SDL_Surface* get_ico();
  bool reqsReset();
  void tickReset();
  void resetReset();
#ifdef RR_ACE
  bool ace_hasInput();
#endif
private:
  rr_joyinputset binds;
  int stickNumber;
  int keydownmask;
  short movemask;
  SDL_Surface* icon;
  rr_subtimer resetTimer;
};

namespace rr_input{
  void setup();
  void cleanup();
  bool onEvent(SDL_Event* e);
  bool clearInputs();
  void joyreset();
  void writeKeybinds();
#ifdef RR_ACE
  void ace_clearall();
  void ace_assign(SDL_JoystickID);
  int ace_numJoysticks();
  std::shared_ptr<rr_inputsys> ace_getStick(int);
  void ace_setSWStick(std::shared_ptr<rr_inputsys>);
#endif
  std::shared_ptr<rr_inputsys> newKbd();
  std::shared_ptr<rr_inputsys> newJoy(int);
  bool setKbdRawEvent(bool);
  int pollKbdRawEvent(SDL_Event*);
  void tickReset();
  bool reqsReset();
}

#endif
