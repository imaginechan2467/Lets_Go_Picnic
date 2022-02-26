/*==============================================================================

   [Facegen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/


//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<d3dx9.h>
#include"mydirect3D.h"
#include"Facegen.h"
#include"texture.h"
#include<vector>

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXVECTOR4 Position;
	D3DCOLOR Color;
	vec2f Texcord;
}Vertex2D;

typedef struct
{
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	vec2f Texcord;
}Vertex3D;

//*****************************************************************************
// マクロ
//*****************************************************************************
#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define CIRCLE_CUTE (32)
#define CIRCLE_NORMAL (16)
#define CIRCLE_MINI (8)

//*****************************************************************************
// 列挙型宣言
//*****************************************************************************
void CircleCute(vec2f pos, float R, int vertexNum, D3DCOLOR color);
void CircleHQ(vec2f pos, float R, D3DCOLOR color);

//*****************************************************************************
// プロトタイプ関数宣言
//*****************************************************************************
vec3f Getvec3ByFaceGenType(FACEGENTYPE type, int index);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static vec3f g_centerVertex[] =
{
	{{-0.5f, 0.5f,0.0f}},
	{{ 0.5f, 0.5f,0.0f}},
	{{-0.5f,-0.5f,0.0f}},
	{{ 0.5f,-0.5f,0.0f}},
};
static vec3f g_leftUpVertex[] =
{
	{{0.0f, 0.0f,0.0f}},
	{{1.0f, 0.0f,0.0f}},
	{{0.0f,-1.0f,0.0f}},
	{{1.0f,-1.0f,0.0f}},
};
static int g_WhitePoliTex;

//*****************************************************************************
// 関数定義
//*****************************************************************************
void FaceGenInit(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		MessageBox(NULL,"デバイス取得失敗","エラー",MB_OK);
		return;
	}

	g_WhitePoliTex = SetTexture("asset/texture/white.png");
	TextureLoad("ホワイト");
}

//左端中心
//テクスチャを切り取らない場合の関数
void FaceGen(vec2f pos, vec2f size, bool IsUseTex,
	int TexId, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ポリゴンのサイズはテクスチャサイズ
	float w = (float)TextureGetTextureSize(TexId, 'W');
	float h = (float)TextureGetTextureSize(TexId, 'H');

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 0.0f, 1.0f),Color,vec2f(0.0f,0.0f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 0.0f, 1.0f),Color,vec2f(1.0f,0.0f)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(0.0f,1.0f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(1.0f,1.0f)},
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//引数が最もシンプルなFaceGen。
void FaceGenSimple(vec2f pos, vec2f size, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	int TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 0.0f, 1.0f),Color,vec2f(0.0f,0.0f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 0.0f, 1.0f),Color,vec2f(1.0f,0.0f)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(0.0f,1.0f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(1.0f,1.0f)},
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void FaceGen3D(vec3f pos, vec2f scale, FACEGENTYPE type,
	bool IsUseTex, int TexId, float radian, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	Vertex3D v[] = {
	{Getvec3ByFaceGenType(type,0),Color,vec2f(0.0f,0.0f)},
	{Getvec3ByFaceGenType(type,1),Color,vec2f(1.0f,0.0f)},
	{Getvec3ByFaceGenType(type,2),Color,vec2f(0.0f,1.0f)},
	{Getvec3ByFaceGenType(type,3),Color,vec2f(1.0f,1.0f)},
	};

	D3DXMATRIX mtxR, mtxS, mtxT, mtxW;
	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&mtxS, scale.x, scale.y, 0.0f);
	D3DXMatrixRotationZ(&mtxR, radian);
	mtxW = mtxS * mtxR * mtxT;

	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex3D));
}

void FaceGenforTex3D(vec3f pos, vec2f scale, FACEGENTYPE type,
	bool IsUseTex, int TexId, int CurrentFrame, float radian, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ポリゴンのサイズはテクスチャサイズを計算
	float w = (float)TextureGetTextureSize(TexId, 'W');
	float h = (float)TextureGetTextureSize(TexId, 'H');
	Int2 FrameMAX = Texture_GetFrameMax(TexId);

	//分割するテクスチャの一つのサイズを計算
	float TexOnePieceX = (w / FrameMAX.x);
	float TexOnePieceY = (h / FrameMAX.y);

	//現在のフレームの場所を計算
	int CurrentFramePosX = CurrentFrame % FrameMAX.x;
	int CurrentFramePosY = CurrentFrame / FrameMAX.x;

	//UVpos計算
	float u0 = ((TexOnePieceX * CurrentFramePosX)) / w;
	float v0 = ((TexOnePieceY * CurrentFramePosY)) / h;
	float u1 = ((TexOnePieceX * CurrentFramePosX) + TexOnePieceX) / w;
	float v1 = ((TexOnePieceY * CurrentFramePosY) + TexOnePieceY) / h;

	Vertex3D v[] = {
	{Getvec3ByFaceGenType(type,0),Color,vec2f(u0,v0)},
	{Getvec3ByFaceGenType(type,1),Color,vec2f(u1,v0)},
	{Getvec3ByFaceGenType(type,2),Color,vec2f(u0,v1)},
	{Getvec3ByFaceGenType(type,3),Color,vec2f(u1,v1)},
	};

	D3DXMATRIX mtxW, mtxT, mtxS, mtxR;
	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, 0.0f);
	D3DXMatrixScaling(&mtxS, scale.x, scale.y, 0.0f);
	D3DXMatrixRotationZ(&mtxR, radian);
	mtxW = mtxS * mtxR * mtxT;
	pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex3D));
}

//テクスチャ分割関数
void FaceGenforTex(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ポリゴンのサイズはテクスチャサイズを計算
	float w = (float)TextureGetTextureSize(TexId, 'W');
	float h = (float)TextureGetTextureSize(TexId, 'H');
	Int2 FrameMAX = Texture_GetFrameMax(TexId);
	//分割するテクスチャの一つのサイズを計算
	float TexOnePieceX = (w / FrameMAX.x);
	float TexOnePieceY = (h / FrameMAX.y);

	//現在のフレームの場所を計算
	int CurrentFramePosX = CurrentFrame % FrameMAX.x;
	int CurrentFramePosY = CurrentFrame / FrameMAX.x;

	//UVpos計算
	float u0 = ((TexOnePieceX * CurrentFramePosX)) / w;
	float v0 = ((TexOnePieceY * CurrentFramePosY)) / h;
	float u1 = ((TexOnePieceX * CurrentFramePosX) + TexOnePieceX) / w;
	float v1 = ((TexOnePieceY * CurrentFramePosY) + TexOnePieceY) / h;

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 0.0f, 1.0f),Color,vec2f(u0,v0)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 0.0f, 1.0f),Color,vec2f(u1,v0)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(u0,v1)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 0.0f, 1.0f),Color,vec2f(u1,v1)},
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void FaceGenforTexInvert(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, DIR dir, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ポリゴンのサイズはテクスチャサイズを計算
	float w = (float)TextureGetTextureSize(TexId, 'W');
	float h = (float)TextureGetTextureSize(TexId, 'H');

	Int2 FrameMAX = Texture_GetFrameMax(TexId);
	//分割するテクスチャの一つのサイズを計算
	float TexOnePieceX = (w / FrameMAX.x);
	float TexOnePieceY = (h / FrameMAX.y);

	//現在のフレームの場所を計算
	int CurrentFramePosX = CurrentFrame % FrameMAX.x;
	int CurrentFramePosY = CurrentFrame / FrameMAX.x;

	//UVpos計算
	float u0 = ((TexOnePieceX * CurrentFramePosX)) / w;
	float v0 = ((TexOnePieceY * CurrentFramePosY)) / h;
	float u1 = ((TexOnePieceX * CurrentFramePosX) + TexOnePieceX) / w;
	float v1 = ((TexOnePieceY * CurrentFramePosY) + TexOnePieceY) / h;

	if (dir == dir_left)
	{
		float ucp = u0;
		u0 = u1;
		u1 = ucp;
	}

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 1.0f, 1.0f),Color,vec2f(u0,v0)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 1.0f, 1.0f),Color,vec2f(u1,v0)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(u0,v1)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(u1,v1)},
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

}

//回転
void FaceGenRotate(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, float angle, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	if (!IsUseTex) TexId = g_WhitePoliTex;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ポリゴンのサイズはテクスチャサイズを計算
	float w = (float)TextureGetTextureSize(TexId, 'W');
	float h = (float)TextureGetTextureSize(TexId, 'H');

	Int2 FrameMAX = Texture_GetFrameMax(TexId);
	//分割するテクスチャの一つのサイズを計算
	float TexOnePieceX = (w / FrameMAX.x);
	float TexOnePieceY = (h / FrameMAX.y);

	//現在のフレームの場所を計算
	int CurrentFramePosX = CurrentFrame % FrameMAX.x;
	int CurrentFramePosY = CurrentFrame / FrameMAX.y;

	//UVpos計算
	float u0 = ((TexOnePieceX * CurrentFramePosX)) / w;
	float v0 = ((TexOnePieceY * CurrentFramePosY)) / h;
	float u1 = ((TexOnePieceX * CurrentFramePosX) + TexOnePieceX) / w;
	float v1 = ((TexOnePieceY * CurrentFramePosY) + TexOnePieceY) / h;

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 1.0f, 1.0f),Color,vec2f(u0,v0)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 1.0f, 1.0f),Color,vec2f(u1,v0)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(u0,v1)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(u1,v1)},
	};

	D3DXMATRIX mtxT1;
	D3DXMatrixTranslation(&mtxT1, -(size.x / 2 + pos.x), -(size.y / 2 + pos.y), 0.0f);

	D3DXMATRIX mtxT2;
	D3DXMatrixTranslation(&mtxT2, size.x / 2 + pos.x, size.y / 2 + pos.y, 0.0f);

	D3DXMATRIX mtxR;
	D3DXMatrixRotationZ(&mtxR, angle);

	D3DXMATRIX mtxWorld;
	mtxWorld = mtxT1 * mtxR * mtxT2;

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void FaceGenRenderRotate(vec2f pos, vec2f size,
	IDirect3DTexture9* Render, float w, float h,
	float angel, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Render);

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 1.0f, 1.0f),Color,vec2f(0.f,0.f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 1.0f, 1.0f),Color,vec2f(1.f,0.f)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(0.f,1.f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 1.0f, 1.0f),Color,vec2f(1.f,1.f)},
	};

	D3DXMATRIX mtxT1;
	D3DXMatrixTranslation(&mtxT1, -(size.x / 2 + pos.x), -(size.y / 2 + pos.y), 0.0f);

	D3DXMATRIX mtxT2;
	D3DXMatrixTranslation(&mtxT2, size.x / 2 + pos.x, size.y / 2 + pos.y, 0.0f);

	D3DXMATRIX mtxR;
	D3DXMatrixRotationZ(&mtxR, angel);

	D3DXMATRIX mtxWorld;
	mtxWorld = mtxT1 * mtxR * mtxT2;

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

}
//DIR引数の方向から、modeで指定した方法でゲージが動く。
//GAGEMODE引数 -> gage_add:0から増やす / gage_sub:sizeからDIRに合わせて減らしていく。
//GageNum引数はこの関数を呼ぶcppで変更する必要あり。
/*※GageNumがマイナスになることは想定していないので、
ゲージを減らしていきたい場合もmodeをgage_subにしてGageNumを足していく。*/
void FaceGenforGage(vec2f pos, vec2f size, float GageNum, DIR dir,
	GAGEMODE mode, bool isUI, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(g_WhitePoliTex));

	float width = size.x;
	float height = size.y;

	//頂点ポジション
	float x0 = pos.x;
	float y0 = pos.y;
	float x1 = pos.x;
	float y1 = pos.y;

	switch (dir)
	{
	case dir_top:
		y0 = mode == gage_sub ? pos.y + GageNum : pos.y + height - GageNum;
		y1 = pos.y + height;
		x1 = pos.x + width;
		break;
	case dir_under:
		y1 = mode == gage_sub ? pos.y + height - GageNum : pos.y + GageNum;
		x1 = pos.x + width;
		break;
	case dir_right:
		x1 = mode == gage_sub ? pos.x + width - GageNum : pos.x + GageNum;
		y1 = pos.y + height;
		break;
	case dir_left:
		x0 = mode == gage_sub ? pos.x + GageNum : pos.x + width - GageNum;
		x1 = pos.x + width;
		y1 = pos.y + height;
		break;
	default:
		break;
	}

	Vertex2D v[] = {
	{D3DXVECTOR4(x0,y0, 1.0f, 1.0f),Color,vec2f(0,0)},
	{D3DXVECTOR4(x1,y0, 1.0f, 1.0f),Color,vec2f(1,0)},
	{D3DXVECTOR4(x0,y1, 1.0f, 1.0f),Color,vec2f(0,1)},
	{D3DXVECTOR4(x1,y1, 1.0f, 1.0f),Color,vec2f(1,1)},
	};

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void CircleGen(vec2f pos, float R, int vertexNum, D3DCOLOR color)
{
	if (R < 5.0f)
	{
		FaceGenSimple(pos, vec2f(R, R), color);
	}
	else
	{
		CircleCute(pos, R, vertexNum, color);
	}
}

