
#include "Score.h"
#include "texture.h"
#include "Facegen.h"
#include "Scene.h"
#include "Keylogger.h"
#include "GameObjectClass.h"
#include "sound.h"


ScoreMenu ScoreClass::currentMenu;
int ScoreClass::BottunTexture;
int ScoreClass::CurrentBottunFrameTexture;
int ScoreClass::CoinTexture;
int ScoreClass::CharaTexture;
int ScoreClass::cnt;
int ScoreClass::frame;
int ScoreClass::BottunCharaTexture;

ScoreClass::ScoreClass()
{
}

ScoreClass::~ScoreClass()
{
}

void ScoreClass::Init(void)
{
	currentMenu = scoremenu_next;

	BottunTexture = SetTexture("asset/texture/bottun_frame.png", { 1,3 });
	CurrentBottunFrameTexture = SetTexture("asset/texture/bottun_frame.png", { 1,3 });
	CoinTexture = SetTexture("asset/texture/ui_coin.png", { 2,1 });
	CharaTexture = SetTexture("asset/texture/text_score.png", { 1,3 });
	BottunCharaTexture = SetTexture("asset/texture/score_text.png", { 1,3 });
	TextureLoad("スコア");

	cnt = 0;
	frame = 0;
}

void ScoreClass::Uninit(void)
{
	TextureRelease(&BottunTexture, 1);
	TextureRelease(&CurrentBottunFrameTexture, 1);
	TextureRelease(&CoinTexture, 1);
	TextureRelease(&CharaTexture, 1);
	TextureRelease(&BottunCharaTexture, 1);

	BottunTexture = NULL;
	CurrentBottunFrameTexture = NULL;
	CoinTexture = NULL;
	CharaTexture = NULL;
	BottunCharaTexture = NULL;
}

void ScoreClass::Update(void)
{

	cnt ++;
	if (cnt > 5)
	{
		cnt = 0;
		frame++;
	}
	if (frame > 3)
	{
		frame = 0;
	}

	if (KeyloggerTrigger(KL_W))
	{
		currentMenu = (ScoreMenu)(currentMenu - 1);
		if (currentMenu < 0)
		{
			currentMenu = (ScoreMenu)(scoremenu_MAX - 1);
		}
		PlaySound(se_switch);
	}
	if (KeyloggerTrigger(KL_S))
	{
		currentMenu = (ScoreMenu)((currentMenu + 1) % scoremenu_MAX);
		PlaySound(se_switch);
	}
	if (KeyloggerTrigger(KL_SPACE) || KeyloggerTrigger(KL_ENTER))
	{
		PlaySound(se_pop);
		if (currentMenu == scoremenu_next)
		{
			GameObjectClass::SetCurrentStage(GameObjectClass::stage_MAX);
			SceneClass::ChangeScene(SceneClass::scene_game);
		}
		else if (currentMenu == scoremenu_replay)
		{
			SceneClass::ChangeScene(SceneClass::scene_game);
		}
		else//スタート画面にもどる
		{
			SceneClass::ChangeScene(SceneClass::scene_startmenu);
		}
	}
}

void ScoreClass::Draw(void)
{

	FaceGenSimple({ 0,0 }, SCREENSIZE, D3DCOLOR_RGBA(255, 230, 128, 255));

	for(int i = 0;i < 3;i++)
	{
		//ボタン
		FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,(512.0f / 3) * (i + 1) },
			{ 512.0f,512.0f / 3.0f }, true, BottunTexture, frame);
	}

	//カレントフレーム
	FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,(512.0f / 3) * (currentMenu + 1) },
		{ 512.0f,512.0f / 3.0f }, true, CurrentBottunFrameTexture, frame, COLOR_ORANGE);

	for (int i = 0; i < 3; i++)
	{
		FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,(512.0f / 3) * (i + 1) },
			{ 512.0f,512.0f / 3.0f }, true, BottunCharaTexture, i);
	}

	//スコア文字
	FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,0.0f },
		{ 512.0f,512.0f / 3.0f }, true, CharaTexture, frame);

}



