#include <stdint.h>
#include <SDL.h>

#ifndef _AALINE_H
#define _AALINE_H

void Draw_Pixel(SDL_Surface* s, int x, int y, 
                uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, float thick,
                 uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, float thick, uint32_t color);
void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, uint32_t color);

#endif

