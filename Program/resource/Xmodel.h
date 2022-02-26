#ifndef XMODEL_H_
#define XMODEL_H_

#include <d3dx9.h>

#define XMODEL_INVALID_ID (-1)//無効な番号

void XModelInit(void);

//戻り値は整理番号。
int XModelLoad(const char* pFileName);

void XModelUninit(void);

//pID...開放したい整理番号が入ったメモリ領域の先頭アドレス
//cnt...開放したいモデル整理番号の個数
void XModelRelease(const int* pID,int cnt);

void XModelReleaseAll(void);

void XModelDraw(int id, D3DXMATRIX* mtxModel);

#endif