//円をラインで描く関数。
void CircleGenForLine(vec2f pos, float R, D3DCOLOR color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	const float PI = 3.14f;

	Vertex2D ver[CIRCLE_CUTE + 1] = {};
	for (int i = 0; i <= CIRCLE_CUTE; i++)
	{
		int n = i % CIRCLE_CUTE;
		float rad = 2 * PI / CIRCLE_CUTE * n;
		float px = pos.x + cosf(rad) * R;
		float py = pos.y + sinf(rad) * R;

		ver[i].Position = D3DXVECTOR4(px, py, 1.0f, 1.0f);
		ver[i].Color = color;
	}

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLE_CUTE, ver, sizeof(Vertex2D));

}

//一本グランプリ。
void LineGen(vec2f startpos, vec2f endpos, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	Vertex2D ver[] =
	{
		{D3DXVECTOR4(startpos.x ,startpos.y  , 1.0f, 1.0f),Color},
		{D3DXVECTOR4(endpos.x   ,endpos.y    , 1.0f, 1.0f),Color},
	};

	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, ver, sizeof(Vertex2D));
}

//一本グランプリ。
void LineGen3D(vec2f pos, float size, DIR dir, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, NULL);

	Vertex3D ver[] =
	{
		{D3DXVECTOR3(0.0f , 0.0f , 0.0f),Color},
		{D3DXVECTOR3(1.0f , 0.0f , 0.0f),Color},
	};

	D3DXMATRIX mtxS, mtxT, mtxR, mtxW;

	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, 0.0f);

	switch (dir)
	{
	case dir_top:
		D3DXMatrixScaling(&mtxS, 0.0f, size, 0.0f);
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
		break;
	case dir_under:
		D3DXMatrixScaling(&mtxS, 0.0f, size, 0.0f);
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(-90));
		break;
	case dir_right:
		D3DXMatrixScaling(&mtxS, size, 0.0f, 0.0f);
		break;
	case dir_left:
		D3DXMatrixScaling(&mtxS, size, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	mtxW = mtxS * mtxR * mtxT;
	pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, ver, sizeof(Vertex3D));
}

