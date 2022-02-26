/*==============================================================================

   [Config.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include"Config.h"
#include "mydirect3D.h"

#define OBJECTMAX (256)

typedef struct
{
	D3DPRIMITIVETYPE type;
	int primcnt;
	void * pvertex; 
	UINT asize; 
	float zpos;
	bool isUse;
}OBJECT;

static int g_objectNum = 0;
static OBJECT g_obj[OBJECTMAX] = {};//ちゃんとやるときは動的メモリ確保にする

void ConfigInit(void)
{
	g_objectNum = 0;
	memset(g_obj, 0, sizeof(g_obj));
}

//テクスチャ切り取りに使うInt2の値を設定する関数
Int2 MakeInt2(int x, int y)
{
	Int2 alpha;
	alpha.x = x;
	alpha.y = y;
	return alpha;
}

Int3 MakeInt3(int x, int y, int z)
{
	Int3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

void SetConfigDrawPrimitiveUp(D3DPRIMITIVETYPE type, int primcnt, void * pvertex, UINT asize,float zpos)
{
	if (!pvertex)
	{
		return;
	}

	for (int i = 0; i < OBJECTMAX; i++)
	{
		if (g_obj[i].isUse)continue;

		g_obj[i].type = type;
		g_obj[i].primcnt = primcnt;
		g_obj[i].pvertex = pvertex;
		g_obj[i].asize = asize;
		g_obj[i].zpos = zpos;

		break;
	}
}

void ConfigDrawPrimitiveUp(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	for (int i = 0; i < OBJECTMAX; i++)
	{
		if (!g_obj[i].isUse)continue;

		pDevice->DrawPrimitiveUP(g_obj[i].type, g_obj[i].primcnt, g_obj[i].pvertex, g_obj[i].asize);
	}
}

void ConfigZSort(void)
{
	for (int i = 0;; i++)
	{

	}
}


