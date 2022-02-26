/*==============================================================================

   [GameObjectClass.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------

	memo:

==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "GameObjectClass.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define GAMEOBJECTCLASS_SAVEDATA_FILENAME ("asset/data/currentStageSaveData.dat")

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
GameObjectClass::STAGE GameObjectClass::currentStage;
int GameObjectClass::objectCount;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
const char* GameObjectClass::ObjectTypeName[] =
{
	{"block"},
	{"item"},
	{"deco"},
	{"player"},
	{"system"},
};

//*****************************************************************************
// 関数定義
//*****************************************************************************
GameObjectClass::GameObjectClass(vec3f setpos, vec3f setsize,
	bool setisActive, OBJECTTYPE settype, STAGE setstage)
{
	pos = setpos;
	size = setsize;
	isActive = setisActive;
	objType = settype;
	stageNumber = setstage;
	uniqueNumber = objectCount;

	objectCount++;
}

GameObjectClass::GameObjectClass()
{
	objectCount++;
}

GameObjectClass::~GameObjectClass()
{
	//DeleteAll();
}

void GameObjectClass::Init(void)
{
	objectCount = 0;
	currentStage = stage_1;
}

void GameObjectClass::SetCurrentStage(STAGE setstage)
{
	if (setstage == stage_MAX)
	{
		currentStage = (STAGE)((currentStage + 1) % stage_MAX);
		return;
	}
	currentStage = setstage;
}

void GameObjectClass::SetPos(vec3f setpos)
{
	pos = setpos;
}

void GameObjectClass::SetSize(vec3f setsize)
{
	size = setsize;
}

void GameObjectClass::SetIsActive(bool set)
{
	isActive = set;
}

void GameObjectClass::SetColor(D3DCOLOR setcolor)
{
	color = setcolor;
}

GameObjectClass::STAGE GameObjectClass::GetCurrentStage(void)
{
	return currentStage;
}

vec3f GameObjectClass::GetPos(void)
{
	return pos;
}

vec3f GameObjectClass::GetSize(void)
{
	return size;
}

bool GameObjectClass::GetIsActive(void)
{
	return isActive;
}

int GameObjectClass::GetUniqueNum(void)
{
	return uniqueNumber;
}

D3DCOLOR GameObjectClass::GetColor(void)
{
	return color;
}

GameObjectClass::STAGE GameObjectClass::GetStageNumber(void)
{
	return stageNumber;
}
GameObjectClass::OBJECTTYPE GameObjectClass::GetObjType(void)
{
	return objType;
}

char* GameObjectClass::MakeSaveDataFileName(char * Out, OBJECTTYPE objType)
{
	const char startText[] = {"asset/data/"};
	const char endText[] = {"_savedata.dat"};
	char temp[96] = {};

	const char* text[] = 
	{
		{"block"},
		{"item"},
		{"deco"},
		{"player"},
		{"system"},
	};

	sprintf(temp, "stage_%d/%s", GetCurrentStage() + 1, text[objType]);

	strcat(Out, startText);
	strcat(Out, temp);
	strcat(Out, endText);

	return Out;
}

Collision GameObjectClass::GetCollision(void)
{
	return collision;
}

void GameObjectClass::Move(DIRfor3D dir)
{
	switch (dir)
	{
	case dir3_top:
		pos.y += OBJECT_MOVE_SPEED;
		break;
	case dir3_under:
		pos.y -= OBJECT_MOVE_SPEED;
		break;
	case dir3_right:
		pos.x += OBJECT_MOVE_SPEED;
		break;
	case dir3_left:
		pos.x -= OBJECT_MOVE_SPEED;
		break;
	case dir3_forward:
		pos.z += OBJECT_MOVE_SPEED;
		break;
	case dir3_back:
		pos.z -= OBJECT_MOVE_SPEED;
		break;
	case dir3_MAX:
		break;
	default:
		break;
	}
}

void GameObjectClass::ChangeSize(DIRfor3D dir)
{
	switch (dir)
	{
	case dir3_top:
		size.y += OBJECT_MOVE_SPEED;
		break;
	case dir3_under:
		size.y -= OBJECT_MOVE_SPEED;
		break;
	case dir3_right:
		size.x += OBJECT_MOVE_SPEED;
		break;
	case dir3_left:
		size.x -= OBJECT_MOVE_SPEED;
		break;
	case dir3_forward:
		size.z -= OBJECT_MOVE_SPEED;
		break;
	case dir3_back:
		size.z += OBJECT_MOVE_SPEED;
		break;
	default:
		break;
	}
}

void GameObjectClass::ChangeIsActive(void)
{
	isActive = (isActive + 1) % 2;
}

void GameObjectClass::ResetStatus(void)
{
	pos = {};
	size = { 1.0f,1.0f,1.0f };
	isActive = true;
	stageNumber = GetCurrentStage();
	color = COLOR_NORMAL;
}

void GameObjectClass::Save(FILE * fp, int setobjtypedetail)
{
	fwrite(&pos, sizeof(vec3f), 1, fp);
	fwrite(&size, sizeof(vec3f), 1, fp);
	fwrite(&isActive, sizeof(bool), 1, fp);
	fwrite(&setobjtypedetail, sizeof(int), 1, fp);//オブジェクトの細かいタイプ
	fwrite(&stageNumber, sizeof(STAGE), 1, fp);
}
