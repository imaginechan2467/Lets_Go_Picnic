#ifndef XMODEL_H_
#define XMODEL_H_

#include <d3dx9.h>

#define XMODEL_INVALID_ID (-1)//�����Ȕԍ�

void XModelInit(void);

//�߂�l�͐����ԍ��B
int XModelLoad(const char* pFileName);

void XModelUninit(void);

//pID...�J�������������ԍ����������������̈�̐擪�A�h���X
//cnt...�J�����������f�������ԍ��̌�
void XModelRelease(const int* pID,int cnt);

void XModelReleaseAll(void);

void XModelDraw(int id, D3DXMATRIX* mtxModel);

#endif