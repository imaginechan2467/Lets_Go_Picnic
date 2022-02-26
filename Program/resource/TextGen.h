#ifndef TEXTGEN_H_
#define TEXTGEN_H_

#include<d3dx9.h>
#include"Config.h"

#define TEXTSIZE_NORMAL (vec2f(16.0f,32.0f))
#define TEXTSIZE_HIRAKANA_NORMAL (vec2f(16,32))
#define TEXTSIZE_BIG (vec2f(64,128))
#define TEXTPOS_DEBUG(text,textsize)(vec2f(SCREEN_WIDTH - strlen(text) * textsize.x,0.0f))

void TextGenInit(void);

void TextGenUninit(void);

void TextGenUpdate(void);

void TextGenforEng(vec2f pos, vec2f size,
	const char* text, D3DCOLOR color = COLOR_NORMAL);

void TextGenforJp(vec2f pos, vec2f size,
	const char* text, D3DCOLOR color = COLOR_NORMAL);

void TextGenDrawforSecond(void);

void TextGenSetTextforEng(const char* text, int subAlphaSpeed = 1,
	vec2f size = TEXTSIZE_NORMAL, bool isUseAlpha = true,
	D3DCOLOR color = COLOR_NORMAL);

#endif