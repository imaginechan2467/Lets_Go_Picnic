
//--------------------
//DIRECT3D�̐���
//--------------------

#ifndef MYDIRECT3D_H_
#define MYDIRECT3D_H_

#include<d3d9.h>

//Direct3D�֘A�̏�����
//
//�߂�l�@�������Ɏ��s������false
//
bool MyDirect3D_INIT(HWND hWnd);


//Direct3D�֘A�̏I������
//
void MyDirect3D_FIN(void);

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif//MYDIRECT3D_H_