//dirに指定した方向が三角のてっぺんになる。
void DeltaGen(vec2f pos, vec2f size, DIR dir, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y             , 1.0f, 1.0f),Color,vec2f(0.0f,0.0f)},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y / 2, 1.0f, 1.0f),Color,vec2f(1.0f,0.0f)},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y    , 1.0f, 1.0f),Color,vec2f(0.0f,1.0f)},
	};

	D3DXMATRIX mtxR, mtxT1, mtxT2, mtxWorld;

	D3DXMatrixTranslation(&mtxT1, -(size.x / 2 + pos.x), -(size.y / 2 + pos.y), 0.0f);
	D3DXMatrixTranslation(&mtxT2, size.x / 2 + pos.x, size.y / 2 + pos.y, 0.0f);

	const float pad = 3.141592f / 180.0f;
	int angle = dir == dir_right ? 0 :
		dir == dir_under ? 90 :
		dir == dir_left ? 180 : 270;

	D3DXMatrixRotationZ(&mtxR, angle * pad);
	mtxWorld = mtxT1 * mtxR * mtxT2;

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, v, sizeof(Vertex2D));
}

//ラインの四角を作る関数。
void LineGenforSquare(vec2f pos, vec2f size, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	Vertex2D v[] = {
	{D3DXVECTOR4(pos.x         ,pos.y         , 1.0f, 1.0f),Color},
	{D3DXVECTOR4(pos.x + size.x,pos.y         , 1.0f, 1.0f),Color},
	{D3DXVECTOR4(pos.x + size.x,pos.y + size.y, 1.0f, 1.0f),Color},
	{D3DXVECTOR4(pos.x         ,pos.y + size.y, 1.0f, 1.0f),Color},
	{D3DXVECTOR4(pos.x         ,pos.y         , 1.0f, 1.0f),Color},
	};

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, v, sizeof(Vertex2D));
}

