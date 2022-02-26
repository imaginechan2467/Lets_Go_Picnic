#ifndef TEXTURE_H_
#define TEXTURE_H_

#include<d3d9.h>
#include<d3dx9.h>
#include "Config.h"
#define TEXTURE_INVALID_ID (-1)
#define TEXTURE_FILENAME_MAX (64)

void TextureINIT(void);
void TextureUNINIT(void);

int SetTexture(const char* pFileName, Int2 Cut = { 1,1 });
int TextureLoad(const char* text);

void TextureRelease(int textureIds[], int count);
void TextureAllRelease(void);

float TextureGetTextureSize(int textureId, char WorH);
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId);
Int2 Texture_GetFrameMax(int textureId);

#endif