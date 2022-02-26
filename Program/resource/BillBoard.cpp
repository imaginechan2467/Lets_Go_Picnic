
#include <d3dx9.h>
#include "mydirect3D.h"
#include "texture.h"
#include "BillBoard.h"

typedef struct BillBoardVertex_tag
{
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;
}BillBoardVertex;

#define FVF_BILLBOARD (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

static D3DXMATRIX g_mtxView;

void BillboardInit(void)
{
	D3DXMatrixIdentity(&g_mtxView);
}

void BillboardUninit(void)
{

}

void BillboardDraw(int textureID, vec3f pos)
{
	const BillBoardVertex v[] =
	{
		{{-0.5f, 0.5f,1.0f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,0.0f}},
		{{ 0.5f, 0.5f,1.0f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,0.0f}},
		{{-0.5f,-0.5f,1.0f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,1.0f}},
		{{ 0.5f,-0.5f,1.0f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,1.0f}},
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxW, mtxT, mtxS;
	D3DXMATRIX mtxInvView = g_mtxView;

	//平行移動成分0
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;
	//D3DXMatrixInverse(&mtxInvView, NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	mtxW = mtxInvView * mtxT;//スケールならinvか移動の前にかける。

	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
	pDevice->SetFVF(FVF_BILLBOARD);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(BillBoardVertex));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void BillboardDrawforTex(vec3f setpos, vec3f setsize,
	int textureID, int frame, D3DCOLOR setcolor, bool isZtestTrue)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//ポリゴンのサイズはテクスチャサイズを計算
	float w = (float)TextureGetTextureSize(textureID, 'W');
	float h = (float)TextureGetTextureSize(textureID, 'H');
	Int2 FrameMAX = Texture_GetFrameMax(textureID);

	//分割するテクスチャの一つのサイズを計算
	float TexOnePieceX = (w / FrameMAX.x);
	float TexOnePieceY = (h / FrameMAX.y);

	//現在のフレームの場所を計算
	int CurrentFramePosX = frame % FrameMAX.x;
	int CurrentFramePosY = frame / FrameMAX.x;

	//UVpos計算
	float u0 = ((TexOnePieceX * CurrentFramePosX)) / w;
	float v0 = ((TexOnePieceY * CurrentFramePosY)) / h;
	float u1 = ((TexOnePieceX * CurrentFramePosX) + TexOnePieceX) / w;
	float v1 = ((TexOnePieceY * CurrentFramePosY) + TexOnePieceY) / h;

	BillBoardVertex v[] =
	{
		{{-0.5f, 0.5f,0.0f},setcolor,vec2f(u0,v0)},
		{{ 0.5f, 0.5f,0.0f},setcolor,vec2f(u1,v0)},
		{{-0.5f,-0.5f,0.0f},setcolor,vec2f(u0,v1)},
		{{ 0.5f,-0.5f,0.0f},setcolor,vec2f(u1,v1)},
	};

	D3DXMATRIX mtxW, mtxT, mtxS;
	D3DXMatrixTranslation(&mtxT, setpos.x, setpos.y, setpos.z);
	D3DXMatrixScaling(&mtxS, setsize.x, setsize.y, setsize.z);
	mtxW = mtxS * mtxT;
	pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	D3DXMATRIX mtxInvView = g_mtxView;

	//平行移動成分0
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;
	//D3DXMatrixInverse(&mtxInvView, NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixTranslation(&mtxT, setpos.x, setpos.y, setpos.z);
	mtxW = mtxInvView * mtxT;//スケールならinvか移動の前にかける。

	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
	pDevice->SetFVF(FVF_BILLBOARD);
	pDevice->SetTexture(0, Texture_GetTexture(textureID));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(BillBoardVertex));
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void BillBoardSetViewMatrix(const D3DXMATRIX& mtxView)
{
	g_mtxView = mtxView;
}


