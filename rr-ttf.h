/* -*- C++ -*- */
/*
 * Font loading management
 */
#ifndef _rr_ttf_h_
#define _rr_ttf_h_

#include<map>
#include<string>
#include"rr-sdlinc.h"

namespace rr_ttf{
  TTF_Font* load(std::string,int);
  void cleanup();
}

#endif
