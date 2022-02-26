#include "StageMenu.h"
#include "texture.h"
#include "Facegen.h"
#include "Keylogger.h"
#include "Scene.h"
#include "sound.h"


int StageMenuClass::stageNameTexture;
int StageMenuClass::bottunFrameTex;
int StageMenuClass::charaTex;
int StageMenuClass::chooseFrameTex;
int StageMenuClass::clearTexture;
int StageMenuClass::coinTexture;
int StageMenuClass::deleteKeyTexture;
int StageMenuClass::cnt;
int StageMenuClass::frame;
int StageMenuClass::stageTexture[STAGEMAX];
int StageMenuClass::coinNum[STAGEMAX];
bool StageMenuClass::isClear[STAGEMAX];
GameObjectClass::STAGE StageMenuClass::chooseStage;

StageMenuClass::StageMenuClass()
{
}

StageMenuClass::~StageMenuClass()
{
}

void StageMenuClass::Init(void)
{
	bottunFrameTex = SetTexture("asset/texture/bottun_frame.png", { 1,3 });
	charaTex = SetTexture("asset/texture/bottun_frame.png");
	chooseFrameTex = SetTexture("asset/texture/Frame.png", {1,3});
	stageNameTexture = SetTexture("asset/texture/satge_name.png", {1,5});
	stageTexture[0] = SetTexture("asset/texture/stage_1.png");
	stageTexture[1] = SetTexture("asset/texture/stage_2.png");
	stageTexture[2] = SetTexture("asset/texture/stage_3.png");
	stageTexture[3] = SetTexture("asset/texture/stage_4.png");
	stageTexture[4] = SetTexture("asset/texture/stage_5.png");
	clearTexture = SetTexture("asset/texture/clear_tex.png", {1,3});
	coinTexture = SetTexture("asset/texture/ui_coin.png",{2,1});
	deleteKeyTexture = SetTexture("asset/texture/deleteKey.png");
	TextureLoad("ステージメニュー");
	chooseStage = GameObjectClass::stage_1;
	cnt = 0;
	frame = 0;
	memset(coinNum, 0, sizeof(coinNum));
	memset(isClear, false, sizeof(isClear));

	for (int i = 0; i < STAGEMAX; i++)
	{
		char temp[128] = {};
		sprintf(temp, "asset/data/stage_%d/clear_data.dat",i + 1);
		FILE* fp = fopen(temp, "rb");
		if (fp == NULL)
		{
			coinNum[i] = 0;
			isClear[i] = false;
			continue;
		}
		fread(&coinNum[i], sizeof(int), 1, fp);
		fread(&isClear[i], sizeof(bool), 1, fp);

		fclose(fp);
	}
}

void StageMenuClass::Uninit(void)
{
	TextureRelease(&bottunFrameTex, 1);
	TextureRelease(&charaTex, 1);
	TextureRelease(&chooseFrameTex, 1);
	TextureRelease(&stageNameTexture, 1);
	TextureRelease(&clearTexture, 1);
	TextureRelease(&coinTexture, 1);
	TextureRelease(&deleteKeyTexture, 1);
	TextureRelease(stageTexture, STAGEMAX);
	bottunFrameTex = NULL;
	charaTex = NULL;
	chooseFrameTex = NULL;
	stageNameTexture = NULL;
	clearTexture = NULL;
	coinTexture = NULL;
	deleteKeyTexture = NULL;
	memset(stageTexture, 0, sizeof(stageTexture));
}

void StageMenuClass::Update(void)
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

	if (KeyloggerTrigger(KL_D))
	{
		chooseStage = (GameObjectClass::STAGE)((chooseStage + 1));
		if (chooseStage >= GameObjectClass::stage_MAX)
		{
			chooseStage = (GameObjectClass::STAGE)(GameObjectClass::stage_MAX - 1);
		}
		rand() % 2 ? PlaySound(se_swap2) : PlaySound(se_swap3);
	}
	if (KeyloggerTrigger(KL_A))
	{
		chooseStage = (GameObjectClass::STAGE)((chooseStage - 1));
		if (chooseStage < 0)
		{
			chooseStage = (GameObjectClass::STAGE)0;
		}
		rand() % 2 ? PlaySound(se_swap2) : PlaySound(se_swap3);
	}
	if (KeyloggerTrigger(KL_SPACE) || KeyloggerTrigger(KL_ENTER))
	{
		PlaySound(se_pop);

		GameObjectClass::SetCurrentStage(chooseStage);

		SceneClass::ChangeScene(SceneClass::scene_game);
	}

	if (KeyloggerPress(KL_LEFTCTRL))
	{
		if (KeyloggerTrigger(KL_P))
		{
			for (int i = 0; i < STAGEMAX; i++)
			{
				char temp[128] = {};
				sprintf(temp, "asset/data/stage_%d/clear_data.dat", i + 1);
				FILE* fp = fopen(temp, "wb");

				int tempnum = 0;
				bool tempis = false;
				fwrite(&tempnum, sizeof(int), 1, fp);
				fwrite(&tempis, sizeof(bool), 1, fp);
				fclose(fp);

				coinNum[i] = 0;
				isClear[i] = false;
			}
		}
	}
}

void StageMenuClass::Draw(void)
{
	const vec2f tempsize = { 512.0f, 512.0f / 3.0f };

	//ボタンカレント
	FaceGenforTex(CENTERPOS(tempsize), tempsize, true, bottunFrameTex, frame);

	//ステージ表示
	for (int i = 0; i < GameObjectClass::stage_MAX; i++)
	{
		FaceGen({ (SCREEN_WIDTH - tempsize.x) * 0.5f - (tempsize.x + 64.0f) * chooseStage + (tempsize.x + 64.0f) * i,
			(SCREEN_HEIGHT - SCREENSIZE.y * 0.5f) * 0.5f }, SCREENSIZE * 0.5f,true,stageTexture[i]);

		//ステージ名表示
		FaceGenforTex({ (SCREEN_WIDTH - tempsize.x) * 0.5f - (tempsize.x + 64.0f) * chooseStage + (tempsize.x + 64.0f) * i,
			(SCREEN_HEIGHT - tempsize.y) * 0.5f + tempsize.y + 64.0f },tempsize,true, stageNameTexture,i,COLOR_BLACK);
	}

	//カレントフレーム
	FaceGenforTex({(SCREEN_WIDTH - tempsize.x) * 0.5f,160.0f},
		SCREENSIZE * 0.5f, true, chooseFrameTex, frame);

	//取ったコイン
	for (int i = 0; i < 3; i++)
	{
		int temp = (coinNum[chooseStage] - i) > 0 ? 1 : 0;
		FaceGenforTex({ SCREEN_WIDTH - 64.0f * (3 - i),0.0f }, { 64.0f,64.0f }, true, coinTexture, temp);
	}

	if (isClear[chooseStage])
	{
		//クリアしたかどうか
		FaceGenforTex({ (SCREEN_WIDTH - 256.0f) * 0.5f,0.0f }, { 256.0f,256.0f / 3 }, true, clearTexture, frame);
	}

	FaceGen({0.0f,SCREEN_HEIGHT - 256.0f / 3}, {256.0f,256.0f / 3},true, deleteKeyTexture);
}
