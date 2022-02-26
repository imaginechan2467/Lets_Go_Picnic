/*==============================================================================

   [Player.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Player.h"
#include "Collision.h"
#include "Block.h"
#include "mydirect3D.h"
#include "Light.h"
#include "texture.h"
#include "Game.h"
#include "Camera.h"
#include "Keylogger.h"
#include "Item.h"
#include "Shade.h"
#include "Facegen.h"
#include "Effect.h"
#include "sound.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define MOVE_SPEED (0.07f)
#define PLAYER_NORMAL_SIZE (vec3f(0.7f,0.7f,0.7f))
#define PLAYER_START_POS (vec3f(0.0f,5.0f,0.0f))
#define NEARLY_ZERO_VALUE (0.1f)
#define ROTATION_SPEED (10.0f)
#define HIT_BACK_SIZE (0.01f)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int PlayerClass::playerTexture;
int PlayerClass::HPTexture;
bool PlayerClass::isTouchGround;
int PlayerClass::takenCoinNum;
int PlayerClass::coinUITex;


//*****************************************************************************
// 関数定義
//*****************************************************************************
PlayerClass::PlayerClass()
	:GameObjectClass(PLAYER_START_POS, PLAYER_NORMAL_SIZE,true,objtype_player,GetCurrentStage())
{
	takenCoinNum = 0;
	isTouchGround = false;
	weight = 1.0f;
	HP = 3;
	moveState = movestate_walk;
	fallSpeed = 0.0f;
	pos = PLAYER_START_POS;
	size = PLAYER_NORMAL_SIZE;
	onFloorIndex = 0;
	oldMoveDir = dir3_MAX;
	front = { 0.0f,0.0f,-1.0f };
	playerGravity = GRAVITY;
	oldShadePos = vec3f_NULL;

	isActive = true;
}

PlayerClass::~PlayerClass()
{
}

void PlayerClass::Init(void)
{
	playerTexture = SetTexture("asset/texture/player_skin_2.png", {3,2});
	HPTexture = SetTexture("asset/texture/ui_heart.png");
	coinUITex = SetTexture("asset/texture/ui_coin.png", {2,1});
	TextureLoad("プレイヤー");
}

void PlayerClass::Uninit(void)
{
	TextureRelease(&playerTexture, 1);
	TextureRelease(&HPTexture, 1);
	TextureRelease(&coinUITex, 1);

	playerTexture = NULL;
	HPTexture = NULL;
	coinUITex = NULL;
}

void PlayerClass::Update(void)
{
	DIRfor3D tempdir;
	if (isTouchGround)
	{
		fallSpeed = 0.0f;
		tempdir = dir3_under;
	}
	else
	{
		//重力を足す
		if(ItemClass::GetIsActiveUmbrella() && fallSpeed < 0.0f)
		{
			fallSpeed -= playerGravity;
		}
		else
		{
			fallSpeed -= GRAVITY;
		}

		pos.y += fallSpeed;
		if (fallSpeed > 0)
		{
			tempdir = dir3_top;
		}
		else
		{
			tempdir = dir3_under;
		}
	}

	//todo:ステージによって高さを変える

	float underpos = GetCurrentStage() == stage_3 ? -20.0f : -3.0f;
	if (pos.y <= underpos)
	{
		ResetStatus();

		if (!GameSystem::GetIsClear() || !GameSystem::GetIsGameOver())
		{
			HP -= 1;
			PlaySound(se_fall);
			if (HP <= 0)
			{
				GameSystem::SetIsGameOver(true);
			}
		}
	}

	//ブロック判定
	bool ishitfloor = false;
	for (auto& i : BlockClass::blockList)
	{
		if (!i->GetIsActive() || i->GetObjType() == GameObjectClass::objtype_system)continue;

		Collision::ColInfo_2 info = 
			Collision::GetBoxVertexHit(pos, size, i->GetPos(), i->GetSize(), tempdir);

		if (info.hitvertex[cubedir_RUF] || info.hitvertex[cubedir_RUB] ||
			info.hitvertex[cubedir_LUF] || info.hitvertex[cubedir_LUB])
		{
			//if (i->GetDetailType() == BlockClass::BlockType_fire)//マグマ
			//{
			//	PlaySound(se_burn);
			//	HP -= 1;
			//	if (HP <= 0)
			//	{
			//		GameSystem::SetIsGameOver(true);
			//	}
			//	ResetStatus();
			//	break;
			//}
			if (tempdir == dir3_top)
			{
				pos.y = info.adjustpos.y - HIT_BACK_SIZE;
				isTouchGround = false;
				break;
			}
			else if (tempdir == dir3_under)
			{
				if (i->GetDetailType() == BlockClass::BlockType_goal)//クリア
					GameSystem::SetIsClear(true);

				if (i->GetDetailType() == BlockClass::BlockType_fall)//落ちる床
					i->SetFallCountAdd();

				pos.y = info.adjustpos.y;
				isTouchGround = true;
				ishitfloor = true;

				if (ItemClass::GetIsActiveUmbrella())
				{
					ItemClass::SetUmbrella(false);
					playerGravity = GRAVITY;
				}
				break;
			}
		}
	}
	if (!ishitfloor) isTouchGround = false;

	//アイテム判定
	for (auto& i : ItemClass::itemList)
	{
		if (!i->GetIsActive() || i->GetObjType() == GameObjectClass::objtype_system)continue;

		Collision::ColInfo_2 info =
			Collision::GetBoxVertexHit(pos, size,
				i->GetPos(), { 1.0f, 1.0f, 1.0f }, dir3_under);

		//下面との判定
		if ((info.hitvertex[cubedir_RUF] || info.hitvertex[cubedir_RUB] ||
			info.hitvertex[cubedir_LUF] || info.hitvertex[cubedir_LUB]) && tempdir == dir3_under)
		{
			if (i->GetDetailType() == ItemClass::ItemType_spring)
			{
				PlaySound(se_jump);
				isTouchGround = false;
				fallSpeed = ITEM_SPRING_JUMP_SIZE;
			}
		}

		for (int k = 0; k < cubedir_MAX; k++)
		{
			if (info.hitvertex[k])
			{
				if (i->GetDetailType() == ItemClass::ItemType_coin)
				{
					//コインUI等
					ItemClass::DeleteByUniqueNumber(i->GetUniqueNum());
					takenCoinNum++;
					new EffectClass({ SCREEN_WIDTH - (3 - takenCoinNum) * 64.0f,0.0f,0.0f },
						{ 64.0f,64.0f }, eftype_shy, 3, efact_none, 0, COLOR_RED);
					PlaySound(se_coin);
				}
				break;
			}
		}
	}
}

void PlayerClass::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	SetLight(true);

	D3DXMATRIX mtxW, mtxT, mtxR, mtxS;

	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	D3DXMatrixScaling(&mtxS, size.x, size.y, size.z);
	mtxW = mtxS * mtxT;

	pDevice->SetTransform(D3DTS_WORLD, &mtxW);
	pDevice->SetFVF(FVF_BLOCK);
	pDevice->SetTexture(0, Texture_GetTexture(playerTexture));

	pDevice->SetIndices(BlockBaseClass::GetIndexBuffer());
	pDevice->SetStreamSource(0, BlockBaseClass::GetVerTexBuffer(), 0, sizeof(Vertex3D));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, BLOCK_VERTEX_MAX, 0, BLOCK_PRIMITIVE_MAX);

	SetLight(false);


	if (!isTouchGround)
	{
		//影描画
		bool tempishit = false;
		vec3f temppos = pos;
		for (auto& i : BlockClass::blockList)
		{
			if ((i->GetPos().x < pos.x - size.x * 0.5f || i->GetPos().x > pos.x + size.x * 0.5f) ||
				(i->GetPos().z < pos.z - size.z * 0.5f || i->GetPos().z > pos.z + size.z * 0.5f))continue;

			for (int k = (int)round(pos.y); k > -5; k--)
			{
				Collision::CollisionCircle A = { {temppos.x,(float)k,temppos.z},0.5f };
				Collision::CollisionCircle B = { i->GetPos(),0.5f };

				if (Collision::CollisionCircleAndCircleHit(&A, &B))
				{
					temppos = pos;
					temppos.y = i->GetPos().y + i->GetSize().y * 0.5f + 0.01f;
					oldShadePos = temppos;
					tempishit = true;
					break;
				}
			}
			if (tempishit) break;
		}
		//ヒットしたらその場所に描画、ヒットしなかったら前の場所に描画
		if (tempishit)
		{
			ShadeDraw(temppos, SHADE_DEFAULT_SIZE);
		}
		else
		{
			ShadeDraw(oldShadePos, SHADE_DEFAULT_SIZE);
		}
	}

	//HP描画
	for (int i = 0; i < HP; i++)
	{
		FaceGen({ 32.0f * i,0.0f }, { 32.0f,32.0f }, true, HPTexture);
	}

	for (int i = 0; i < 3; i++)
	{
		if (i < takenCoinNum)
		{
			FaceGenforTex({ SCREEN_WIDTH - 64.0f * (3 - i),0.0f }, { 64.0f,64.0f }, true, coinUITex, 1);
		}
		else
		{
			FaceGenforTex({ SCREEN_WIDTH - 64.0f * (3 - i),0.0f }, {64.0f,64.0f }, true, coinUITex, 0);
		}
	}
}

void PlayerClass::UpdateAll(void)
{
}

void PlayerClass::DrawAll(void)
{
}

void PlayerClass::Create(vec3f setpos, vec3f setsize, bool setIsActive, int setobjDetailType, OBJECTTYPE setobjType, float setangle, vec3f setDir)
{
}

void PlayerClass::Delete(int index)
{
}

void PlayerClass::Load(bool isreset)
{
}

void PlayerClass::SetType(int type)
{
}

void PlayerClass::ResetAll(bool isalldelete)
{
}

void PlayerClass::ResetStatus(void)
{
	pos = PLAYER_START_POS;
	size = PLAYER_NORMAL_SIZE;
	playerType = playerType_normal;
	float weight = 0.1f;
	fallSpeed = 0.0f;
	isTouchGround = false;
	/*ItemClass::SetUmbrella(false);
	playerGravity = GRAVITY;*/
}

