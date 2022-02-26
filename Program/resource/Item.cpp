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
#include "Item.h"
#include "mydirect3D.h"
#include "Config.h"
#include "texture.h"
#include "Light.h"
#include "Xmodel.h"
#include "Game.h"
#include "Player.h"
#include "Block.h"
#include "Facegen.h"
#include "TextGen.h"
#include "Effect.h"
#include "sound.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define ITEM_SIZE_CHANGE_SPEED (0.1f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
std::vector<ItemClass*> ItemClass::itemList;
int ItemClass::itemModel[ItemClass::ItemType_MAX] = {};
int ItemClass::iteminventry[ItemClass::ItemType_MAX] = {};
int ItemClass::bombUiTexture;
ItemClass* ItemClass::pUmbrella;;

//*****************************************************************************
// 関数定義
//*****************************************************************************
ItemClass::ItemClass()
	:GameObjectClass({ 0.0f,0.0f,0.0f }, { 0.25f,0.25f,0.25f }, true, objtype_system, GetCurrentStage())
{
	pos = { 0.0f,0.0f,0.0f };
	size = { 0.25f,0.25f,0.25f };
	isActive = true;
	objType = objtype_system;
	color = COLOR_NORMAL;
	stageNumber = GetCurrentStage();
	itemType = (ItemType)0;
	cnt = 0;
	frame = 0;
	waitCnt = 0;
	D3DXMatrixScaling(&mtxModel, 0.25f, 0.25f, 0.25f);
	D3DXMatrixIdentity(&mtxModel);
	angle = 0.0f;
	AxisDir = { 0.0f,0.0f,0.0f };
	isSet = false;

	SetMatrix();

	itemList.push_back(this);
}

ItemClass::ItemClass(vec3f setpos, vec3f setsize, bool setisActive,
	ItemType settype, STAGE setstagenum, float setangle, vec3f setRotationDir)
	:GameObjectClass(setpos, setsize, setisActive, objtype_item, setstagenum)
{
	pos = setpos;
	size = setsize;
	isActive = setisActive;
	itemType = settype;
	stageNumber = setstagenum;
	color = COLOR_NORMAL;

	isSet = false;
	objType = objtype_item;
	cnt = 0;
	frame = 0;
	waitCnt = 0;
	angle = setangle;
	AxisDir = setRotationDir;

	SetMatrix();

	itemList.push_back(this);
}

ItemClass::~ItemClass()
{

}

void ItemClass::Init(void)
{
	itemModel[ItemType_spring] = XModelLoad("asset/model/spring.x");
	itemModel[ItemType_bomb] = XModelLoad("asset/model/bomb.x");
	itemModel[ItemType_umbrella] = XModelLoad("asset/model/umbrella.x");
	itemModel[ItemType_coin] = XModelLoad("asset/model/coin.x");
	itemModel[ItemType_goalpost] = XModelLoad("asset/model/goal.x");

	bombUiTexture = SetTexture("asset/texture/ui_bomb.png");
	TextureLoad("爆弾UI");

	//todo:ステージごとに保存してロード
	memset(iteminventry, 0, sizeof(iteminventry));
	iteminventry[ItemType_bomb] = 12;
	
	//デバッグ表示
	int num = 0;
	for (auto& i : itemModel)
	{
		if (i == XMODEL_INVALID_ID)
		{
			const char* errortext[] =
			{
				{"ボタン"},
				{"ジャンプ"},
				{"トランポリン"},
				{"爆弾"},
				{"傘"},
				{"樽"},
				{"ゴール"},
			};
			char disptext[96] = {};
			sprintf(disptext,"%s 読み込み失敗",errortext[num]);

			MessageBox(NULL, disptext, "エラー", MB_OK);
		}
		num++;
	}
}

void ItemClass::Uninit(void)
{
	//モデル開放
	XModelRelease(itemModel, ItemType_MAX);

	TextureRelease(&bombUiTexture, 1);
	bombUiTexture = NULL;

	delete pUmbrella;
	pUmbrella = NULL;

	//配列削除
	itemList.erase(itemList.begin(), itemList.end());
}

void ItemClass::UpdateAll(void)
{
	for (auto& i : itemList)
	{
		i->Update();
	}
}

