
//--------------------
//DIRECT3Dの制御
//--------------------

#ifndef MYDIRECT3D_H_
#define MYDIRECT3D_H_

#include<d3d9.h>

//Direct3D関連の初期化
//
//戻り値　初期化に失敗したらfalse
//
bool MyDirect3D_INIT(HWND hWnd);


//Direct3D関連の終了処理
//
void MyDirect3D_FIN(void);

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif//MYDIRECT3D_H_