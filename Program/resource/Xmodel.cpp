

#include "Xmodel.h"
#include "mydirect3D.h"
#include "texture.h"
#include "Config.h"
#include "Light.h"

#define XMODEL_MAX (64)

struct XMODEL
{
	LPD3DXMESH pMesh;//メッシュ　インデックスとか頂点とかを管理してるヤツ。
	//LPD3DXBUFFER pAdjacency;//隣接情報バッファ
	DWORD materialCnt;//マテリアル数
	//マテリアル情報
	D3DMATERIAL9* pMaterials;
	//テクスチャID
	int* pTextureIds;
};


static XMODEL g_XModels[XMODEL_MAX] = {};


void XModelInit(void)
{
	XModelReleaseAll();
}

int XModelLoad(const char * pFileName)
{
	//g_XModelsから未使用領域を検索

	int id = XMODEL_INVALID_ID;
	for (int i = 0; i < XMODEL_MAX; i++)
	{
		if (g_XModels[i].pMesh == NULL)
		{
			id = i;
			break;
		}
	}

	if (id == XMODEL_INVALID_ID)
	{
		return XMODEL_INVALID_ID;//未使用領域がなかった
	}

	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	LPD3DXBUFFER pAdjacency;	//隣接情報バッファ(一時使用)
	LPD3DXMESH pTempMesh;		//モデルメッシュ(一時使用)
	LPD3DXBUFFER pMaterial;
	DWORD materialCount;

	//とりあえず編集可能な状態で読み込む
	hr = D3DXLoadMeshFromX(
		pFileName,			//ファイル名
		D3DXMESH_SYSTEMMEM,	//オプション(メインメモリに情報を作成)
		pDevice,			//Direct3Dデバイス
		&pAdjacency,		//隣接情報(取得)
		&pMaterial,			//マテリアル情報(取得)
		NULL,				//エフェクト（シェーダー）(取得)
		&materialCount,		//マテリアル数(取得)
		&pTempMesh
	);

	g_XModels[id].materialCnt = materialCount;
	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterial->GetBufferPointer();

	g_XModels[id].pMaterials = new D3DMATERIAL9[materialCount];
	g_XModels[id].pTextureIds = new int[materialCount];

	for (unsigned i = 0; i < materialCount; i++)
	{
		//マテリアル値のコピー
		//要素を選んでコピーした方がいいかも。
		g_XModels[id].pMaterials[i] = pD3DXMaterial[i].MatD3D;
		//g_XModels[id].pMaterials[i].Diffuse = pD3DXMaterial[i].MatD3D.Emissive;
		pD3DXMaterial[i].MatD3D.Emissive = {1,1,1};

		pD3DXMaterial[i].pTextureFilename;

		//テクスチャファイル名の取得
		if (pD3DXMaterial[i].pTextureFilename)
		{
			char buf[256] = {};
			strcpy(buf, pFileName);

			char* pSplit = strrchr(buf, '/');
			*(pSplit + 1) = '\0';

			strcat(buf, pD3DXMaterial[i].pTextureFilename);
			g_XModels[id].pTextureIds[i] = SetTexture(buf);
		}
		else
		{
			g_XModels[id].pTextureIds[i] = TEXTURE_INVALID_ID;
		}
	}
	pMaterial->Release();

	if (FAILED(hr))
	{
		MessageBox(NULL, "モデル読み込み失敗", "エラー", NULL);
		return XMODEL_INVALID_ID;
	}

	//メッシュの最適化をする
	pTempMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);

	//高速で面倒を見てくれる頂点バッファを取り扱うメッシュとしてクローンを作成する
	pTempMesh->CloneMeshFVF(
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		pTempMesh->GetFVF(),
		pDevice,
		&g_XModels[id].pMesh
	);

	//用済みのバッファを削除
	pAdjacency->Release();
	pTempMesh->Release();

	return id;
}

void XModelDraw(int id, D3DXMATRIX * mtxModel)
{
	if (!g_XModels[id].pMesh)return;//指定したモデル番号が未使用なら。

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, mtxModel);

	SetLight(true);

	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	for (DWORD i = 0; i < g_XModels[id].materialCnt; i++)
	{
		pDevice->SetMaterial(&g_XModels[id].pMaterials[i]);//マテリアルを設定

		pDevice->SetTexture(0, Texture_GetTexture(g_XModels[id].pTextureIds[i]));

		//FVFの設定、頂点バッファとインデックスバッファをデバイスに設定->DrawPrimitive
		g_XModels[id].pMesh->DrawSubset(i);
	}
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	SetLight(false);
}

void XModelUninit(void)
{

}

void XModelRelease(const int * pID, int cnt)
{
	for (int i = 0; i < cnt; i++)
	{
		if (!g_XModels[i].pMesh)continue;

		g_XModels[pID[i]].pMesh->Release();
		g_XModels[pID[i]].pMesh = NULL;

		delete[] g_XModels[pID[i]].pTextureIds;
		delete[] g_XModels[pID[i]].pMaterials;
	}
}

void XModelReleaseAll(void)
{
	for (int i = 0; i < XMODEL_MAX; i++)
	{
		XModelRelease(&i, 1);
	}
}