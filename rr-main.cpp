#include<stdio.h>
#include<stdlib.h>

#include"rr-sdlinc.h"
#include"rr-stdinc.h"

#include<fstream>
#include<sstream>

#ifndef SDL_MAJOR_VERSION
#error "SDL appears to not be included."
#endif

#if SDL_MAJOR_VERSION == 1
#error "Only compileable with SDL2"
#endif

char* g_crash_msg = NULL;

#ifdef RR_ACE
const char* RL_VERNUM = "Beta 0.2.2-ACE";
#else
const char* RL_VERNUM = "Beta 0.2.2";
#endif
const char* RL_VERNAM = "What can `make clean` do for you?";

const int FPS = 30;
const int TPF = 1000 / FPS;

#if SDL_MAJOR_VERSION == 2
/*
 * Planning on compiling for SDL2 and only SDL2, but including failsafes
 * in case somebody tries to compile it with SDL1
 */
SDL_Window* g_window;
#endif


char* g_stdout_buf = NULL;
char* g_stderr_buf = NULL;
char* g_command = NULL;

Uint16 wicopxls[16*16] = { // Red Player Icon facing Right
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xffff,0xffff,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xffff,0xf000,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xffff,0xf000,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xffff,0xffff,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xf888,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0x0000,
  0x0000,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

int main(int argc,char* args[]){
  // Setting stdout & stderr buffers to a readable area in memory.
  g_stdout_buf = new char[BUFSIZ];
  g_stderr_buf = new char[BUFSIZ];
  g_command = new char[1024];
  bzero(g_stdout_buf,sizeof(char)*BUFSIZ);
  bzero(g_stderr_buf,sizeof(char)*BUFSIZ);
  bzero(g_command,sizeof(char)*1024);
  fflush(stdout);
  fflush(stderr);
  // I CAN'T BELIEVE I FORGOT THESE!
  setbuf(stdout,g_stdout_buf);
  setbuf(stderr,g_stderr_buf);
#ifdef DEBUG
  fprintf(stderr,"Top of program, start of Init code\n");
  fflush(stderr);
#endif
  printf("Robotics Lite %s \"%s\"\n",RL_VERNUM,RL_VERNAM);
  try{ // Java habits ...
    int state = 0;
    SDL_Surface* sf = ngn_init(&state);
    state |= STATE_INITIALIZED;
    mainloop(sf,&state);
    ngn_close(sf,&state);
  }
  catch(int e){ // TODO add block for standard exceptions
    
  }
  catch(...){
    sansngncrshmsg("An Unknown Exception has occoured in the initialization");
  }
  if(g_crash_msg != NULL)
    delete g_crash_msg;
  delete g_command;
}

void mainloop(SDL_Surface* sf,int* stateptr){
#ifdef DEBUG
  printf("Entering main loop\n");
#endif
  rr_subtimer tick_cap_timer;
  char consoleBuffer[32][256];
  char consoleCBuffer[32];
  int consoleCursorPos = 0;
  bzero(consoleBuffer,sizeof(char)*32*256);
  bzero(consoleCBuffer,sizeof(char)*32);
  char* consoleLine = new char[256];
  rr_input::setup();
#ifdef RR_ACE
  rr_renderable* renderableBackview = NULL;
  /*
  std::vector<std::string> idleshowvector;
  std::vector<std::string>::iterator idleiterator;
  */
  std::vector<rr_renderable*> idleshowvector;
  std::vector<rr_renderable*>::iterator idleiterator;
  rr_level* activeLevel = new rr_level_powerup(0);
  rr_menu* activeMenu = NULL;
  for(int i=0;i<rr_input::ace_numJoysticks();i++){
    activeLevel->ace_addJoystick(rr_input::ace_getStick(i));
  }
  std::ifstream infile("slides.txt");
  char* schbarf = new char[512];
  if(!infile.fail()){
    bzero(schbarf,sizeof(char)*512);
    while(!infile.fail() || !infile.eof()){
      infile.getline(schbarf,100);
      if(strlen(schbarf) != 0 && !strStartsWith("#",schbarf)){
	// idleshowvector.push_back(std::string(schbarf));
	if(strStartsWith("img:",schbarf))
	  idleshowvector.push_back(new rr_slideshowpane(schbarf));
	else if(strcmp("rrx:highscores",schbarf) == 0)
	  idleshowvector.push_back(new rr_highScoreDisplay());
      }
    }
  }
  else{
    idleshowvector.push_back(new rr_highScoreDisplay());
  }
  delete schbarf;
  idleiterator = idleshowvector.begin();
#else
  rr_renderable* renderableBackview = NULL;
  rr_level* activeLevel = NULL;
  rr_menu* activeMenu = new rr_mainmenu();
#endif
  bzero(consoleLine,sizeof(char)*256);
  while((*stateptr & STATE_BREAK) != STATE_BREAK){
    tick_cap_timer.start();
#ifdef RR_ACE
    if(renderableBackview == NULL){
      /*
      if(strStartsWith("img:",idleiterator->c_str()))
	renderableBackview = new rr_slideshowpane(idleiterator->c_str());
      else if(strcmp("rrx:highscores",idleiterator->c_str()) == 0)
	renderableBackview = new rr_highScoreDisplay();
      */
      renderableBackview = *idleiterator;
      renderableBackview->update();
    }
#endif
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){ // Exit Button or Ctrl-C in terminal.
#ifdef DEBUG
	fprintf(stderr,"SDL_Quit handeled\n");
#endif
	*stateptr ^= STATE_BREAK;
      }
      else if((*stateptr & STATE_CONSOLE) == STATE_CONSOLE && // If console mode is active
	      e.type == SDL_TEXTINPUT && // It's a text-input event
	      e.text.text[0] != '`'){ // And it was not the enable/disable console key
	strcat(consoleLine,e.text.text);
	consoleCursorPos += strlen(e.text.text); // TODO make this UTF-8 aware instead of ASCII
	// (At least my understanding of how strlen works is string of bytes instead of string
	// of characters that may or may not be multibyte... because encodings.)
	//
	// (Jeez, i18n is going to be a difficult thing to do)
      }
      else if(e.type == SDL_JOYAXISMOTION ||
	      e.type == SDL_JOYHATMOTION ||
	      e.type == SDL_JOYBUTTONDOWN ||
	      e.type == SDL_JOYBUTTONUP){
	if(!rr_input::onEvent(&e)){
	  if(activeMenu != NULL){
	    activeMenu->onInput(&e);
	  }
	  else if(activeLevel != NULL){
	    if(activeLevel->requestsUndeterminedInput()) activeLevel->onUndeterminedInput(&e);
	  }
	}
      }
      else if(e.type == SDL_KEYDOWN){
	if((*stateptr & STATE_CONSOLE) == STATE_CONSOLE){
	  // If we are in the console...
	  if(e.key.keysym.sym == SDLK_BACKQUOTE || e.key.keysym.sym == SDLK_ESCAPE){
	    // ... And somebody just hit a backtick or escape, then exit console
	    *stateptr &= ~STATE_CONSOLE;
	    SDL_StopTextInput();
	  }
	  else if(e.key.keysym.sym == SDLK_BACKSPACE){
	    if(consoleCursorPos > 0)
	      consoleLine[--consoleCursorPos] = 0;
	  }
	  else if(e.key.keysym.sym == SDLK_RETURN){
	    if(consoleCursorPos == 0){
	      // Exiting Console on an Empty Line
	      *stateptr &= ~STATE_CONSOLE;
	      SDL_StopTextInput();
	    }
	    else{
	      for(int i=30;i>-1;i--){
		// Append to buffer
		strcpy(consoleBuffer[i+1],consoleBuffer[i]);
		consoleCBuffer[i+1] = consoleCBuffer[i];
	      }
	      strcpy(consoleBuffer[0],consoleLine);
	      consoleCBuffer[0] = 0;
	      strcpy(g_command,consoleLine);
	      bzero(consoleLine,sizeof(char)*256);
	      consoleCursorPos = 0;
	    } // Console Command Entry
	  } // e.key.keysym.sym == SDLK_RETURN
	} // if In Console
	else if(e.key.keysym.sym == SDLK_BACKQUOTE){
	  // Enter console upon pressing a backtick
	  *stateptr |= STATE_CONSOLE;
#if SDL_MAJOR_VERSION == 2
	  SDL_StartTextInput();
	  continue;
#endif
	}
	else if(activeMenu != NULL){
	  activeMenu->onInput(&e);
	}
	else if(activeLevel != NULL && activeLevel->requestsUndeterminedInput()){
	  if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKQUOTE){
	    *stateptr |= STATE_CONSOLE;
#if SDL_MAJOR_VERSION == 2
	    SDL_StartTextInput();
	    continue;
#endif
	  }
	  else if(!rr_input::onEvent(&e)) activeLevel->onUndeterminedInput(&e);
	}
	else if(!rr_input::onEvent(&e)){
	  // If the input handler can't handle this.
	  switch(e.key.keysym.sym){
	  case SDLK_TAB: // For network multiplayer scores
	    break;
	  case SDLK_BACKQUOTE:
	    *stateptr |= STATE_CONSOLE;
#if SDL_MAJOR_VERSION == 2
	    SDL_StartTextInput();
	    continue;
#endif
	  case SDLK_ESCAPE:
	    *stateptr ^= STATE_PAUSED;
	    break;
	  case SDLK_F11:
	    // This breaks things on my laptop, apparently.
	    if(*stateptr & STATE_FULLSCREEN){
	      SDL_SetWindowFullscreen(g_window,0);
	      *stateptr &= ~STATE_FULLSCREEN;
	    }
	    else{
	      SDL_SetWindowFullscreen(g_window,SDL_WINDOW_FULLSCREEN);
	      *stateptr |= STATE_FULLSCREEN;
	    }
	    break; // TODO Fullscreen
	  }
	} // Extraneous Game-mode keys
      } // SDL_KEYDOWN processing
      else if(e.type == SDL_KEYUP && !rr_input::onEvent(&e)){
	// Shouldn't need anything here, but here's a keyup handler block
      }
    }
    // End Event Loop