void LineGenforSquare3D(vec2f pos, vec2f size, D3DCOLOR Color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX3D);
	pDevice->SetTexture(0, NULL);

	//Vertex3D v[] = {
	//{D3DXVECTOR3(0.0f,0.0f,0.0f),Color},
	//{D3DXVECTOR3(1.0f,0.0f,0.0f),Color},
	//{D3DXVECTOR3(1.0f,-1.0f,0.0f),Color},
	//{D3DXVECTOR3(0.0f,-1.0f,0.0f),Color},
	//{D3DXVECTOR3(0.0f,0.0f,0.0f),Color},
	//};

	Vertex3D v[] = {
	{D3DXVECTOR3(pos.x         ,pos.y - 1.0f,0.0f),Color},
	{D3DXVECTOR3(pos.x + size.x,pos.y - 1.0f,0.0f),Color},
	{D3DXVECTOR3(pos.x + size.x,pos.y - 1.0f + size.y,0.0f),Color},
	{D3DXVECTOR3(pos.x         ,pos.y - 1.0f + size.y,0.0f),Color},
	{D3DXVECTOR3(pos.x         ,pos.y - 1.0f,0.0f),Color},
	};

	//D3DXMATRIX mtxS, mtxR, mtxT, mtxW;
	//D3DXMatrixTranslation(&mtxT, pos.x, pos.y, 0.0f);
	//D3DXMatrixScaling(&mtxS, size.x, size.y, 0.0f);
	//mtxW = mtxS * mtxT;
	//pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, v, sizeof(Vertex3D));
}

