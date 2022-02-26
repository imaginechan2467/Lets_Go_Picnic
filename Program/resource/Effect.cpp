/*==============================================================================

   [Effect.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Effect.h"
#include "texture.h"
#include "Facegen.h"
#include "BillBoard.h"
#include "mydirect3D.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
std::vector<EffectClass*> EffectClass::effectList;
int EffectClass::effectTexture[eftype_MAX] = {};
int EffectClass::effectCnt;

int EffectRandomClass::effectRandomCnt;
std::vector<EffectRandomClass*> EffectRandomClass::effectRandomList;

//*****************************************************************************
// 関数定義
//*****************************************************************************
EffectClass::EffectClass(vec3f setpos, vec2f setsize,
	EffectType settype, int setRepeatNum,EffectActionType setaction,float startgravity,D3DCOLOR setcolor)
{
	pos = setpos;
	effectType = settype;
	size = setsize;
	color = setcolor;
	isActive = true;
	cnt = 0;
	frame = 0;
	repeatNum = 0;
	repeatMax = setRepeatNum;
	fallSpeed = startgravity;
	effectAction = setaction;
	value = 0.0f;

	uniqueNum = effectCnt;
	effectCnt++;

	effectList.push_back(this);
}

EffectClass::~EffectClass()
{

}

void EffectClass::Init(void)
{
	effectTexture[0] = SetTexture("asset/texture/effect_fire.png",{3, 2});
	effectTexture[1] = SetTexture("asset/texture/effect_moyamoya.png",{3, 3});
	effectTexture[2] = SetTexture("asset/texture/effect_exp.png",{3, 2});
	effectTexture[3] = SetTexture("asset/texture/effect_confetti.png",{3, 2});
	effectTexture[4] = SetTexture("asset/texture/item_hilight.png",{3, 2});
	effectTexture[5] = SetTexture("asset/texture/effect_shy.png",{3, 1});
	effectTexture[6] = SetTexture("asset/texture/effect_shybig.png",{1, 3});
	TextureLoad("エフェクト");

	effectCnt = 0;
}

void EffectClass::Uninit(void)
{
	TextureRelease(effectTexture, eftype_MAX);
	memset(effectTexture, NULL, sizeof(effectTexture));

	effectList.erase(effectList.begin(), effectList.end());
}

void EffectClass::Update(void)
{
	cnt++;
	if (cnt >= EFFECT_DEFAULT_COUNT)
	{
		cnt = 0;
		frame++;
	}

	switch (effectAction)
	{
	case efact_none:
		break;
	case efact_fall:

		fallSpeed += EFFECT_GRAVITY;
		pos.y -= fallSpeed;

		break;
	case efact_up:

		fallSpeed += EFFECT_GRAVITY;
		pos.y += fallSpeed;

		break;
	case efact_jump:

		value += 0.1f;
		pos.y -= sinf(value) * 0.1f;

		break;
	default:
		break;
	}

	if (frame >= (Texture_GetFrameMax(effectTexture[effectType]).x * Texture_GetFrameMax(effectTexture[effectType]).y))
	{
		repeatNum++;
		if (repeatNum >= repeatMax)
		{
			isActive = false;
		}
		else
		{
			frame = 0;
		}
	}
}

void EffectClass::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (effectType == eftype_bigshy || 
		effectType == eftype_shy)
	{
		FaceGenforTex(vec3_to_vec2(pos),size,true, effectTexture[effectType],frame,color);
	}
	else
	{
		//ビルボード
		BillboardDrawforTex(pos, vec2_to_vec3(size), effectTexture[effectType], frame, color);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void EffectClass::DrawAll(void)
{
	for (auto& i : effectList)
	{
		if (!i->isActive)continue;

		i->Draw();
	}
}

void EffectClass::UpdateAll(void)
{
	int cnt = 0;
	for (auto& i : effectList)
	{
		if (!i->isActive)
		{
			effectList.erase(effectList.begin() + cnt);//削除
			continue;
		}
		cnt++;

		i->Update();
	}

	
}

//*****************************************************************************
//エフェクトランダムクラス定義
//*****************************************************************************

EffectRandomClass::EffectRandomClass()
{
	createCntMax = 0;
	createSecondMax = 0;
	cntforCreate = 0;
	craeteTimeCnt = 0;
	createSpace = {};
	craeteActionType = efact_none;
	uniqueNum = effectRandomCnt;
	effectRandomCnt++;
	createSize = vec2f_NULL;
	isUserandomColor = false;
	isUseRandomSize = false;
	repeatNum = 1;
	isUseRandomRepeat = false;
	isUseRandomGravity = false;

	effectRandomList.push_back(this);
}

EffectRandomClass::~EffectRandomClass()
{
}

void EffectRandomClass::Update(void)
{
	//ランダム作成

	craeteTimeCnt++;
	if (craeteTimeCnt >= createSecondMax)
	{
		//作成モード終了

		int cnt = 0;
		for (auto& i : effectRandomList)
		{
			if (i->uniqueNum == this->uniqueNum)
			{
				effectRandomList.erase(effectRandomList.begin() + cnt);
				break;
			}
			cnt++;
		}
	}
	else
	{
		cntforCreate++;
		if (cntforCreate >= createCntMax)
		{
			//作成
			cntforCreate = 0;

			vec3f temppos = vec3f_NULL;

			temppos.x = (float)(rand() % (createSpace.x / 2) + 1);
			temppos.y = (float)(rand() % (createSpace.y / 2) + 1);
			temppos.z = (float)(rand() % (createSpace.z / 2) + 1);

			if (rand() % 2 == 0)
			{
				//位置を左右逆転
				int temprandom = rand() % 3;
				if (temprandom == 0)
				{
					temppos.x *= -1;
				}
				else if (temprandom == 1)
				{
					temppos.y *= -1;
				}
				else
				{
					temppos.z *= -1;
				}
			}

			if (rand() % 2)
			{
				if (rand() % 2)
				{
					temppos -= {(float)((rand() % 10) * 0.1f), (float)((rand() % 10) * 0.1f), (float)((rand() % 10) * 0.1f)};
				}
				else
				{
					temppos += {(float)((rand() % 10) * 0.1f), (float)((rand() % 10) * 0.1f), (float)((rand() % 10) * 0.1f)};
				}
			}

			temppos += centerpos;

			vec2f tempsize = createSize;
			if (isUseRandomSize)
			{
				tempsize.x = (float)(rand() % (int)createSize.x + 1);
				tempsize.y = (float)(rand() % (int)createSize.y + 1);
			}

			D3DCOLOR tempcolor = COLOR_NORMAL;
			if (isUserandomColor)
			{
				tempcolor = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			}

			int temprepeatNum = repeatNum;
			if (isUseRandomRepeat)
			{
				temprepeatNum = rand() % (repeatNum + 1) + 1;
			}

			float tempGravity = 0.0f;
			if (isUseRandomGravity)
			{
				tempGravity = (float)(rand() % 2);
				tempGravity *= 0.1f;
				float tempunder = (float)(rand() % 10);
				tempGravity += tempunder * 0.01f;
			}

			new EffectClass(temppos, tempsize, createType,
				temprepeatNum, craeteActionType, tempGravity, tempcolor);
		}
	}
}

void EffectRandomClass::UpdateAll(void)
{
	for (auto& i : effectRandomList)
	{
		i->Update();
	}
}

void EffectRandomClass::Uninit(void)
{
	effectRandomList.erase(effectRandomList.begin(), effectRandomList.end());
}

void EffectRandomClass::SetEffectRandomforSecond(EffectType settype, 
	int setcreateCnt, int setcreateSecondMax, Int3 setCreateSpace,
	bool setisUseRandomSize, vec2f setsize,vec3f setcenterpos, EffectActionType setAction, bool setisUseRandomColor,
	bool setisUseRandomRepeat, int setrepeatNum, bool setIsUseRandomGraviry)
{

	EffectRandomClass* temp = new EffectRandomClass();

	temp->createType = settype;
	temp->createCntMax = setcreateCnt;
	temp->createSecondMax = setcreateSecondMax;
	temp->craeteTimeCnt = 0;
	temp->cntforCreate = 0;
	temp->createSpace = setCreateSpace;
	temp->isUseRandomSize = setisUseRandomSize;
	temp->createSize = setsize;
	temp->centerpos = setcenterpos;
	temp->craeteActionType = setAction;
	temp->isUserandomColor = setisUseRandomColor;
	temp->isUseRandomRepeat = setisUseRandomRepeat;
	temp->repeatNum = setrepeatNum;
	temp->isUseRandomGravity = setIsUseRandomGraviry;

}