#ifdef RR_ACE
    rr_input::tickReset();
    if(rr_input::reqsReset()){
      delete activeLevel;
      activeLevel = new rr_level_powerup(0);
      for(int i=0;i<rr_input::ace_numJoysticks();i++){
	activeLevel->ace_addJoystick(rr_input::ace_getStick(i));
      }
    }
#endif
    if(g_command[0] != 0){
      // Command entry
      // To allow menus to cue menu actions as commands
      if(strcmp(g_command,"exit") == 0 || strcmp(g_command,"quit") == 0){
#ifdef DEBUG
	fprintf(stderr,"Exiting by command.\n");
#endif
	*stateptr |= STATE_BREAK;
      }
      else if((*stateptr & STATE_CRASHED) != STATE_CRASHED){
	// Commands may not be run in crashed mode.
	if(strcmp(g_command,"powerup") == 0){
	  if(activeLevel != NULL) delete activeLevel;
	  if(activeMenu != NULL){
	    delete activeMenu;
	    activeMenu = NULL;
	  }
	  activeLevel = new rr_level_powerup(0);
	  if(*stateptr & STATE_ALWAYSDEMO)
	    activeLevel->record(NULL);
	}
	else if(strStartsWith("autorec ",g_command)){
	  if(g_command[8] == '1') *stateptr |= STATE_ALWAYSDEMO;
	  else if(g_command[8] == '0') *stateptr &= ~STATE_ALWAYSDEMO;
	}
	else if(strStartsWith("playdemo ",g_command)){
	  
	}
	else if(strcmp("mainmenu",g_command) == 0){
	  if(activeMenu != NULL) delete activeMenu;
	  activeMenu = new rr_mainmenu();
	}
#if defined(__unix__) && defined(RR_ACE) && defined(RL_ACE_ALLOWREBOOT)
	else if(strcmp("reboot",g_command) == 0 || strcmp("restart",g_command) == 0){
	  // This is probably the most dodgy bodgy idea I've ever thought.
	  //
	  // But hey, it does the job, since I think I set up sudo to allow the
	  // User that the game is running under to run reboot without
	  // entering their password
	  system("sudo reboot");
	}
	else if(strcmp("poweroff",g_command) == 0 || strcmp("shutdown",g_command) == 0){
	  system("sudo poweroff");
	}
#elif defined(_WIN32) && defined(RR_ACE) && defined(RL_ACE_ALLOWREBOOT)
	/*
	 * I don't know if this'll be needed.
	 *
	 * However, it's templated out for somebody to implement later, especially if
	 * you're on some embedded host of the sort.
	 */
	else if(strcmp("reboot",g_command) == 0 || strcmp("restart",g_command) == 0){
	  fprintf(stderr,"Reboot commands not implemented on Windows\n");
	}
	else if(strcmp("shutdown",g_command) == 0 || strcmp("poweroff",g_command) == 0){
	  fprintf(stderr,"Shutdown commands not implemented on Windows\n");
	}
#endif
#ifndef RR_ACE
	else if(strStartsWith("keybindeditor ",g_command) && activeLevel == NULL){
	  if(g_command[14] >= '0' && g_command[14] <= '3'){
	    if(activeMenu != NULL) delete activeMenu;
	    activeMenu = new rr_kbdmenu(g_command[14] & 0x03); // Converting ASCII to a number since 0095!
	  }
	  else{
	    fprintf(stderr,"Usage: keybindeditor <n>. Where <n> is the playerset number.");
	  }
	}
	else if(strcmp("settings",g_command) == 0 && activeLevel == NULL){
	  if(activeMenu != NULL) delete activeMenu;
	  activeMenu = new rr_optmenu();
	}
#endif
#ifdef RR_HAS_NET_MP
	// Network Multiplayer Commands TODO
	else if(strStartsWith("connect ",g_command)){
	  // Manually connects to a server
	}
	else if(strStartsWith("netmode ",g_command)){
	  // Switch between TCP (2), UDP (1), and Autoselect (0)
	}
	else if(strStartsWith("maxping ",g_command)){
	  // Sets the default maximum ping before auto disconnect
	}
#else
	else if(strStartsWith("connect ",g_command) || strStartsWith("netmode ",g_command) || strStartsWith("maxping ",g_command))
	  fprintf(stderr,"Network support not installed.\n");
#endif
#ifdef DEBUG
	// Debug-only Commands
	else if(strStartsWith("dlevelload ",g_command)){
	  // TODO
	  fprintf(stderr,"In dlevelload\n");
	  
	}
	else if(strStartsWith("dpowerup ",g_command)){
	  if(activeLevel != NULL) delete activeLevel;
	  activeLevel = new rr_level_powerup(0);
	}
	else if(strStartsWith("dlmpl ",g_command)){
	  // TODO
	}
	else if(strStartsWith("dmaptime ",g_command)){
	  // TODO
	}
	else if(strStartsWith("dlevcountdown ",g_command)){
	  
	  // TODO
	}
	else if(strStartsWith("dmenu ",g_command)){
	  // TODO
	}
	else if(strStartsWith("dfreezeai ",g_command) && strlen(g_command) == 11){
	  if(g_command[10] == '1') *stateptr |= STATE_DBG_FREEZEAI;
	  else if(g_command[10] == '0') *stateptr |= STATE_DBG_FREEZEAI;
	  else fprintf(stderr,"Usage: dfreezeai [0|1]\nHalt AI Tracking\n");
	}
	else if(strcmp("close",g_command) == 0){
	  if(activeLevel != NULL){
	    delete activeLevel;
	    activeLevel = NULL;
	  }
	}
#endif // Debug-only Commands
#ifdef RR_ACE
	else if(strStartsWith("reassign ",g_command)){
	  // Reassign controllers on the Arcade Cabinet Edition
	  // Syntax: reassign <1234>, where the numbers are the raw joystick numbers.
	  // They're initially assigned in the order they appear to the system
	  std::queue<std::shared_ptr<rr_inputsys>> requeue;
	  for(int i=0;i<4;i++){
	    if(g_command[9+i] == 0) break; // Avoid reading into a wrong area of memory
	    int j = g_command[9+i] & 0x0F; // Convert the char ascii to int
	    requeue.push(rr_input::ace_getStick(j));
	  }
	  rr_input::ace_clearall();
	  for(int i=0;i<4;i++){
	    rr_input::ace_setSWStick(requeue.front());
	    requeue.pop();
	  }
	  // Lastly reset the level state
	  if(activeLevel != NULL){
	    delete activeLevel;
	    activeLevel = new rr_level_powerup(0);
	    for(int i=0;i<4;i++){
	      activeLevel->ace_addJoystick(rr_input::ace_getStick(i));
	    }
	  }
	}
#endif // Arcade Cabinet Edition
      } // Is Not Crashed (Cmd Entry)
      bzero(g_command,sizeof(char)*1024);
    }
    // Begin Game Logic
    if(activeLevel != NULL){
      activeLevel->logicTick();
#ifdef RR_ACE
      if(activeLevel->isDone()){
	delete activeLevel;
	activeLevel = new rr_level_powerup(0);
	for(int i=0;i<rr_input::ace_numJoysticks();i++){
	  activeLevel->ace_addJoystick(rr_input::ace_getStick(i));
	}
      }
#else
      if(activeLevel->isDone()){
	delete activeLevel;
	activeLevel = NULL;
	rr_input::clearInputs();
	if(activeMenu == NULL){
	  activeMenu = new rr_mainmenu();
	}
      }
#endif
    }
    if(renderableBackview != NULL){
      renderableBackview->logicTick();
    }
    // Take Standard Output and Standard Error Buffers and put them in the console screen buffer
    if(g_stdout_buf[0] != 0){
      char* temp = new char[strlen(g_stdout_buf)+1];
      strcpy(temp,g_stdout_buf);
      char* token = strtok(temp,"\n");
      while(token != NULL){
	for(int i=30;i>-1;i--){
	  strcpy(consoleBuffer[i+1],consoleBuffer[i]);
	  consoleCBuffer[i+1] = consoleCBuffer[i];
	}
	strcpy(consoleBuffer[0],token);
	consoleCBuffer[0] = 1;
	token = strtok(NULL,"\n");
      }
      delete temp;
      fflush(stdout);
      bzero(g_stdout_buf,sizeof(char)*BUFSIZ);
    }
    if(g_stderr_buf[0] != 0){
      char* temp = new char[strlen(g_stderr_buf)+1];
      strcpy(temp,g_stderr_buf);
      char* token = strtok(temp,"\n");
      while(token != NULL){
	for(int i=30;i>-1;i--){
	  strcpy(consoleBuffer[i+1],consoleBuffer[i]);
	  consoleCBuffer[i+1] = consoleCBuffer[i];
	}
	strcpy(consoleBuffer[0],token);
	consoleCBuffer[0] = 2;
	token = strtok(NULL,"\n");
      }
      free(token);
      delete temp;
      fflush(stderr);
      bzero(g_stderr_buf,sizeof(char)*BUFSIZ);
    }
    // End Logic
    // Begin Render
    // Clear screen.
    SDL_FillRect(sf,NULL,SDL_MapRGB(sf->format,0,0,0));
