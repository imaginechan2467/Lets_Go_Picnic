/*==============================================================================

   [Grid.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<d3dx9.h>
#include"mydirect3D.h"
#include"Config.h"
#include<stdlib.h>

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
}Vertex3D;

//*****************************************************************************
// マクロ
//*****************************************************************************
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static Vertex3D* g_pVertex = NULL;
static int g_VertexCount = 0;
static int g_LineCount = 0;

void GridInit(float size, float count, D3DCOLOR color)
{
	//ライン数　指定+1
	g_LineCount = (int)(count + 1) * 2;
	//頂点数
	g_VertexCount = g_LineCount * 2;

	//指定した頂点数文メモリ確保
	g_pVertex = (Vertex3D*)malloc(g_VertexCount * sizeof(Vertex3D));

	float length = size * count;//ライン1本の長さ
	float start = -length * 0.5f;//ラインの半分の長さ　半分にしないと２倍になる　下も同様
	float end   = length * 0.5f;
	int n = g_LineCount;//並行のラインの頂点数

	//ラインの描き始めの頂点と、描き終わりの頂点の位置を設定（縦のラインと横のライン）
	for (int i = 0; i < count + 1; i++)
	{
		g_pVertex[i * 2    ].Position = D3DXVECTOR3(start + size * i,0.0f, start);
		g_pVertex[i * 2 + 1].Position = D3DXVECTOR3(start + size * i,0.0f, end  );
		g_pVertex[n + i * 2    ].Position = D3DXVECTOR3(start, 0.0f, start + size * i);
		g_pVertex[n + i * 2 + 1].Position = D3DXVECTOR3(end  , 0.0f, start + size * i);
	}

	//色を設定
	D3DCOLOR centercolor = D3DCOLOR_RGBA(255, 0, 0, 255);
	for (int i = 0; i < g_VertexCount; i++)
	{
		g_pVertex[i].Color = color;

		if ((i == (g_VertexCount / 4) || i == (g_VertexCount / 4) - 1) ||
			(i == (g_VertexCount / 4) * 3 || i == (g_VertexCount / 4) * 3 - 1))
		{
			g_pVertex[i].Color = centercolor;
		}
	}
}

void GridDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//ワールド座標変換行列の作成
	D3DXMATRIX mtxWorld;
	//単位行列の作成
	D3DXMatrixIdentity(&mtxWorld);//mtxWorld = 1;
	//回転行列の作成
	//D3DXMatrixRotationY(&mtxWorld, g_angle);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//デバイスのFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX3D);//ローカルなFVF。テクスチャなし。

	//デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, NULL);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ポリゴンを描画する
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, g_LineCount, g_pVertex, sizeof(Vertex3D));
}


void GridUninit(void)
{
	free(g_pVertex);
	g_pVertex = NULL;
}
