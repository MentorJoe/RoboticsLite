/* -*- C++ -*- */
#ifndef _rr_level_powerup_h_
#define _rr_level_powerup_h_

// Core Level Object
#include"rr-level.h"
// Core Hud Object (for Powerup Hud)
#include"rr-hud.h"

// Enum for the three powerups, to give them actual values instead of
// Just using arbritrary integers
enum rr_powerups{
  FORCE,
  LEVITATE,
  BOOST
};

// Struct for a powerup object, e.g. cue this powerup.
struct rr_powerup_pow{
  rr_powerups pow;
  unsigned char level;
  char team;
  unsigned char* lvsetr;
};

// Struct for a team's vault.
struct rr_powerup_set{
  unsigned char force;
  unsigned char levitate;
  unsigned char boost;
};

// Class for the powerup hud. (Unique to Powerup, therefore in the powerup
// Source file instead of the hud source file)
class powerupHud:public rr_hud{
public:
  powerupHud(rr_powerup_set*,int,int*);
  void render(SDL_Surface*);
private:
  rr_powerup_set* pset;
  int side;
  int* cubecount;
};

/*
 * The Actual Powerup Game Class
 *
 * Methods on this:
 *  + Constructor: Level Setup
 *  + Deconstructor: Cleanup
 *  + onGameStart(): Run at the end of the countdown, right before the
 *      game timer starts
 *  + onSecond(): Run every second (scoring)
 *  + logicTick(): Run every logic tick cycle of the game (movement & logic)
 *  + itemSpriteIndex(): The item sprite to look up and display
 *  + endGame(): Run at the end of the countdown, end-game scoring (climbing)
 * Everything else is powerup-specific.
 */
class rr_level_powerup:public rr_level{
public: // I should comment this
  rr_level_powerup(int); // So that emacs's Git Blame Mode
  ~rr_level_powerup(); // Actually indents these lines with the rest
  void onGameStart();
  void onSecond();
  void logicTick();
  int itemSpriteIndex();
  void endGame();
  bool powInQueue(int);
  bool cuePowerup(rr_powerup_pow*);
  void render(SDL_Surface*);
private:
  int RSwitchSway;
  int   ScaleSway;
  int BSwitchSway;
  int RInVault;
  int BTPortal;
  int BBPortal;
  int BInVault;
  int RTPortal;
  int RBPortal;
  rr_powerup_set rpow;
  rr_powerup_set bpow;
  unsigned int leveldata; // The Randomization of the Scales
  rr_powerup_pow activePowerup;
  rr_powerup_pow powerupInQueue;
  short powsInQueue;
  int powCountdown;
  short levitatePowerupUsed;
};

#endif
