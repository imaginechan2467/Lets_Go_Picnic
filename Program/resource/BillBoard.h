#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include <d3dx9.h>
#include "Config.h"

void BillboardInit(void);
void BillboardUninit(void);
void BillboardDraw(int textureID, vec3f pos);
void BillboardDrawforTex(vec3f setpos, vec3f setsize,
	int textureID, int frame, D3DCOLOR setcolor, bool isZtestTrue = true);
void BillBoardSetViewMatrix(const D3DXMATRIX& mtxView);


#endif