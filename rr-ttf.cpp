

#include"rr-sdlinc.h"
#include"rr-ttf.h"

#include<sstream>
#include<string>
#include<stdio.h>

std::map<std::string,TTF_Font*> fontmap;

TTF_Font* rr_ttf::load(std::string fontname,int size){
  std::stringstream outStream;
  outStream << fontname << "|||" << size;
  std::string fontkey = outStream.str();
  
  if(fontmap.find(fontkey) == fontmap.end()){
    // Font is not cached, load it
    TTF_Font* font = TTF_OpenFont(fontname.c_str(),size);
    fontmap[fontkey] = font;
    return font;
  }
  else{
    return fontmap[fontkey];
  }
}

void rr_ttf::cleanup(){
  // Iterate over all loaded fonts and close them.
  std::map<std::string,TTF_Font*>::iterator it;
  for(std::map<std::string,TTF_Font*>::iterator it = fontmap.begin();
      it != fontmap.end();
      ++it){
    TTF_CloseFont(it->second);
  }
}
