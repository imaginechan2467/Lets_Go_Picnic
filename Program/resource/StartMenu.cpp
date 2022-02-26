/*==============================================================================

   [Facegen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "StartMenu.h"
#include "texture.h"
#include "Facegen.h"
#include "Keylogger.h"
#include "Scene.h"
#include "main.h"
#include "TextGen.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int StartMenuClass::backgroundTexture;
int StartMenuClass::bottunNormalBackgroundTexture;
int StartMenuClass::bottunChooseBackgroundTexture;
int StartMenuClass::titleBackgroundTexture;
int StartMenuClass::bottunNormalTextTexture;
int StartMenuClass::titleCharaTexture;
int StartMenuClass::cnt;
int StartMenuClass::frame;
StartMenuClass::STARTMENUBOTTUN StartMenuClass::currentBottun;

//*****************************************************************************
// 関数定義
//*****************************************************************************
void StartMenuClass::Init(void)
{
	currentBottun = startmenu_play;

	titleCharaTexture = SetTexture("asset/texture/title_text.png", {1,3});
	bottunNormalBackgroundTexture = SetTexture("asset/texture/bottun_frame.png", {1,3});
	bottunChooseBackgroundTexture = SetTexture("asset/texture/bottun_frame2.png", {1,3});
	bottunNormalTextTexture = SetTexture("asset/texture/startmenu_bottun_chara.png", {1,2});
	titleBackgroundTexture = SetTexture("asset/texture/start_background.png");
	TextureLoad("タイトル");

	cnt = 0;
	frame = 0;
}

void StartMenuClass::Update(void)
{
	cnt++;
	if (cnt >= 5)
	{
		cnt = 0;
		frame++;
	}
	if (frame >= 3)
	{
		frame = 0;
	}

	if (KeyloggerTrigger(KL_W))
	{
		currentBottun = (STARTMENUBOTTUN)(currentBottun - 1);
		if (currentBottun < 0)currentBottun = startmenu_end;
		PlaySound(se_switch);
	}
	if (KeyloggerTrigger(KL_S))
	{
		currentBottun = (STARTMENUBOTTUN)((currentBottun + 1) % startmenu_MAX);
		PlaySound(se_switch);
	}
	if (KeyloggerTrigger(KL_ENTER) || KeyloggerTrigger(KL_SPACE))
	{
		if (currentBottun == startmenu_play)
		{
			PlaySound(se_pop);
			SceneClass::ChangeScene(SceneClass::scene_stagemenu);
		}
		else
		{
			PlaySound(se_pop);
			ProgramFinish();
		}
	}
}

void StartMenuClass::Draw(void)
{
	//背景
	/*FaceGenSimple({0,0}, SCREENSIZE, D3DCOLOR_RGBA(0, 200, 200, 255));*/

	FaceGen({0,0},SCREENSIZE,true,titleBackgroundTexture);

	//ボタン
	const vec2f tempsize = { 512.0f,512.0f / 3.0f };
	float titleX = (SCREEN_WIDTH - tempsize.x) * 0.5f;
	float tempX = 32.0f;

	for (int i = 0; i < 2; i++)
	{
		FaceGenforTex({ tempX,tempsize.y * (i + 1) + 128.0f},
			tempsize, true, bottunNormalBackgroundTexture, frame);
	}

	//選択ボタン
	FaceGenforTex({ tempX,tempsize.y * (currentBottun + 1) + 128.0f },
		tempsize, true, bottunNormalBackgroundTexture, frame,COLOR_ORANGE);

	//文字
	FaceGenforTex({ tempX,tempsize.y + 128.0f },
		tempsize, true, bottunNormalTextTexture,0);//play
	FaceGenforTex({ tempX,tempsize.y * 2 + 128.0f },
		tempsize, true, bottunNormalTextTexture,1);//end
	
	//タイトル
	FaceGenforTex({ titleX, 32.0f},
		tempsize, true, titleCharaTexture, frame);
}

void StartMenuClass::Uninit(void)
{
	TextureRelease(&backgroundTexture, 1);
	TextureRelease(&bottunNormalBackgroundTexture, 1);
	TextureRelease(&bottunChooseBackgroundTexture, 1);
	TextureRelease(&bottunNormalTextTexture, 1);
	TextureRelease(&titleCharaTexture, 1);

	backgroundTexture = NULL;
	bottunNormalBackgroundTexture = NULL;
	bottunChooseBackgroundTexture = NULL;
	bottunNormalTextTexture = NULL;
	titleCharaTexture = NULL;
}
