


#include "Shade.h"
#include "texture.h"
#include "mydirect3D.h"

typedef struct Shade_tag
{
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;
}ShadeVertex;

#define FVF_SHADE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

static int g_shadeTex;

void ShadeInit(void)
{
	g_shadeTex = SetTexture("asset/texture/shade.png");
	TextureLoad("‰e");
}

//void ShadeDraw(D3DXVECTOR2 pos, D3DXVECTOR2 size)
//{
//	static const ShadeVertex v[] =
//	{
//		{{-0.5f, 0.0f, 0.5f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,0.0f}},
//		{{ 0.5f, 0.0f, 0.5f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,0.0f}},
//		{{-0.5f, 0.0f,-0.5f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,1.0f}},
//		{{ 0.5f, 0.0f,-0.5f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,1.0f}},
//	};
//
//	D3DXMATRIX mtxR, mtxT, mtxS, mtxW;
//	//D3DXMatrixRotationX(&mtxR, 90.0f);
//	D3DXMatrixTranslation(&mtxT, pos.x, 0.001f, pos.y);
//	D3DXMatrixScaling(&mtxS, size.x, 0.0f, size.y);
//	mtxW = mtxT;
//
//	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
//
//	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
//	pDevice->SetFVF(FVF_SHADE);
//	pDevice->SetTexture(0, Texture_GetTexture(g_shadeTex));
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(ShadeVertex));
//
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//}

void ShadeDraw(vec3f pos, D3DXVECTOR2 size)
{
	static const ShadeVertex v[] =
	{
		{{-0.5f, 0.0f, 0.5f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,0.0f}},
		{{ 0.5f, 0.0f, 0.5f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,0.0f}},
		{{-0.5f, 0.0f,-0.5f},D3DCOLOR_RGBA(255,255,255,255),{0.0f,1.0f}},
		{{ 0.5f, 0.0f,-0.5f},D3DCOLOR_RGBA(255,255,255,255),{1.0f,1.0f}},
	};

	D3DXMATRIX mtxR, mtxT, mtxS, mtxW;
	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&mtxS, size.x, 0.0f, size.y);
	mtxW = mtxT;

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
	pDevice->SetFVF(FVF_SHADE);
	pDevice->SetTexture(0, Texture_GetTexture(g_shadeTex));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(ShadeVertex));

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void ShadeUninit(void)
{
	TextureRelease(&g_shadeTex, 1);
	g_shadeTex = NULL;
}
