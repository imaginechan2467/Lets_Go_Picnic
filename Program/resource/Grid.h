#ifndef GRID_H_
#define GRID_H_

#include<d3dx9.h>

//grid	�O���b�h�T�C�Y
//size	��ӂ̃O���b�h��
void GridInit(float size, float count, D3DCOLOR color);
void GridDraw(void);
void GridUninit(void);

#endif