#ifdef RR_ACE
    if(activeLevel == NULL && renderableBackview != NULL){
      // If there's no level but there's a renderable backview, then render the backview
      renderableBackview->render(sf);
    }
    else if(activeLevel != NULL && activeLevel->getState() == LEVELSTATE_CONFIG){
      // Level is waiting for controllers to ready up
      if(renderableBackview != NULL) renderableBackview->render(sf);
      activeLevel->render(sf);
    }
    else if(activeLevel != NULL)
      activeLevel->render(sf);
#else
    // Render backview if it exists and the level will permit it
    /*
    if(renderableBackview != NULL){
      renderableBackview->render(sf);
    }
    */
    
    // Render the level if there is a level.
    if(activeLevel != NULL){
      activeLevel->render(sf);
      if(activeMenu != NULL){
	// If there's a level and a menu up, then do something fancy between the level and the menu
	activeMenu->render(sf);
      }
    }
    else if(activeMenu != NULL) activeMenu->render(sf);
    // Else if there's no level, but there is a menu, then just render it
#endif
    
    SDL_Surface* tx = NULL;
    char* barf = new char[256];
    SDL_Rect blitdest;
    SDL_Color concolors[] = {
      {128,128,128},
      {255,255,255},
      {192,0,0}
    };
    TTF_Font* largeFont  = rr_ttf::load("ttf/djvm.ttf",16);
    TTF_Font* smallFont  = rr_ttf::load("ttf/profont.ttf",12);
    TTF_Font* sevsegFont = rr_ttf::load("ttf/7seg.ttf",32);

    if((*stateptr & STATE_PAUSED) == STATE_PAUSED){
      tx = TTF_RenderText_Blended(sevsegFont,"PAUSED",concolors[1]);
      blitdest.x = 24;
      blitdest.y = 24;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);
      tx = NULL;
    }
    if(
#ifdef DEBUG
       true
#else
       (*stateptr & STATE_CONSOLE) == STATE_CONSOLE ||
       (*stateptr & STATE_ALWAYSCONSOLE) == STATE_ALWAYSCONSOLE ||
       (*stateptr & STATE_CRASHED) == STATE_CRASHED
#endif
       ){
      if((*stateptr & STATE_CONSOLE) == STATE_CONSOLE){
	// In console indicator
	tx = TTF_RenderText_Solid(smallFont,">",concolors[1]);
	blitdest.x = 12;
	blitdest.y = sf->h - tx->h - 12;
	blitdest.w = tx->w;
	blitdest.h = tx->h;
	SDL_BlitSurface(tx,NULL,sf,&blitdest);
	SDL_FreeSurface(tx);
      }
      if(consoleLine[0] != 0){
	// Input Line
	tx = TTF_RenderText_Solid(smallFont,consoleLine,concolors[0]);
	blitdest.x = 24;
	blitdest.y = sf->h - tx->h - 12;
	blitdest.w = tx->w;
	blitdest.h = tx->h;
	SDL_BlitSurface(tx,NULL,sf,&blitdest);
	SDL_FreeSurface(tx);
      }
      if(consoleBuffer[0] != 0){
	for(int i=0;i<32;i++){
	  if(consoleBuffer[i][0] != 0){
	    tx = TTF_RenderText_Solid(smallFont,consoleBuffer[i],concolors[consoleCBuffer[i]]);
	    blitdest.x = 24;
	    blitdest.y = sf->h-(12*i)-36-tx->h;
	    blitdest.w = tx->w;
	    blitdest.h = tx->h;
	    SDL_BlitSurface(tx,NULL,sf,&blitdest);
	    SDL_FreeSurface(tx);
	  }
	}
      }
    }
