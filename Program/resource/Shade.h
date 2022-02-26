#ifndef SHADE_H_
#define SHADE_H_

#include <d3dx9.h>
#include "Config.h"

#define SHADE_DEFAULT_SIZE (vec2f(1.0f,1.0f))

void ShadeInit(void);
//void ShadeDraw(D3DXVECTOR2 pos, D3DXVECTOR2 size);
void ShadeDraw(vec3f pos, D3DXVECTOR2 size);
void ShadeUninit(void);

#endif