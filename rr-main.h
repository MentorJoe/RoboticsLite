/* -*- C++ -*- */

#ifndef _rr_main_h_
#define _rr_main_h_

#include "rr-sdlinc.h"

/*
 * Values for the State Pointer
 */
#define STATE_INITIALIZED   0x0001
#define STATE_CLOSED        0x0002
#define STATE_CRASHED       0x0004
#define STATE_BREAK         0x0010
#define STATE_FULLSCREEN    0x0020
#define STATE_CONSOLE       0x0100
#define STATE_ALWAYSCONSOLE 0x0200
#define STATE_PAUSED        0x0400
#define STATE_KSTAT         0x1000
#define STATE_JSTAT         0x2000
#define STATE_ALWAYSDEMO    0x4000

#ifdef DEBUG
#define STATE_DBG_FREEZEAI 0x00010000
#endif

#include<string.h>
#include<strings.h>

#define strStartsWith(pre,str)(strlen(str) < strlen(pre) ? false : strncmp(pre,str,strlen(pre)) == 0)

#ifdef _WIN32
#define bzero(b,len)(memset((b),'\0',(len)),(void) 0)
#define bcopy(b1,b2,len)(memmove((b2),(b1),(len)),(void) 0)
#endif

extern char* g_crash_msg;
extern const char* RL_VERNUM;
extern const char* RL_VERNAM;

/*
 * Main C/C++ function of program.
 */
int main(int,char**);

/*
 * "Engine Initialization"
 *
 * Arguments:
 *  + int* stateptr: The State Pointer
 */
SDL_Surface* ngn_init(int*);

/*
 * The Main Loop
 *
 * Arguments:
 *  + SDL_Surface* sf: The Screen Surface
 *  + int* stateptr: The State Pointer
 */
void mainloop(SDL_Surface*,int*);

/*
 * Engine Close
 *
 * Arguments:
 *  + SDL_Surface* sf: The Screen Surface
 *  + int* stateptr: The State Pointer
 */
void ngn_close(SDL_Surface*,int*);

/*
 * "Sans Engine Crash Message"
 *
 * I don't know why. I usually name a method of this sort <CENSOR>,
 * and figured that because this is a source code that kids might
 * be looking at, I'd want to not make teachers, coaches, and
 * mentors angry at me for being the foulmouthed Nord that I usually
 * am.
 *
 * Arguments:
 *  + msg: The message to display
 */
void sansngncrshmsg(const char*);

/*
 * Displays a message and sets the engine into crashed mode.
 *
 * If the engine is not initialized, then calls `sansngncrshmsg`
 *
 * Arguments:
 *  + Stateptr
 *  + Message
 */
void crashmsg(int*,const char*);

// From http://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php
class rr_subtimer{
public:
  rr_subtimer();
  void start();
  void stop();
  void pause();
  void unpause();

  Uint32 getTicks();

  bool esStarted();
  bool esPaused();
private:
  Uint32 startTicks;
  Uint32 pausedTicks;
  bool isPaused;
  bool isStarted;
};

#endif

/*
 * Stupid things said during the YouTubing Adventures of TOMC:
 *
 * "If our choices have made any decisions thus so far"
 * "... This absolute cesspool of the worst the internet has to offer at times."
 */