#ifdef DEBUG
    // ==== Declaration of Debug Build ====
    sprintf(barf,"ROBOTICS LITE DEBUG BUILD %s",RL_VERNUM);
    tx = TTF_RenderText_Solid(smallFont,barf,concolors[1]);
    blitdest.x = sf->w - tx->w;
    blitdest.y = 12;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

    sprintf(barf,"\"%s\"",RL_VERNAM);
    tx = TTF_RenderText_Solid(smallFont,barf,concolors[1]);
    blitdest.x = sf->w - tx->w;
    blitdest.y = 24;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

#ifdef __cplusplus
    sprintf(barf,"Built %s %s, C++ %ld",__DATE__,__TIME__,__cplusplus);
#else
    sprintf(barf,"Built %s %s, C",__DATE__,__TIME__);
#endif
    tx = TTF_RenderText_Solid(smallFont,barf,concolors[1]);
    blitdest.x = sf->w - tx->w;
    blitdest.y = 48;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);

    sprintf(barf,"SDL version %d.%d.%d",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);
    tx = TTF_RenderText_Solid(smallFont,barf,concolors[1]);
    blitdest.x = sf->w - tx->w;
    blitdest.y = 60;
    blitdest.w = tx->w;
    blitdest.h = tx->h;
    SDL_BlitSurface(tx,NULL,sf,&blitdest);
    SDL_FreeSurface(tx);
