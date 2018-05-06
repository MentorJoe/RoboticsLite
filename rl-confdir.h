/* -*- C++ -*- */
#ifndef _rl_confdir_h_
#define _rl_confdir_h_

#include<string>

namespace rl_confdir{
  /*
   * Gets the configuration directory, and creates it if it doesn't exist.
   *
   */
  std::string getConfDir();
  /*
   * Appends the Highscores File to the result of getConfDir()
   */
  std::string getHSFileName();
  /*
   * Appends the Keybind File Name to the result of getConfDir()
   */
  std::string getKeybindFileName();
}

#endif
