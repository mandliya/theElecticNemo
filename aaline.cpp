#include <algorithm>
#include <cmath>
#include <SDL.h>
#include <stdio.h>
#include "aaline.h"

void Draw_Pixel(SDL_Surface* s, int x, int y, 
                uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	Uint8 c_r, c_g, c_b, c_a;
	Uint8* buf8;
	Uint32* buf32;

	//In range?
	if(x < 0 || y < 0 || x >= s->w || y >= s->h)
		return;

	switch(s->format->BytesPerPixel){
		case 3:
			buf8 = (Uint8*)s->pixels + y*s->pitch + x;
			c_r = *(buf8+s->format->Rshift/8);
			c_g = *(buf8+s->format->Gshift/8);
			c_b = *(buf8+s->format->Bshift/8);
			*(buf8+s->format->Rshift/8) = (r*a/255) + (c_r*(255-a)/255);
			*(buf8+s->format->Gshift/8) = (g*a/255) + (c_g*(255-a)/255);
			*(buf8+s->format->Bshift/8) = (b*a/255) + (c_b*(255-a)/255);
			break;

		case 4:
			buf32 = (Uint32 *)s->pixels + y*s->pitch/4 + x;
			SDL_GetRGBA(*buf32, s->format, &c_r, &c_g, &c_b, &c_a);
			r = (r*a) / 255 + (c_r*(255-a)) /255;
			g = (g*a) / 255 + (c_g*(255-a)) /255;
			b = (b*a) / 255 + (c_b*(255-a)) /255;
			*buf32 = SDL_MapRGBA(s->format, r, g, b, 255);
			break;

		default:
			//Unsupported bit depth...
			break;
	}

	return;
}