#endif

    if((*stateptr & STATE_CRASHED) == STATE_CRASHED){
      tx = TTF_RenderText_Blended(sevsegFont,"DEADBEEF",{192,0,0});
      blitdest.x = sf->w/2-tx->w/2;
      blitdest.h = 36;
      blitdest.w = tx->w;
      blitdest.h = tx->h;
      SDL_BlitSurface(tx,NULL,sf,&blitdest);
      SDL_FreeSurface(tx);

      if(g_crash_msg != NULL){
	tx = TTF_RenderText_Solid(smallFont,g_crash_msg,{255,255,255});
	blitdest.x = 24;
	blitdest.y = 24;
	blitdest.w = tx->w;
	blitdest.h = tx->h;
	SDL_BlitSurface(tx,NULL,sf,&blitdest);
	SDL_FreeSurface(tx);
      }
    }
    SDL_UpdateWindowSurface(g_window);
    delete barf;
    // End Render
#ifdef RR_ACE
    if(activeLevel != NULL){
      if(renderableBackview->isDone()){
	// delete renderableBackview;
	renderableBackview = NULL;
	idleiterator++;
      }
      if(idleiterator == idleshowvector.end()) idleiterator = idleshowvector.begin();
    }
#endif
    int frameTicks = tick_cap_timer.getTicks();
    if(frameTicks < TPF){
      SDL_Delay(TPF - frameTicks);
    }
  }
  // End Main Loop
  delete consoleLine;
  rr_input::cleanup();
  rr_ttf::cleanup();
}

