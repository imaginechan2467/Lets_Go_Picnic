#ifndef GRID_H_
#define GRID_H_

#include<d3dx9.h>

//grid	グリッドサイズ
//size	一辺のグリッド数
void GridInit(float size, float count, D3DCOLOR color);
void GridDraw(void);
void GridUninit(void);

#endif