void Draw_AALine(SDL_Surface* screen, float x0, float y0, 
                 float x1, float y1, float thick,
                 uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	float lower, upper;
	float delta;
	//Thickness less than screen diagonal
	delta = sqrtf(screen->w*screen->w + screen->h*screen->h) + 1.0f;
	if(thick > delta){
		thick = delta;
	}

	//Cap ends off, X dimension
	lower = 0.0f - thick;
	upper = screen->w + thick;
	x0 = std::min(upper, std::max(lower, x0));
	x1 = std::min(upper, std::max(lower, x1));

	//Cap ends off, Y dimension
	lower = 0.0f - thick;
	upper = screen->h + thick;
	y0 = std::min(upper, std::max(lower, y0));
	y1 = std::min(upper, std::max(lower, y1));

	//Steeper than shallow?
	if(abs(y1-y0) > abs(x1-x0)){
		//Re-order to go bottom to top
		if(y0 > y1){
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		//Adjust thickness by slope factor
		thick = thick * sqrtf(float(x1-x0)*(x1-x0) + float(y1-y0)*(y1-y0)) / float(y1-y0);

		//Calculate lower/upper/delta wrt X
		delta = float(x1-x0) / float(y1-y0);
		lower = x0 - thick*0.5f;
		upper = x0 + thick*0.5f;

		//Loop over each scanline, draw
		for(int y=y0; y<=y1; y++){
			int rl = int(lower);
			int ru = int(upper);
			float pl = 1.0f - (lower - rl);
			float pu = upper - ru;

			//Plot edge then interior pixels
			Draw_Pixel(screen, rl, y, r, g, b, a*pl);
			Draw_Pixel(screen, ru, y, r, g, b, a*pu);
			for(int x=rl+1; x<ru; x++){
				Draw_Pixel(screen, x, y, r, g, b, a);
			}

			//Update bounds
			lower += delta;
			upper += delta;
		}

		//Draw caps
		if(x0 != x1){
			float deltal, deltau;

			//Reset bounds
			lower = x1 - thick*0.5f;
			upper = x1 + thick*0.5f;

			//Choose which gets different delta
			if(x0 < x1){
				deltal = delta;
				deltau = -1.0f / delta;
			} else{
				deltal = -1.0f / delta;
				deltau = delta;
			}

			//Update bounds
			lower += deltal;
			upper += deltau;

			//Draw end cap
			for(int y=y1+1; lower<upper; y++){
				int rl = int(lower);
				int ru = int(upper);
				float pl = 1.0f - (lower - rl);
				float pu = upper - ru;

				//Plot edge then interior pixels
				Draw_Pixel(screen, rl, y, r, g, b, a*pl);
				Draw_Pixel(screen, ru+1, y, r, g, b, a*pu);
				for(int x=rl+1; x<=ru; x++){
					Draw_Pixel(screen, x, y, r, g, b, a);
				}

				//Update bounds
				lower += deltal;
				upper += deltau;
			}

			//Reset bounds
			lower = x0 - thick*0.5f;
			upper = x0 + thick*0.5f;

			//Update bounds
			std::swap(deltal, deltau);
			lower -= deltal;
			upper -= deltau;

			//Draw start cap
			for(int y=y0-1; lower<upper; y--){
				int rl = int(lower);
				int ru = int(upper);
				float pl = 1.0f - (lower - rl);
				float pu = upper - ru;

				//Plot edge then interior pixels
				Draw_Pixel(screen, rl, y, r, g, b, a*pl);
				Draw_Pixel(screen, ru+1, y, r, g, b, a*pu);
				for(int x=rl+1; x<=ru; x++){
					Draw_Pixel(screen, x, y, r, g, b, a);
				}

				//Update bounds
				lower -= deltal;
				upper -= deltau;
			}
		}

		return;
	}

	//Must have been more shallow
	//Reorder left to right
	if(x0 > x1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	//Adjust thickness by slope factor
	thick = thick * sqrtf(float(x1-x0)*(x1-x0) + float(y1-y0)*(y1-y0)) / float(x1-x0);

	//Calculate lower/upper/delta wrt Y
	delta = float(y1-y0) / float(x1-x0);
	lower = y0 - thick*0.5f;
	upper = y0 + thick*0.5f;

	//Loop over each scanline, draw
	for(int x=x0; x<=x1; x++){
		int rl = int(lower);
		int ru = int(upper);
		float pl = 1.0f - (lower - rl);
		float pu = upper - ru;

		//Plot edge pixels
		Draw_Pixel(screen, x, rl, r, g, b, a*pl);
		Draw_Pixel(screen, x, ru+1, r, g, b, a*pu);
		for(int y=rl+1; y<=ru; y++){
			Draw_Pixel(screen, x, y, r, g, b, a);
		}

		//Update bounds
		lower += delta;
		upper += delta;
	}

	//Draw caps
	if(y0 != y1){
		float deltal, deltau;

		//Reset bounds
		lower = y1 - thick*0.5f;
		upper = y1 + thick*0.5f;

		//Choose which gets different delta
		if(y0 < y1){
			deltal = delta;
			deltau = -1.0f / delta;
		} else{
			deltal = -1.0f / delta;
			deltau = delta;
		}

		//Update bounds
		lower += deltal;
		upper += deltau;

		//Draw end cap
		for(int x=x1+1; lower<upper; x++){
			int rl = int(lower);
			int ru = int(upper);
			float pl = 1.0f - (lower - rl);
			float pu = upper - ru;

			//Plot edge then interior pixels
			Draw_Pixel(screen, x, rl, r, g, b, a*pl);
			Draw_Pixel(screen, x, ru+1, r, g, b, a*pu);
			for(int y=rl+1; y<=ru; y++){
				Draw_Pixel(screen, x, y, r, g, b, a);
			}

			//Update bounds
			lower += deltal;
			upper += deltau;
		}

		//Reset bounds
		lower = y0 - thick*0.5f;
		upper = y0 + thick*0.5f;

		//Update bounds
		std::swap(deltal, deltau);
		lower -= deltal;
		upper -= deltau;

		//Draw start cap
		for(int x=x0-1; lower<upper; x--){
			int rl = int(lower);
			int ru = int(upper);
			float pl = 1.0f - (lower - rl);
			float pu = upper - ru;

			//Plot edge then interior pixels
			Draw_Pixel(screen, x, rl, r, g, b, a*pl);
			Draw_Pixel(screen, x, ru+1, r, g, b, a*pu);
			for(int y=rl+1; y<=ru; y++){
				Draw_Pixel(screen, x, y, r, g, b, a);
			}

			//Update bounds
			lower -= deltal;
			upper -= deltau;
		}
	}

	return;
}

void Draw_AALine(SDL_Surface* screen, float x0, float y0, float x1, float y1, float thick, uint32_t color){
	Uint8 r, g, b;
	SDL_GetRGB(color, screen->format, &r, &g, &b);
	Draw_AALine(screen, x0+0.5f, y0+0.5f, x1+0.5f, y1+0.5f, thick, r, g, b, 0xFF);
}

void Draw_AALine(SDL_Surface* screen, float x0, float y0, float x1, float y1, uint32_t color){
	Uint8 r, g, b;
	SDL_GetRGB(color, screen->format, &r, &g, &b);
	Draw_AALine(screen, x0+0.5f, y0+0.5f, x1+0.5f, y1+0.5f, 1.0f, r, g, b, 0xFF);
}