//丸を作る関数。
void CircleCute(vec2f pos, float R, int vertexNum, D3DCOLOR color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	const float PI = 3.14f;
	if (vertexNum < 2)vertexNum = 3;//頂点数が2以下なら3にする。
	Vertex2D* ver = new Vertex2D[vertexNum];

	ver[0].Position = D3DXVECTOR4(pos.x, pos.y, 1.0f, 1.0f);
	ver[0].Color = color;

	for (int i = 1; i <= vertexNum + 1; i++)
	{
		int n = i % vertexNum;
		float rad = 2 * PI / vertexNum * n;
		float px = pos.x + cosf(rad) * R;
		float py = pos.y + sinf(rad) * R;

		ver[i].Position = D3DXVECTOR4(px, py, 1.0f, 1.0f);
		ver[i].Color = color;
	}
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertexNum, ver, sizeof(Vertex2D));

	delete[] ver;
	ver = NULL;
}

void RingGen(vec2f pos, float R1, float R2, D3DCOLOR color1, D3DCOLOR color2, int Prec)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	const float PI = 3.14f;
	int vNum = (int)(2.0f * R2 * PI / Prec);
	Vertex2D* ver = new Vertex2D[vNum * 2 + 2];
	for (int i = 0; i < vNum + 1; i++)
	{
		float rad = 2 * PI * i / vNum;
		float px = pos.x + cosf(rad) * R1;
		float py = pos.y + sinf(rad) * R1;
		ver[2 * i].Position = D3DXVECTOR4(px, py, 1.0f, 1.0f);
		ver[2 * i].Color = color1;
		px = pos.x + cosf(rad) * R2;
		py = pos.y + sinf(rad) * R2;
		ver[2 * i + 1].Position = D3DXVECTOR4(px, py, 1.0f, 1.0f);
		ver[2 * i + 1].Color = color2;
	}

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vNum * 2, ver, sizeof(Vertex2D));
	delete[] ver;
}

void CircleHQ(vec2f pos, float R, D3DCOLOR color)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) return;

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, NULL);

	const float PI = 3.14f;
	int vNum = (int)(2.0f * R * PI);
	Vertex2D* ver = new Vertex2D[vNum];
	for (int i = 0; i < vNum; i++)
	{
		float rad = 2 * PI * i / vNum;
		float px = pos.x + cosf(rad) * R;
		float py = pos.y + sinf(rad) * R;
		ver[i].Position = D3DXVECTOR4(px, py, 1.0f, 1.0f);
		ver[i].Color = color;
	}

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vNum - 2, ver, sizeof(Vertex2D));
	delete[] ver;
}

void CircleLightGen(vec2f pos, float R, unsigned Intensity, D3DCOLOR Color)
{
	float Dem = R * Intensity / 10;
	CircleHQ(pos, Dem, Color);
	D3DCOLOR tmp = 0x00ffffff & Color;
	RingGen(pos, Dem, R, Color, tmp, 20);
}

vec3f Getvec3ByFaceGenType(FACEGENTYPE type, int index)
{
	//switch (type)
	//{
	//case facegentype_center:
	//	return g_centerVertex[index];
	//case facegentype_leftup:
	//	return g_leftUpVertex[index];
	//default:
	//	break;
	//}

	return g_leftUpVertex[index];
}

void FaceGenUninit()
{
	TextureRelease(&g_WhitePoliTex, 1);
	g_WhitePoliTex = NULL;
}