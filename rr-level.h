/* -*- C++ -*- */
#ifndef _rr_level_h_
#define _rr_level_h_

#include"rr-player.h"
#include"rr-entity.h"
#include"rr-hud.h"
// #include"rr-level.h" // ಠ_ಠ
#include"rr-renderable.h"
#include"rr-hscorefile.h"

#include<vector>
#include<memory>
#include<map>

#define LEVELSTATE_CONFIG 0x01
#define LEVELSTATE_WAITING 0x02
#define LEVELSTATE_COUNTDOWN 0x03
#define LEVELSTATE_PSEUDOAUTO 0x04
#define LEVELSTATE_TELEOP 0x05
#define LEVELSTATE_POSTGAME 0x06
#define LEVELSTATE_DONE 0x07

/*
 * Explanation of the Level States:
 *
 * Config: assigning players to team.
 *  -- func setup() --
 * Waiting: board is set, waiting for players to ready up.
 * Countdown: Self-explanatory
 *  -- func onGameStart() --
 * Pseudoauto: First 15 seconds, scoring simulates Autonomous
 * Teleop: Teleop Period
 *  -- func endGame() --
 * Postgame: Showing the post-game score breakdown.
 *  -- func cleanup() --
 * Done: Level Object is ready to be destroyed.
 *  -- func isDone() returns true, main loop destroys level instance --
 */

enum eLevelConfiguration{
  NIL,
  SOLO,
  FOURFORALL,
  TEAMBATTLE  
};

int getTileWallSides(unsigned char);

class rr_level:public rr_renderable{
public:
  rr_level();
  virtual ~rr_level();
  virtual void render(SDL_Surface*);
  int testLevel(int);
  virtual void logicTick();
  virtual void onSecond();
  virtual void onGameStart();
  virtual void setup();
  virtual void endGame();
  virtual void cleanup();
  virtual void writeLevelDataDemo(FILE*);
  void logicTick(bool);
  bool isDone();
  void addGameHud(std::shared_ptr<rr_hud>);
  void addInput(std::shared_ptr<rr_inputsys>);
  bool requestsUndeterminedInput();
  bool onUndeterminedInput(SDL_Event*);
  int getWidth();
  int getHeight();
  void addEntity(std::shared_ptr<rr_entity>);
  void record(const char*);

  virtual int itemSpriteIndex();
  
  /*
   * Loads a level.
   *
   * Returns:
   *  + 0 on success.
   *  + 1 on no action (LAN level)
   *  + a negative number on various failures
   */
  virtual int load(const char*);
  /*
   * Does collission detection of the map.
   *
   * Passed to player and entity as a pointer in the logicTick function.
   *
   * Returns:
   *  + 0 if Movement into the square is impossible.
   *  + 1 if Movement into the square is possible.
   *  + 2 if Movement into the square would requre an AI Pathfinding Decision
   */
  int collissionDetect(int,int,int,int);
  int tileCollissionDetect(int,int,int);
  int playerCollissionDetect(int,int,int,int);
  int getState();
#ifdef RR_ACE
  bool doBkgSlideshow();
  void ace_addJoystick(std::shared_ptr<rr_inputsys>);
#endif
protected:
  char* tilemap = NULL;
  char* tilesubdata = NULL;
  eLevelConfiguration levelconf;
  int width = 0;
  int height = 0;
  std::shared_ptr<rr_team> losRojos;
  std::shared_ptr<rr_team> losAzuls;
  int numKbdInputs = 0;
  int numJoyInputs = 0;
  std::vector< std::shared_ptr<rr_inputsys> > lInputs;
  std::vector<int> lInputSides;
  std::vector< std::shared_ptr<rr_player> > playerz;
  std::vector< std::shared_ptr<rr_hud> > hudz;
  std::vector< std::shared_ptr<rr_entity> > entitiez;
  unsigned long ticks = 0; // Probably don't need to be an unsigned long, but it doesn't hurt to be prepared
  int leveltime = 0;
  int countdown = 3;
  int lState = 0;
  int redScore = 0;
  int bluScore = 0;
  int rReadyUp = 0;
  int bReadyUp = 0;
#ifdef RR_ACE
  int ace_ctrlrs_enabled = 0;
#endif
  int inHighScoreMode = 0;
  rr_highScoreDisplay* hsDisplay = NULL;
  char* hsinputname = NULL;
  int hsinputcursor0;
  int hsinputcursor1;
};

#endif
