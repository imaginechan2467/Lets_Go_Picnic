/*==============================================================================

   [Facegen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Camera.h"
#include "mydirect3D.h"
#include "Keylogger.h"
#include "TextGen.h"
#include <stdio.h>
#include "Game.h"
#include "Player.h"
#include "sound.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define CAMERA_DEBUG_MOVE_SPEED (0.1f)
#define CAMERA_MOVE_SPEED (0.1f)

#define MOVING_SPEED_MAX (0.1f)
#define MOVING_ACCEL (0.05f)
#define MOVING_ATTENUATION_RATE (0.9f)
#define CAMERA_MAX (6)
#define TPS_OFFSET (10.0f)

#define VRIGHT (vec3f{1.0f,0.0f,0.0f})

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
vec3f CameraClass::pos;
vec3f CameraClass::vAt;
vec3f CameraClass::vUp;
vec3f CameraClass::vFront;
vec3f CameraClass::vRight;
D3DXMATRIX CameraClass::mtxView;

bool CameraClass::isFps;
float CameraClass::distance;

//*****************************************************************************
// 関数定義
//*****************************************************************************

CameraClass::CameraClass()
{
	vFront = { 0.0f,0.0f,1.0f };
	vRight = { 1.0f,0.0f,0.0f };

	D3DXVec3Normalize(&vFront, &vFront);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vFront, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vAt = { 0.0f,0.0f,0.0f };
	vUp = { 0.0f,1.0f,0.0f };
	pos = { 0.0f,2.0f,-10.0f };
	isActive = true;
	frontDir = dir_top;

	SetViewMatrix();
}

CameraClass::~CameraClass()
{

}

void CameraClass::Init(void)
{
	isFps = false;
	distance = TPS_OFFSET;
}

void CameraClass::Update(void)
{
	if (GameSystem::GetIsPlayMode())
	{
		if (KeyloggerPress(KL_UP))
		{
			Role(dir_top, true);
		}
		if (KeyloggerPress(KL_DOWN))
		{
			Role(dir_under, true);
		}
		if (KeyloggerTrigger(KL_RIGHT))
		{
			Role(dir_right, true);
			PlaySound(se_swap1);
		}
		if (KeyloggerTrigger(KL_LEFT))
		{
			Role(dir_left, true);
			PlaySound(se_swap1);
		}
	}
	else
	{
		if (KeyloggerPress(KL_W)) Move(dir3_forward);

		if (KeyloggerPress(KL_S)) Move(dir3_back);

		if (KeyloggerPress(KL_D)) Move(dir3_right);

		if (KeyloggerPress(KL_A)) Move(dir3_left);

		if (KeyloggerPress(KL_E)) Move(dir3_top);

		if (KeyloggerPress(KL_Q)) Move(dir3_under);

		//if (KeyloggerTrigger(KL_SPACE)) g_camera->Refresh();//位置・方向・初期化

		if (KeyloggerPress(KL_UP)) Role(dir_top,false);

		if (KeyloggerPress(KL_DOWN)) Role(dir_under, false);

		if (KeyloggerPress(KL_RIGHT)) Role(dir_right, false);

		if (KeyloggerPress(KL_LEFT)) Role(dir_left, false);

		//if (KeyloggerTrigger(KL_1)) SetAngleByDir(dir_top);//位置変更

		//if (KeyloggerTrigger(KL_2)) SetAngleByDir(dir_right);

		//if (KeyloggerTrigger(KL_3)) SetAngleByDir(dir_left);

		//if (KeyloggerTrigger(KL_4)) SetAngleByDir(dir_under);
	}
}

void CameraClass::Uninit(void)
{

}

void CameraClass::Draw(void)
{
}

void CameraClass::SetViewMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (pDevice == NULL)return;

	//ビュー座標変換業行列の作成
	D3DXMATRIX mtxR;

	//注視点を算出する
	vAt = pos + vFront * 2.5f;//注視点
	D3DXMatrixLookAtLH(&mtxView, &pos, &vAt, &vUp);

	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション（パースペクティブ）変換行列の作成
	D3DXMATRIX mtxProjection;
	float fov_y = D3DXToRadian(60);
	float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	float z_near = 0.1f;
	float z_far = 100.0f;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, fov_y, aspect, z_near, z_far);
	//デバイスにプロジェクション変換行列を設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

void CameraClass::Move(DIRfor3D setdir)
{
	vec3f dir;
	switch (setdir)
	{
	case dir3_top:
		pos.y += CAMERA_DEBUG_MOVE_SPEED;
		break;
	case dir3_under:
		pos.y -= CAMERA_DEBUG_MOVE_SPEED;
		break;
	case dir3_right:
		dir = vRight;
		dir.y = 0.0f;
		D3DXVec3Normalize(&dir, &dir);
		pos += dir * 0.1f;
		break;
	case dir3_left:
		dir = vRight;
		dir.y = 0.0f;
		D3DXVec3Normalize(&dir, &dir);
		pos -= dir * 0.1f;
		break;
	case dir3_forward:
		dir = vFront;
		dir.y = 0.0f;
		D3DXVec3Normalize(&dir, &dir);
		pos += dir * 0.1f;
		break;
	case dir3_back:
		dir = vFront;
		dir.y = 0.0f;
		D3DXVec3Normalize(&dir, &dir);
		pos -= dir * 0.1f;
		break;
	default:
		break;
	}
}

void CameraClass::Role(DIR setdir, bool isFollowPlayer)
{
	D3DXMATRIX mtxR;
	switch (setdir)
	{
	case dir_top:
		if (isFollowPlayer) 
		{
			vAt = pos - vFront * distance;

			D3DXMatrixRotationAxis(&mtxR, &vRight, ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

			pos = vAt - vFront * distance;
		}
		else
		{
			D3DXMatrixRotationAxis(&mtxR, &vRight, ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);
		}
		break;
	case dir_under:

		if (isFollowPlayer)
		{
			vAt = pos - vFront * distance;

			D3DXMatrixRotationAxis(&mtxR, &vRight, -ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

			pos = vAt - vFront * distance;
		}
		else
		{
			D3DXMatrixRotationAxis(&mtxR, &vRight, -ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);
		}

		break;
	case dir_right:

		if (isFollowPlayer)
		{
			vAt = pos - vFront * distance;
			D3DXMatrixRotationY(&mtxR, D3DXToRadian(90));

			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

			pos = vAt - vFront * distance;
		}
		else
		{
			D3DXMatrixRotationY(&mtxR, ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);
		}

		break;
	case dir_left:

		if (isFollowPlayer)
		{
			vAt = pos - vFront * distance;
			D3DXMatrixRotationY(&mtxR, -D3DXToRadian(90));

			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

			pos = vAt - vFront * distance;
		}
		else
		{
			D3DXMatrixRotationY(&mtxR, -ROTATION_SPEED_FOR_PLAY);
			D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
			D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
			D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);
		}

		break;
	default:
		break;
	}
}

void CameraClass::RoleforClear(DIR setdir)
{
	D3DXMATRIX mtxR;
	if (setdir == dir_right)
	{
		vAt = pos - vFront * distance;
		D3DXMatrixRotationY(&mtxR, CAMERA_ROTATION_SPEED);

		D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
		D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
		D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

		pos = vAt - vFront * distance;
	}
	else
	{
		vAt = pos - vFront * distance;
		D3DXMatrixRotationY(&mtxR, -CAMERA_ROTATION_SPEED);

		D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
		D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
		D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

		pos = vAt - vFront * distance;
	}
}

void CameraClass::Refresh(void)
{
	vFront = { 0.0f,0.0f,1.0f };
	vRight = { 1.0f,0.0f,0.0f };

	D3DXVec3Normalize(&vFront, &vFront);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vFront, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vAt = { 0.0f,0.0f,0.0f };
	vUp = { 0.0f,1.0f,0.0f };
	pos = { 0.0f,0.0f,-10.0f };
}

void CameraClass::SetAngleByDir(DIR setdir)
{
	D3DXMATRIX mtxR;

	switch (setdir)
	{
	case dir_top://正面から
		vFront = { 0.0f,0.0f,1.0f };
		vRight = { 1.0f,0.0f,0.0f };

		D3DXVec3Normalize(&vFront, &vFront);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVec3Cross(&vUp, &vFront, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		vAt = { 0.0f,0.0f,0.0f };
		vUp = { 0.0f,1.0f,0.0f };
		pos = { 0.0f,0.0f,-10.0f };

		break;
	case dir_under:
		pos = { 0.0f,10.0f,0.0f };

		D3DXMatrixRotationAxis(&mtxR, &vRight, D3DXToRadian(90));
		D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
		D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

		break;
	case dir_right:
		pos = { 10.0f,0.0f,0.0f };

		D3DXMatrixRotationY(&mtxR, D3DXToRadian(90));
		D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
		D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
		D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

		break;
	case dir_left:
		pos = { -10.0f,0.0f,0.0f };

		D3DXMatrixRotationY(&mtxR, D3DXToRadian(-90));
		D3DXVec3TransformNormal(&vFront, &vFront, &mtxR);
		D3DXVec3TransformNormal(&vRight, &vRight, &mtxR);
		D3DXVec3TransformNormal(&vUp, &vUp, &mtxR);

		break;
	default:
		break;
	}
}

void CameraClass::SetPos(vec3f setpos)
{
	if (isFps)
	{
		pos = setpos;
	}
	else
	{
		vec3f temp;

		temp = vFront;
		D3DXVec3Normalize(&temp, &temp);
		pos = setpos - temp * distance;
	}
}

void CameraClass::SetDistance(bool isAdd)
{
	if (isAdd)
	{
		distance += 0.1f;
		if (distance >= FAR_MAX)
		{
			distance = FAR_MAX;
		}
	}
	else
	{
		distance -= 0.1f;
		if (distance <= NEAR_MAX)
		{
			distance = NEAR_MAX;
		}
	}
}

const vec3f & CameraClass::GetFront(void)
{
	return vFront;
}

const vec3f & CameraClass::GetAt(void)
{
	return vAt;
}

const vec3f & CameraClass::GetRight(void)
{
	return vRight;
}

const vec3f & CameraClass::GetPos(void)
{
	return pos;
}

const D3DXMATRIX & CameraClass::GetView(void)
{
	return mtxView;
}