void ItemClass::DrawAll(void)
{
	for (auto& i : itemList)
	{
		if (!i->isActive)continue;

		i->Draw();
	}

	//爆弾UI
	FaceGen(ITEM_BOMB_UI_POS, ITEM_BOMB_UI_SIZE, true, bombUiTexture);

	//爆弾個数
	char temp[16] = {};
	sprintf(temp, "%d", iteminventry[ItemType_bomb]);
	TextGenforEng(ITEM_BOMB_CNT_POS, TEXTSIZE_NORMAL, temp);
}

void ItemClass::Update(void)
{
	D3DXMATRIX mtxR, mtxS;

	switch (itemType)
	{
	case ItemClass::ItemType_spring:
		break;
	case ItemClass::ItemType_bomb:

		cnt++;
		if (cnt >= ITEM_BOMB_COUNT)
		{
			cnt = 0;

			PlaySound(se_bomb);

			//爆発エフェクト
			EffectRandomClass::SetEffectRandomforSecond(eftype_moya, 5, 60 * 2, {2,2,2}, true, { 8.0f,8.0f }, pos);

			//範囲の岩ブロックを壊す
			for (auto& i : BlockClass::blockList)
			{
				if (i->GetDetailType() != BlockClass::BlockType_rock)continue;

				Collision::CollisionCircle A = {pos,1.5f};
				Collision::CollisionCircle B = {i->GetPos(),0.5f};
				if(Collision::CollisionCircleAndCircleHit(&A,&B))
				{
					BlockClass::DeleteByUniqueNumber(i->GetUniqueNum());
				}
			}

			//削除
			DeleteByUniqueNumber(uniqueNumber);
		}

		break;
	case ItemClass::ItemType_umbrella:

		break;
	case ItemClass::ItemType_coin:
	case ItemClass::ItemType_goalpost:

		angle += 0.05f;
		AxisDir = {0.0f,1.0f,0.0f};

		break;
	default:
		break;
	}

	SetMatrix();
}

void ItemClass::Draw(void)
{
	if (/*(GameSystem::GetIsCameraMode() && objType == objtype_system) ||*/
		(GameSystem::GetCurrentMakerType() != objtype_item && objType == objtype_system))return;

	if (isActive)
	{
		XModelDraw(itemModel[itemType], &mtxModel);
	}

	//debug表示
	D3DXMATRIX mtxW,mtxS,mtxT;
	D3DXMatrixScaling(&mtxS, size.x + 0.5f, size.y + 0.5f, size.z + 0.5f);
	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	mtxW = mtxS * mtxT;

	//BlockBaseClass::DrawBoxNotIndex(mtxW,-1,COLOR_RED);
}

void ItemClass::SaveAll(void)
{
	char temp[96] = {};
	FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_item), "wb");

	//個数保存
	int num = itemList.size();
	fwrite(&num, sizeof(int), 1, fp);

	//セーブ
	for (auto& i : itemList)
	{
		if (i->objType == GameObjectClass::objtype_system)continue;

		i->Save(fp, i->itemType);
	}

	fclose(fp);
}

void ItemClass::Load(bool isreset)
{
	char temp[96] = {};
	FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_item), "rb");
	if (!fp)
	{
		MessageBox(NULL, "アイテムセーブデータファイルが開けませんでした", "エラー", MB_OK);
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
	for (int i = 0; i < num; i++)
	{
		vec3f setpos = {};
		vec3f setsize = {};
		bool setisActive = {};
		ItemType setItemType = {};
		float setAngle = {};
		vec3f setDir = {};

		STAGE setstagenum = {};

		fread(&setpos, sizeof(vec3f), 1, fp);
		fread(&setsize, sizeof(vec3f), 1, fp);
		fread(&setisActive, sizeof(bool), 1, fp);
		fread(&setItemType, sizeof(int), 1, fp);
		fread(&setstagenum, sizeof(STAGE), 1, fp);
		fread(&setAngle, sizeof(float), 1, fp);

		new ItemClass(setpos, setsize, setisActive, setItemType, setstagenum, setAngle, setDir);
	}
	fclose(fp);
}

void ItemClass::Save(FILE * fp, int setobjtypedetail)
{
	fwrite(&pos, sizeof(vec3f), 1, fp);
	fwrite(&size, sizeof(vec3f), 1, fp);
	fwrite(&isActive, sizeof(bool), 1, fp);
	fwrite(&setobjtypedetail, sizeof(int), 1, fp);//オブジェクトの細かいタイプ
	fwrite(&stageNumber, sizeof(STAGE), 1, fp);
	fwrite(&angle, sizeof(float), 1, fp);
}