void sansngncrshmsg(const char* msg){
#ifdef _WIN32
  // I should maybe put something in here?
#else

#endif
}

SDL_Surface* ngn_init(int* stateptr){
#if SDL_MAJOR_VERSION == 2
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
  g_window = SDL_CreateWindow("Robotics Lite",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_SHOWN);
  SDL_Surface* ico = SDL_CreateRGBSurfaceFrom(wicopxls,16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);
  SDL_SetWindowIcon(g_window,ico);
  SDL_FreeSurface(ico);
  return SDL_GetWindowSurface(g_window);
#endif
}

void ngn_close(SDL_Surface* sf,int* stateptr){
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}

void crashmsg(int* stateptr,const char* msg){

}

rr_subtimer::rr_subtimer(){
  startTicks = 0;
  pausedTicks = 0;
  isPaused = false;
  isStarted = false;
}

void rr_subtimer::start(){
  isStarted = true;
  isPaused = false;
  startTicks = SDL_GetTicks();
  pausedTicks = 0;
}

void rr_subtimer::stop(){
  isStarted = false;
  isPaused = false;
  startTicks = 0;
  pausedTicks = 0;
}

void rr_subtimer::pause(){
  if(isStarted && !isPaused){
    isPaused = true;
    pausedTicks = SDL_GetTicks() - startTicks;
    startTicks = 0;
  }
}

void rr_subtimer::unpause(){
  if(isStarted && isPaused){
    isPaused = false;
    startTicks = SDL_GetTicks() - pausedTicks;
    pausedTicks = 0;
  }
}

Uint32 rr_subtimer::getTicks(){
  Uint32 time = 0;
  if(isStarted){
    if(isPaused)
      time = pausedTicks;
    else
      time = SDL_GetTicks() - startTicks;
  }
  return time;
}

bool rr_subtimer::esStarted(){
  return isStarted;
}

bool rr_subtimer::esPaused(){
  return isPaused;
}
