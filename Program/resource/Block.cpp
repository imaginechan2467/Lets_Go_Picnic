/*==============================================================================

   [Cube.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include <d3dx9.h>
#include "mydirect3D.h"
#include "Config.h"
#include "texture.h"
#include "Block.h"
#include "Light.h"
#include "Game.h"


//*****************************************************************************
// マクロ
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
Vertex3D BlockBaseClass::CubeVertex[BLOCK_VERTEX_MAX] = 
{
	//手前
	{ D3DXVECTOR3(-0.5f, 0.5f, -0.5f),D3DXVECTOR3(0.0f, 0.0f,-1.0f), COLOR_NORMAL,{0,0}        },
	{ D3DXVECTOR3(0.5f, 0.5f,-0.5f),  D3DXVECTOR3(0.0f, 0.0f,-1.0f), COLOR_NORMAL,{0.33f,0}    },
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f, 0.0f,-1.0f), COLOR_NORMAL,{0,0.5f}     },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f, 0.0f,-1.0f), COLOR_NORMAL,{0.33f,0.5f} },
	//右
	{ D3DXVECTOR3(0.5f, 0.5f,-0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f), COLOR_NORMAL,{0.33f,0} },
	{ D3DXVECTOR3(0.5f, 0.5f, 0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f), COLOR_NORMAL,{0.66f,0} },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f), COLOR_NORMAL,{0.33f,0.5f} },
	{ D3DXVECTOR3(0.5f,-0.5f, 0.5f),  D3DXVECTOR3(1.0f,0.0f,0.0f), COLOR_NORMAL,{0.66f,0.5f} },
	//左
	{ D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f),COLOR_NORMAL,{0.66f,0} },
	{ D3DXVECTOR3(-0.5f, 0.5f,-0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f),COLOR_NORMAL,{1,0} },
	{ D3DXVECTOR3(-0.5f,-0.5f, 0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f),COLOR_NORMAL,{0.66f,0.5f} },
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(-1.0f,0.0f,0.0f),COLOR_NORMAL,{1,0.5f} },
	//後ろ
	{ D3DXVECTOR3(0.5f, 0.5f,0.5f),   D3DXVECTOR3(0.0f,0.0f,1.0f), COLOR_NORMAL,{0,0.5f} },
	{ D3DXVECTOR3(-0.5f, 0.5f,0.5f),  D3DXVECTOR3(0.0f,0.0f,1.0f), COLOR_NORMAL,{0.33f,0.5f} },
	{ D3DXVECTOR3(0.5f,-0.5f,0.5f),   D3DXVECTOR3(0.0f,0.0f,1.0f), COLOR_NORMAL,{0,1} },
	{ D3DXVECTOR3(-0.5f,-0.5f,0.5f),  D3DXVECTOR3(0.0f,0.0f,1.0f), COLOR_NORMAL,{0.33f,1} },
	//上
	{ D3DXVECTOR3(0.5f,0.5f,-0.5f),   D3DXVECTOR3(0.0f,1.0f,0.0f), COLOR_NORMAL,{0.33f,0.5f} },
	{ D3DXVECTOR3(-0.5f,0.5f,-0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f), COLOR_NORMAL,{0.66f,0.5f} },
	{ D3DXVECTOR3(0.5f,0.5f, 0.5f),   D3DXVECTOR3(0.0f,1.0f,0.0f), COLOR_NORMAL,{0.33f,1} },
	{ D3DXVECTOR3(-0.5f,0.5f, 0.5f),  D3DXVECTOR3(0.0f,1.0f,0.0f), COLOR_NORMAL,{0.66f,1} },
	//下
	{ D3DXVECTOR3(-0.5f,-0.5f,-0.5f), D3DXVECTOR3(0.0f,-1.0f,0.0f),COLOR_NORMAL,{0.66f,0.5f} },
	{ D3DXVECTOR3(0.5f,-0.5f,-0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f),COLOR_NORMAL,{1,0.5f} },
	{ D3DXVECTOR3(-0.5f,-0.5f, 0.5f), D3DXVECTOR3(0.0f,-1.0f,0.0f),COLOR_NORMAL,{0.66f,1} },
	{ D3DXVECTOR3(0.5f,-0.5f, 0.5f),  D3DXVECTOR3(0.0f,-1.0f,0.0f),COLOR_NORMAL,{1,1} },
};
LPDIRECT3DVERTEXBUFFER9 BlockBaseClass::pVertexBuffer = NULL;
LPDIRECT3DINDEXBUFFER9 BlockBaseClass::pIndexBuffer = NULL;

int BlockClass::blockTexture[BlockType_MAX] = {};
std::vector<BlockClass*> BlockClass::blockList;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static Vertex3D g_CubeVertex[] =
{
	//正面
	{D3DXVECTOR3(-0.5f, 0.5f,-0.5f), {0,0,-1},COLOR_NORMAL,{0,0}},
	{D3DXVECTOR3(0.5f, 0.5f,-0.5f),  {0,0,-1},COLOR_NORMAL,{0.33f,0}},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f), {0,0,-1},COLOR_NORMAL,{0,0.5f}},
	{D3DXVECTOR3(0.5f, 0.5f,-0.5f),  {0,0,-1},COLOR_NORMAL,{0.33f,0}},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),  {0,0,-1},COLOR_NORMAL,{0.33f,0.5f}},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f), {0,0,-1},COLOR_NORMAL,{0,0.5f}},
	//右
	{D3DXVECTOR3(0.5f, 0.5f,-0.5f),{1,0,0},COLOR_NORMAL,{0.33f,0}},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f),{1,0,0},COLOR_NORMAL,{0.66f,0}},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),{1,0,0},COLOR_NORMAL,{0.33f,0.5f}},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f),{1,0,0},COLOR_NORMAL,{0.66f,0}},
	{D3DXVECTOR3(0.5f,-0.5f, 0.5f),{1,0,0},COLOR_NORMAL,{0.66f,0.5f}},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f),{1,0,0},COLOR_NORMAL,{0.33f,0.5f}},
	//左
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f),{-1,0,0},COLOR_NORMAL,{0.66f,0}},
	{D3DXVECTOR3(-0.5f, 0.5f,-0.5f),{-1,0,0},COLOR_NORMAL,{1,0}},
	{D3DXVECTOR3(-0.5f,-0.5f, 0.5f),{-1,0,0},COLOR_NORMAL,{0.66f,0.5f}},
	{D3DXVECTOR3(-0.5f, 0.5f,-0.5f),{-1,0,0},COLOR_NORMAL,{1,0}},
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),{-1,0,0},COLOR_NORMAL,{1,0.5f}},
	{D3DXVECTOR3(-0.5f,-0.5f, 0.5f),{-1,0,0},COLOR_NORMAL,{0.66f,0.5f}},
	//後ろ
	{D3DXVECTOR3(0.5f, 0.5f,0.5f), {0,0,1}, COLOR_NORMAL,{0,0.5f}},
	{D3DXVECTOR3(-0.5f, 0.5f,0.5f),{0,0,1}, COLOR_NORMAL,{0.33f,0.5f}},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f), {0,0,1}, COLOR_NORMAL,{0,1}},
	{D3DXVECTOR3(-0.5f, 0.5f,0.5f),{0,0,1}, COLOR_NORMAL,{0.33f,0.5f}},
	{D3DXVECTOR3(-0.5f,-0.5f,0.5f),{0,0,1}, COLOR_NORMAL,{0.33f,1}},
	{D3DXVECTOR3(0.5f,-0.5f,0.5f) ,{0,0,1}, COLOR_NORMAL,{0,1}},
	//上
	{D3DXVECTOR3(0.5f,0.5f,-0.5f), {0,1,0}, COLOR_NORMAL,{0.33f,0.5f}},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),{0,1,0}, COLOR_NORMAL,{0.66f,0.5f}},
	{D3DXVECTOR3(0.5f,0.5f, 0.5f), {0,1,0}, COLOR_NORMAL,{0.33f,1}},
	{D3DXVECTOR3(-0.5f,0.5f,-0.5f),{0,1,0}, COLOR_NORMAL,{0.66f,0.5f}},
	{D3DXVECTOR3(-0.5f,0.5f, 0.5f),{0,1,0}, COLOR_NORMAL,{0.66f,1}},
	{D3DXVECTOR3(0.5f,0.5f, 0.5f), {0,1,0}, COLOR_NORMAL,{0.33f,1}},
	//下
	{D3DXVECTOR3(-0.5f,-0.5f,-0.5f),{0,-1,0},COLOR_NORMAL,{0.66f,0.5f}},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f), {0,-1,0},COLOR_NORMAL,{1,0.5f}},
	{D3DXVECTOR3(-0.5f,-0.5f, 0.5f),{0,-1,0},COLOR_NORMAL,{0.66f,1}},
	{D3DXVECTOR3(0.5f,-0.5f,-0.5f), {0,-1,0},COLOR_NORMAL,{1,0.5f}},
	{D3DXVECTOR3(0.5f,-0.5f, 0.5f), {0,-1,0},COLOR_NORMAL,{1,1}},
	{D3DXVECTOR3(-0.5f,-0.5f, 0.5f),{0,-1,0},COLOR_NORMAL,{0.66f,1}},
};

//*****************************************************************************
// 関数定義（BlockClass）
//*****************************************************************************
BlockClass::BlockClass()
	:GameObjectClass({ 0.0f,0.0f,0.0f }, {1.0f,1.0f,1.0f},true,objtype_system,GetCurrentStage())
{
	//このインスタンスは作成用カレント。
	pos = { 0.0f,0.0f,0.0f };
	size = { 1.0f,1.0f,1.0f };
	isActive = true;
	objType = objtype_system;
	color = COLOR_NORMAL;
	stageNumber = GetCurrentStage();
	blockType = BlockType_normal;
	cnt = 0;
	frame = 0;
	waitCnt = 0;
	fallspeed = 0.0f;
	isBack = false;
	isTouched = false;
	tempvec3f_1 = { 0.0f,0.0f,0.0f };
	isSet = false;

	//オブジェクト登録
	blockList.push_back(this);
}

BlockClass::BlockClass(vec3f setpos, vec3f setsize, bool setisActive,
	BlockType settype, STAGE setstagenum)
	:GameObjectClass(setpos,setsize, setisActive,objtype_block,setstagenum)
{
	pos = setpos;
	size = setsize;
	isActive = setisActive;
	blockType = settype;
	stageNumber = setstagenum;
	color = COLOR_NORMAL;

	tempvec3f_1 = { 0.0f,0.0f,0.0f };
	isTouched = false;
	isBack = false;
	fallspeed = 0.0f;
	objType = objtype_block;
	cnt = 0;
	frame = 0;
	waitCnt = 0;
	isSet = false;

	blockList.push_back(this);
}

BlockClass::~BlockClass()
{
	
}

void BlockClass::Init(void)
{
	//テクスチャロード
	{
		blockTexture[0] = SetTexture("asset/texture/block_concrete.png", { 3,2 });
		blockTexture[1] = SetTexture("asset/texture/block_rock.png", { 3,2 });

		blockTexture[2] = SetTexture("asset/texture/block_magma.png", { 3,2 });
		blockTexture[3] = SetTexture("asset/texture/block_fall.png", { 3,2 });
		blockTexture[4] = SetTexture("asset/texture/block_elevator.png", { 3,2 });
		blockTexture[5] = SetTexture("asset/texture/block_horizontal.png", { 3,2 });
		blockTexture[6] = SetTexture("asset/texture/block_soil.png", { 3,2 });
		TextureLoad("ブロック");
	}
}

void BlockClass::Uninit(void)
{
	TextureRelease(blockTexture, BlockType_MAX);
	memset(blockTexture, NULL, sizeof(blockTexture));

	blockList.erase(blockList.begin(), blockList.end());
}

void BlockClass::Update(void)
{
	if (objType == objtype_system)return;

	switch (blockType)
	{
	case BlockClass::BlockType_normal:
		break;
	case BlockClass::BlockType_rock:
		break;
	case BlockClass::BlockType_fire:
		break;
	case BlockClass::BlockType_fall:

		if (isSet)
		{
			cnt++;
			if (cnt >= 60 * 2)
			{
				tempvec3f_1.y = pos.y - BLOCK_FALL_MAX_SIZE;

				fallspeed += GRAVITY;
				pos.y -= fallspeed;

				//削除
				if (pos.y <= tempvec3f_1.y)
					DeleteByUniqueNumber(uniqueNumber);
			}
		}

		break;
	case BlockClass::BlockType_elevator:

		cnt++;
		if (cnt >= BLOCK_SWITCH_COUNT)
		{
			cnt = 0;
			isBack = (isBack + 1) % 2;
		}
		if (isBack)
			pos.y -= BLOCK_MOVE_SPEED;
		else
			pos.y += BLOCK_MOVE_SPEED;


		break;
	case BlockClass::BlockType_horizontal:

		cnt++;
		if (cnt >= BLOCK_SWITCH_COUNT)
		{
			cnt = 0;
			isBack = (isBack + 1) % 2;
		}
		if (isBack)
		{
			pos.x -= BLOCK_MOVE_SPEED;
		}
		else
		{
			pos.x += BLOCK_MOVE_SPEED;
		}

		break;
	case BlockClass::BlockType_goal:
		break;
	default:
		break;
	}
}

void BlockClass::Draw(void)
{
	if (!isActive)return;

	D3DXMATRIX mtxW, mtxT, mtxR, mtxS;

	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&mtxS, size.x, size.y, size.z);
	mtxW = mtxS * mtxT;

	if (blockType == BlockType_fall)
	{
		BlockBaseClass::DrawBoxNotIndex(mtxW,blockTexture[blockType], color);
	}
	else
	{
		DrawCube(mtxW, blockTexture[blockType]);
	}
}

void BlockClass::DrawAll(void)
{
	SetLight(true);

	for (auto& i : blockList)
	{
		i->Draw();
	}

	SetLight(false);
}

void BlockClass::UpdateAll(void)
{
	for (auto& i : blockList)
	{
		i->Update();
	}
}

void BlockClass::SaveAll(void)
{
	//todo:ファイル名作成

	char temp[96] = {};
	FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_block), "wb");

	//個数保存
	int num = blockList.size();
	fwrite(&num, sizeof(int), 1, fp);

	//セーブ
	for (auto& i : blockList)
	{
		if (i->objType == objtype_system)continue;

		i->Save(fp,i->blockType);
	}

	fclose(fp);
}

void BlockClass::Load(bool isreset)
{
	char temp[96] = {};
	FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_block), "rb");
	if (!fp)
	{
		MessageBox(NULL, "ブロックセーブデータファイルが開けませんでした", "エラー", MB_OK);
		return;
	}
	if (isreset)
	{
		//読み込む前に配列をリセット
		ResetAll(false);
	}

	//個数ロード
	int num = 0;
	fread(&num, sizeof(int), 1, fp);

	//ロード
	for (int i = 0;i < num;i++)
	{
		vec3f setpos = {};
		vec3f setsize = {};
		bool setisActive = {};
		BlockType setBlockType = {};

		STAGE setstagenum = {};

		fread(&setpos, sizeof(vec3f), 1, fp);
		fread(&setsize, sizeof(vec3f), 1, fp);
		fread(&setisActive, sizeof(bool), 1, fp);

		fread(&setBlockType, sizeof(BlockType), 1, fp);
		fread(&setstagenum, sizeof(STAGE), 1, fp);

		new BlockClass(setpos, setsize, setisActive, setBlockType, setstagenum);
	}
	fclose(fp);
}

void BlockClass::ResetAll(bool isDeleteAll)
{
	//blockList.erase(blockList.begin(), blockList.end());

	int num = 0;
	for (auto& i : blockList)
	{
		if (i->objType != objtype_system)
		{
			blockList.erase(blockList.begin() + num);
		}
		else
		{
			num++;
		}
	}

	if (isDeleteAll)
	{
		char temp[96] = {};
		FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_block), "wb");
		fclose(fp);
	}
}

void BlockClass::Delete(int index)
{
	blockList.erase(blockList.begin() + index);
}
void BlockClass::Create(vec3f setpos, vec3f setsize, bool setIsActive,
	int setobjDetailType, OBJECTTYPE setobjType,
	float setangle, vec3f setDir)
{
	new BlockClass(setpos, setsize, setIsActive, (BlockType)setobjDetailType, GetCurrentStage());
}

void BlockClass::DeleteByUniqueNumber(int unique)
{
	int cnt = 0;
	for (auto& i : blockList)
	{
		if (i->uniqueNumber == unique)
		{
			blockList.erase(blockList.begin() + cnt);
			return;
		}
		cnt++;
	}
}

void BlockClass::DeleteByType(BlockType deletetype)
{
	for (;;)
	{
		int cnt = 0;
		for (auto& i : blockList)
		{
			if (i->blockType == deletetype)
			{
				blockList.erase(blockList.begin() + cnt);
			}
			cnt++;
		}

		bool isbreak = true;
		for (auto& i : blockList)
		{
			if (i->blockType == deletetype)
			{
				isbreak = false;
				break;
			}
		}
		if (isbreak)
		{
			break;
		}
	}
}

void BlockClass::SetType(int type)
{
	if (type == -1)
	{
		blockType = (BlockType)((blockType + 1) % BlockType_MAX);
		return;
	}
	blockType = (BlockType)type;
}

void BlockClass::SetFallCountAdd(void)
{
	isSet = true;
	color = D3DCOLOR_RGBA(128,128,128,255);
}

void BlockClass::SetIsTouched(bool set)
{
	isTouched = set;
}

int BlockClass::GetDetailType(void)
{
	return blockType;
}

void BlockClass::DrawCube(const D3DXMATRIX& mtxModel, int texID)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	SetLight(true);

	pDevice->SetTransform(D3DTS_WORLD, &mtxModel);
	pDevice->SetFVF(FVF_BLOCK);

	if (texID <= -1)
		pDevice->SetTexture(0, NULL);
	else
		pDevice->SetTexture(0, Texture_GetTexture(texID));

	pDevice->SetIndices(BlockBaseClass::GetIndexBuffer());
	pDevice->SetStreamSource(0, BlockBaseClass::GetVerTexBuffer(), 0, sizeof(Vertex3D));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, BLOCK_VERTEX_MAX, 0, BLOCK_PRIMITIVE_MAX);

	SetLight(false);
}


//*****************************************************************************
// 関数定義（BlockBaseClass）
//*****************************************************************************
void BlockBaseClass::Init(void)
{
	//Vertex,IndexBufSet
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
		if (!pDevice) return;

		pDevice->CreateVertexBuffer(
			sizeof(Vertex3D) * BLOCK_VERTEX_MAX,
			D3DUSAGE_WRITEONLY,
			FVF_BLOCK,
			D3DPOOL_MANAGED,
			&pVertexBuffer,
			NULL
		);

		pDevice->CreateIndexBuffer(
			sizeof(Vertex3D) * BLOCK_INDEX_MAX,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pIndexBuffer,
			NULL
		);

		Vertex3D* pV;
		pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
		memcpy(pV, CubeVertex, sizeof(CubeVertex));
		pVertexBuffer->Unlock();

		WORD* pIndex;
		pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
		for (int i = 0; i < 6; i++)
		{
			pIndex[6 * i] = (WORD)(0 + 4 * i);
			pIndex[6 * i + 1] = (WORD)(1 + 4 * i);
			pIndex[6 * i + 2] = (WORD)(2 + 4 * i);
			pIndex[6 * i + 3] = (WORD)(1 + 4 * i);
			pIndex[6 * i + 4] = (WORD)(3 + 4 * i);
			pIndex[6 * i + 5] = (WORD)(2 + 4 * i);
		}
		pIndexBuffer->Unlock();
	}
}

void BlockBaseClass::Uninit(void)
{
	if (pIndexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = NULL;
	}
}

LPDIRECT3DVERTEXBUFFER9 BlockBaseClass::GetVerTexBuffer(void)
{
	return pVertexBuffer;
}

LPDIRECT3DINDEXBUFFER9 BlockBaseClass::GetIndexBuffer(void)
{
	return pIndexBuffer;
}

void BlockBaseClass::DrawBoxNotIndex(D3DXMATRIX mtx, int texID, D3DCOLOR setcolor)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	for (int i = 0; i < BLOCK_NOTINDEX_VERTEX_MAX; i++)
	{
		g_CubeVertex[i].Color = setcolor;
	}

	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	pDevice->SetFVF(FVF_BLOCK);

	if (texID == -1)
	{
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		pDevice->SetTexture(0, Texture_GetTexture(texID));
	}

	SetLight(false);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &g_CubeVertex, sizeof(Vertex3D));
	SetLight(false);
}

void BlockBaseClass::DrawBoxNotIndexforTex(D3DXMATRIX mtx, int texID,
	Int2 cut, D3DCOLOR setcolor)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	for (int i = 0; i < BLOCK_NOTINDEX_VERTEX_MAX; i++)
	{
		g_CubeVertex[i].Color = setcolor;
	}

	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	pDevice->SetFVF(FVF_BLOCK);

	//todo;テクスチャカット

	if (texID == -1)
	{
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		pDevice->SetTexture(0, Texture_GetTexture(texID));
	}

	SetLight(true);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &g_CubeVertex, sizeof(Vertex3D));
	SetLight(false);
}