void ItemClass::ResetAll(bool isDeleteAll)
{
	itemList.erase(itemList.begin(), itemList.end());

	if (isDeleteAll)
	{
		char temp[96] = {};
		FILE* fp = fopen(MakeSaveDataFileName(temp, objtype_item), "wb");
		fclose(fp);
	}
}

void ItemClass::Delete(int index)
{
	itemList.erase(itemList.begin() + index);
}
void ItemClass::Create(vec3f setpos, vec3f setsize, bool setIsActive,
	int setobjDetailType, OBJECTTYPE setobjType, float setangle, vec3f setRotationDir)
{
	new ItemClass(setpos, setsize, setIsActive,
		(ItemType)setobjDetailType, GetCurrentStage(), setangle, setRotationDir);
}

void ItemClass::DeleteByUniqueNumber(int unique)
{
	int cnt = 0;
	for (auto& i : itemList)
	{
		if (i->uniqueNumber == unique)
		{
			itemList.erase(itemList.begin() + cnt);
			return;
		}
		cnt++;
	}
}

void ItemClass::Move(DIRfor3D dir)
{
	switch (dir)
	{
	case dir3_top:
		pos.y += ITEM_SIZE_CHANGE_SPEED;
		break;
	case dir3_under:
		pos.y -= ITEM_SIZE_CHANGE_SPEED;
		break;
	case dir3_right:
		pos.x += ITEM_SIZE_CHANGE_SPEED;
		break;
	case dir3_left:
		pos.x -= ITEM_SIZE_CHANGE_SPEED;
		break;
	case dir3_forward:
		pos.z += ITEM_SIZE_CHANGE_SPEED;
		break;
	case dir3_back:
		pos.z -= ITEM_SIZE_CHANGE_SPEED;
		break;
	default:
		break;
	}
}

void ItemClass::SetType(int type)
{
	if (type == -1)
	{
		itemType = (ItemType)((itemType + 1) % ItemType_MAX);
		return;
	}
	itemType = (ItemType)type;
}

void ItemClass::SetMatrix(void)
{
	D3DXMATRIX mtxT, mtxR, mtxS;
	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&mtxS, size.x, size.y, size.z);
	D3DXMatrixRotationAxis(&mtxR, &AxisDir, angle);
	mtxModel = mtxS * mtxR * mtxT;
}

void ItemClass::ChangeSize(DIRfor3D dir)
{
	if (dir == dir3_forward || dir == dir3_right)
	{
		size.x += ITEM_SIZE_CHANGE_SPEED;
		size.y += ITEM_SIZE_CHANGE_SPEED;
		size.z += ITEM_SIZE_CHANGE_SPEED;
	}
	else if(dir == dir3_back)
	{
		size.x -= ITEM_SIZE_CHANGE_SPEED;
		size.y -= ITEM_SIZE_CHANGE_SPEED;
		size.z -= ITEM_SIZE_CHANGE_SPEED;
		if (size.x < 0.0f) size.x = 0.0f;
		if (size.y < 0.0f) size.y = 0.0f;
		if (size.z < 0.0f) size.z = 0.0f;
	}
}

void ItemClass::SetBomb(vec3f setpos)
{
	if (iteminventry[ItemType_bomb] >= 1)
	{
		iteminventry[ItemType_bomb]--;

		new ItemClass(setpos, ITEM_START_SIZE, true, ItemType_bomb, GetCurrentStage(), 0.0f, {});
	}
}

void ItemClass::SetUmbrella(bool isuse)
{
	if (isuse)
	{
		GetUmbrella()->isActive = true;
	}
	else
	{
		GetUmbrella()->isActive = false;
	}
}

void ItemClass::UmbrellaSetPos(vec3f setpos)
{
	GetUmbrella()->pos = setpos;
}

int ItemClass::GetDetailType(void)
{
	return itemType;
}

bool ItemClass::GetIsActiveUmbrella(void)
{
	return GetUmbrella()->isActive;
}

ItemClass * ItemClass::GetUmbrella(void)
{
	if (!pUmbrella)
	{
		pUmbrella = new ItemClass({}, { 1.0f,1.0f,1.0f },
			false, ItemType_umbrella, GetCurrentStage(), 0.0f, {});
	}
	return pUmbrella;
}
