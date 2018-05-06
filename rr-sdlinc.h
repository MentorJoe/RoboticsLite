/*
 * To externalize the inclusion of the SDL Headers in
 * its own file.
 */
#ifndef _rr_sdlinc_h_
#define _rr_sdlinc_h_

#if RR_SDL_VER == 1
#include<SDL/SDL.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_endian.h>
#else
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_endian.h>
#endif

#endif