void PlayerClass::SetOnFloorIndex(int set)
{
	onFloorIndex = set;
}

void PlayerClass::SetGravity(bool isnormal)
{
	if (isnormal)
	{
		playerGravity = GRAVITY;
	}
	else
	{
		playerGravity = GRAVITY_FOR_UMBRELLA;
	}
}

void PlayerClass::Move(DIRfor3D dir)
{
	vec3f temp = {};
	vec3f oldpos = pos;

	switch (dir)
	{
	case dir3_forward:
		temp = CameraClass::GetFront();
		temp.y = 0.0f;
		D3DXVec3Normalize(&temp, &temp);
		pos += temp * 0.1f;
		break;
	case dir3_back:
		temp = CameraClass::GetFront();
		temp.y = 0.0f;
		D3DXVec3Normalize(&temp, &temp);
		pos += temp * -0.1f;
		break;
	case dir3_right:
		temp = CameraClass::GetRight();
		temp.y = 0.0f;
		D3DXVec3Normalize(&temp, &temp);
		pos += temp * 0.1f;
		break;
	case dir3_left:
		temp = CameraClass::GetRight();
		temp.y = 0.0f;
		D3DXVec3Normalize(&temp, &temp);
		pos += temp * -0.1f;
		break;
	case dir3_top:
		if (isTouchGround)
		{
			fallSpeed = 0.2f;
			isTouchGround = false;
			oldMoveDir = dir3_top;
		}
	default:
		break;
	}

	vec3f tempcmp = pos - oldpos;
	bool tempisminus[3] = {false,false,false};//XYZの順
	if (tempcmp.x < 0.0f)
	{
		tempcmp.x *= -1;
		tempisminus[0] = true;
	}
	if (tempcmp.y < 0.0f)
	{
		tempcmp.y *= -1;
		tempisminus[1] = true;
	}
	if (tempcmp.z < 0.0f)
	{
		tempcmp.z *= -1;
		tempisminus[2] = true;
	}

	int tempdirmax = -1;//XYZの順
	for (int i = 0; i < 3; i++)
	{
		if (tempcmp[i] > tempcmp[(i + 1) % 3])
		{
			tempdirmax = i;
		}
	}

	if (tempdirmax == 0)//X
	{
		if (tempisminus[0])
		{
			oldMoveDir = dir3_left;
		}
		else
		{
			oldMoveDir = dir3_right;
		}
	}
	else if (tempdirmax == 1)//Y
	{
		if (tempisminus[1])
		{
			oldMoveDir = dir3_under;
		}
		else
		{
			oldMoveDir = dir3_top;
		}
	}
	else if(tempdirmax == 2)//Z
	{
		if (tempisminus[2])
		{
			oldMoveDir = dir3_back;
		}
		else
		{
			oldMoveDir = dir3_forward;
		}
	}

	bool ishitlava = false;
	for (auto& i : BlockClass::blockList)
	{
		Collision::ColInfo_2 info =
			Collision::GetBoxVertexHit(pos, size, i->GetPos(), i->GetSize(), oldMoveDir);

		if ((((info.hitvertex[cubedir_RUF] || info.hitvertex[cubedir_RUB]) && !isTouchGround) ||
			(info.hitvertex[cubedir_RTF] || info.hitvertex[cubedir_RTB])) &&//右の壁との判定
			oldMoveDir == dir3_right)
		{
			if (i->GetDetailType() == BlockClass::BlockType_fire)
			{
				ishitlava = true;
				break;
			}
			pos.x = info.adjustpos.x - HIT_BACK_SIZE;
		}
		if ((((info.hitvertex[cubedir_LUF] || info.hitvertex[cubedir_LUB]) && !isTouchGround) ||
			(info.hitvertex[cubedir_LTF] || info.hitvertex[cubedir_LTB])) &&//左の壁との判定
			oldMoveDir == dir3_left)
		{
			if (i->GetDetailType() == BlockClass::BlockType_fire)
			{
				ishitlava = true;
				break;
			}
			pos.x = info.adjustpos.x + HIT_BACK_SIZE;
		}
		if ((((info.hitvertex[cubedir_RUB] || info.hitvertex[cubedir_LUB]) && !isTouchGround) ||
			(info.hitvertex[cubedir_RTB] || info.hitvertex[cubedir_LTB])) &&//奥の壁との判定
			oldMoveDir == dir3_forward)
		{
			if (i->GetDetailType() == BlockClass::BlockType_fire)
			{
				ishitlava = true;
				break;
			}
 			pos.z = info.adjustpos.z - HIT_BACK_SIZE;
		}
		if ((((info.hitvertex[cubedir_RUF] || info.hitvertex[cubedir_LUF]) && !isTouchGround) ||
			(info.hitvertex[cubedir_RTF] || info.hitvertex[cubedir_LTF])) &&//手前の壁との判定
			oldMoveDir == dir3_back)
		{
			if (i->GetDetailType() == BlockClass::BlockType_fire)
			{
				ishitlava = true;
				break;
			}
			pos.z = info.adjustpos.z + HIT_BACK_SIZE;
		}
	}
	if (ishitlava)
	{
		PlaySound(se_burn);
		HP -= 1;
		if (HP <= 0)
		{
			GameSystem::SetIsGameOver(true);
		}
		ResetStatus();
	}
}

int PlayerClass::GetDetailType(void)
{
	return 0;
}

int PlayerClass::GetOnFloorIndex(void)
{
	return onFloorIndex;
}

bool PlayerClass::GetIsOnFloor(void)
{
	return isTouchGround;
}

DIRfor3D PlayerClass::GetPlayerMoveDir(void)
{
	return oldMoveDir;
}

bool PlayerClass::GetIsUp(void)
{
	return fallSpeed > 0.0f ? true : false;
}

int PlayerClass::GetTakenCoinNum(void)
{
	return takenCoinNum;
}
