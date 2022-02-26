/*==============================================================================

   [Game.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include <d3dx9.h>
#include "Game.h"
#include "Scene.h"
#include "texture.h"
#include "Facegen.h"
#include "Config.h"
#include "Keylogger.h"
#include "TextGen.h"
#include "Grid.h"
#include "Light.h"
#include "Xmodel.h"
#include "Block.h"
#include "Item.h"
#include "Player.h"
#include "Shade.h"
#include "BillBoard.h"
#include "Effect.h"
#include "Fade.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
GameObjectClass::OBJECTTYPE GameSystem::currentMakerType;
GameSystem::DEBUGMODE GameSystem::currentDebugType;
bool GameSystem::isDebug;
bool GameSystem::isPlayMode;
bool GameSystem::isTriggerMode;
bool GameSystem::isCameraMode;
bool GameSystem::isClear;
bool GameSystem::isGameOver;
GameObjectClass* GameSystem::objforMake[GameObjectClass::objtype_MAX] = {};
CameraClass* GameSystem::camera;
int GameSystem::clearTex;
int GameSystem::gameoverTex;
int GameSystem::cnt;
int GameSystem::frame;
int GameSystem::waitCnt;
int GameSystem::sceneChangeCnt;
int GameSystem::bottunFrameTex;
int GameSystem::currentbottunFrameTex;
int GameSystem::gameoverCharaTextTex;
int GameSystem::manualTex;
bool GameSystem::isDispManual;
GameSystem::GameOverMenu GameSystem::currentGameoverMenu;

static bool g_isFirst;
static PlayerClass* g_player;
static BlockBaseClass* g_pb;
static bool g_isFadeEnd;
static bool g_isDebug;


//*****************************************************************************
// �֐���`
//*****************************************************************************

GameSystem::GameSystem()
{

}

GameSystem::~GameSystem()
{

}

void GameSystem::Init(void)
{
	isDebug = true;
	isPlayMode = true;
	isTriggerMode = true;
	isCameraMode = false;
	isClear = false;
	isGameOver = false;
	g_isFadeEnd = false;
	g_isFirst = true;
	g_isDebug = false;
	isDispManual = false;
	cnt = 0;
	frame = 0;
	currentDebugType = debugmode_pos;
	currentMakerType = GameObjectClass::objtype_block;
	sceneChangeCnt = 0;
	currentGameoverMenu = GameSystem::go_restart;

	clearTex = SetTexture("asset/texture/clear_tex.png", { 1,3 });
	gameoverTex = SetTexture("asset/texture/gameover.png", { 1,3 });
	bottunFrameTex = SetTexture("asset/texture/bottun_frame.png", { 1,3 });
	currentbottunFrameTex = SetTexture("asset/texture/bottun_frame2.png", { 1,3 });
	gameoverCharaTextTex = SetTexture("asset/texture/score_text.png", { 1,3 });
	manualTex = SetTexture("asset/texture/manual.png");
	TextureLoad("�Q�[���N���A");

	GridInit(1.0f, 30, COLOR_GREEN_GRID);
	XModelInit();

	//�I�u�W�F�N�gInit
	{
		BillboardInit();
		BlockClass::Init();
		ItemClass::Init();
		PlayerClass::Init();
		EffectClass::Init();
		CameraClass::Init();
		ShadeInit();
	}

	//���̍쐬
	{
		camera = new CameraClass;
		objforMake[GameObjectClass::objtype_block] = new BlockClass();
		objforMake[GameObjectClass::objtype_item] = new ItemClass();
		g_player = new PlayerClass;
	}

	objforMake[GameObjectClass::objtype_block]->Load(false);
	objforMake[GameObjectClass::objtype_item]->Load(false);

	g_pb = new BlockBaseClass;

	FadeClass::SetFade(fade_in, {255,128,0});
}

void GameSystem::Uninit(void)
{
	GridUninit();
	XModelReleaseAll();

	TextureRelease(&clearTex, 1);
	TextureRelease(&gameoverTex, 1);
	TextureRelease(&bottunFrameTex, 1);
	TextureRelease(&currentbottunFrameTex, 1);
	TextureRelease(&gameoverCharaTextTex, 1);
	TextureRelease(&manualTex, 1);
	clearTex = NULL;
	gameoverTex = NULL;
	bottunFrameTex = NULL;
	currentbottunFrameTex = NULL;
	gameoverCharaTextTex = NULL;
	manualTex = NULL;

	EffectClass::Uninit();
	EffectRandomClass::Uninit();
	ShadeUninit();
	PlayerClass::Uninit();
	BlockClass::Uninit();
	ItemClass::Uninit();
	BillboardUninit();

	delete objforMake[0];
	delete objforMake[1];

	delete g_player;
}

void GameSystem::Update(void)
{
	LightUpdate();

	if (isClear)
	{
		//�N���A��
		if (g_isFirst)
		{
			g_isFirst = g_isFirst = false;
			PlaySound(se_clear);
		}
		sceneChangeCnt++;

		cnt++;
		if (cnt > 5)
		{
			cnt = 0;
			frame++;
		}
		if (frame >= 3)
		{
			frame = 0;
			EffectRandomClass::SetEffectRandomforSecond(eftype_confetti, 5, 60 * 10,
				MakeInt3(128,64,64),
				true, { 64.0f,64.0f }, {0,0,0}, efact_fall, true, true, 10, true);

			EffectRandomClass::SetEffectRandomforSecond(eftype_kirakira, 5, 60 * 10,
				MakeInt3(128, 64, 64),
				true, { 64.0f,64.0f }, {0,0,0}, efact_up, true, true, 10, true);
		}

		//�J������]
		camera->RoleforClear(dir_right);
		camera->SetPos(g_player->GetPos());

		if (FadeClass::GetIsFadeFinish())
		{
			//todo�R�C���擾���A�N���A����ۑ�
			char temp[128] = {};
			sprintf(temp, "asset/data/stage_%d/clear_data.dat", GameObjectClass::GetCurrentStage() + 1);
			FILE* fp = fopen(temp, "wb");

			int tempnum = g_player->GetTakenCoinNum();
			bool tempisclear = true;
			fwrite(&tempnum, sizeof(int), 1, fp);
			fwrite(&tempisclear, sizeof(bool), 1, fp);
			fclose(fp);

			SceneClass::ChangeScene(SceneClass::scene_score);
		}

		if (sceneChangeCnt >= SCENECHANGE_WAIT_CNT)
		{
			if (KeyloggerTrigger(KL_SPACE) || KeyloggerTrigger(KL_ENTER))
			{
				PlaySound(se_pop);

				new EffectClass({ (SCREEN_WIDTH - 512.0f) * 0.5f,
					SCREEN_HEIGHT * 0.3f + (512.0f / 3.0f) ,0.0f }, { 512.0f,512.0f / 3.0f },
					eftype_bigshy, 1, efact_none, 0, COLOR_ORANGE);

				FadeClass::SetFadeFinish();
				FadeClass::SetFade(fade_out, { 255,128,0 });
			}
		}
	}
	else if (isGameOver)
	{
		sceneChangeCnt++;
		if (sceneChangeCnt >= SCENECHANGE_WAIT_CNT)
		{
			if (KeyloggerTrigger(KL_W))
			{
				currentGameoverMenu = (GameSystem::GameOverMenu)(currentGameoverMenu - 1);
				if (currentGameoverMenu < 0)
				{
					currentGameoverMenu = (GameSystem::GameOverMenu)(GameSystem::go_MAX - 1);
				}
				PlaySound(se_switch);
			}
			if (KeyloggerTrigger(KL_S))
			{
				currentGameoverMenu = (GameSystem::GameOverMenu)((currentGameoverMenu + 1) % GameSystem::go_MAX);
				PlaySound(se_switch);
			}
			if (KeyloggerTrigger(KL_SPACE) || KeyloggerTrigger(KL_ENTER))
			{
				PlaySound(se_pop);

				new EffectClass({ (SCREEN_WIDTH - 512.0f) * 0.5f,
					SCREEN_HEIGHT * 0.3f + (512.0f / 3.0f) ,0.0f }, { 512.0f,512.0f / 3.0f },
					eftype_bigshy, 1, efact_none, 0, COLOR_ORANGE);

				if (currentGameoverMenu == GameSystem::go_restart)
				{
					GameSystem::Init();
				}
				else
				{
					SceneClass::ChangeScene(SceneClass::scene_startmenu);
				}
			}
		}

		cnt++;
		if (cnt > 5)
		{
			cnt = 0;
			frame++;
		}
		if (frame >= 3)
		{
			frame = 0;
		}

	}
	else
	{
		if (FadeClass::GetIsFadeFinish())
		{
			FadeClass::SetFadeFinish();
		}

		if (isPlayMode)
		{
			if (!isDispManual)
			{
				g_player->Update();
				camera->Update();//�J��������

				//�J�����Ǐ]
				camera->SetPos(g_player->GetPos());

				//�ړ�or�W�����v
				if (KeyloggerPress(KL_W)) g_player->Move(dir3_forward);
				if (KeyloggerPress(KL_S)) g_player->Move(dir3_back);
				if (KeyloggerPress(KL_D)) g_player->Move(dir3_right);
				if (KeyloggerPress(KL_A)) g_player->Move(dir3_left);
				if (KeyloggerTrigger(KL_SPACE))
				{
					if (PlayerClass::GetIsOnFloor())
					{
						g_player->Move(dir3_top);
					}
					else
					{
						if (ItemClass::GetIsActiveUmbrella())
						{
							ItemClass::SetUmbrella(false);
							g_player->SetGravity(true);
						}
						else
						{
							//�P��W�J
							ItemClass::SetUmbrella(true);
							g_player->SetGravity(false);
							PlaySound(se_umbrella);
						}
					}
				}
				//���e�Z�b�g
				if (KeyloggerTrigger(KL_R))
				{
					PlaySound(se_pop);
					ItemClass::SetBomb(g_player->GetPos());
				}
				if (ItemClass::GetIsActiveUmbrella())
				{
					vec3f temp = g_player->GetPos();
					temp.y += 1.0f;
					ItemClass::UmbrellaSetPos(temp);
				}
				if (KeyloggerPress(KL_J))
				{
					camera->SetDistance(false);
				}
				if (KeyloggerPress(KL_K))
				{
					camera->SetDistance(true);
				}
			}
			//����m�F
			if (KeyloggerTrigger(KL_TAB))
			{
				isDispManual = (isDispManual + 1) % 2;
			}
		}
		//else
		if(g_isDebug)
		{
			//�J�������[�h�؂�ւ�
			if (KeyloggerTrigger(KL_0))
			{
				isCameraMode = (isCameraMode + 1) % 2;
				//objforMake[currentMakerType]->ChangeIsActive();//�J�����g���\���ɂ���B
			}
			//�f�o�b�O���[�h�؂�ւ�
			/*if (KeyloggerTrigger(KL_B))
				isDebug = (isDebug + 1) % 2;*/
			//�J�����ړ����[�h
			if (isCameraMode)
			{
				camera->Update();
			}
			//��Ƒ��샂�[�h
			else
			{
				//CTLR�L�[�����Ȃ���
				if (KeyloggerPress(KL_LEFTCTRL))
				{
					//�J�����g�I�u�W�F�N�g�^�C�v�ύX
					if (KeyloggerTrigger(KL_E))
						currentMakerType = (GameObjectClass::OBJECTTYPE)((currentMakerType + 1) % 2);

					//���̓��[�h�؂�ւ�
					if (KeyloggerTrigger(KL_Q))
						isTriggerMode = (isTriggerMode + 1) % 2;

					//����^�C�v�؂�ւ�
					/*if (KeyloggerTrigger(KL_F))
						currentDebugType = (DEBUGMODE)((currentDebugType + 1) % debugmode_MAX);*/

					//�Z�[�u
					if (KeyloggerTrigger(KL_S))
					{
						BlockClass::SaveAll();
						ItemClass::SaveAll();

						//todo:���₷
						TextGenSetTextforEng("SAVE");
					}
					//�폜
					if (KeyloggerTrigger(KL_P))
					{
						bool isalldelete = false;
						if (MessageBox(NULL, "�f�[�^���폜���܂���?", "�m�F", MB_YESNO) == 6)
						{
							isalldelete = true;
						}

						objforMake[0]->ResetAll(isalldelete);
						objforMake[1]->ResetAll(isalldelete);
						//todo:�v���C���[�̈ʒu��������

					}
					//�X�e�[�W�ړ�
					//if (KeyloggerTrigger(KL_N))
					//{
					//	if (MessageBox(NULL, "�X�e�[�W��ύX����O�ɃZ�[�u���܂����H", "�m�F", MB_YESNO) == 6)
					//	{
					//		BlockClass::SaveAll();
					//		ItemClass::SaveAll();
					//		TextGenSetTextforEng("SAVE");
					//	}

					//	//todo�S�z��폜
					//	objforMake[0]->ResetAll(false);
					//	objforMake[1]->ResetAll(false);

					//	GameObjectClass::SetCurrentStage(GameObjectClass::stage_MAX);

					//	objforMake[GameObjectClass::objtype_block]->Load(false);
					//	objforMake[GameObjectClass::objtype_item]->Load(false);
					//}
				}
				else
				{
					//�ړ�
					if (currentDebugType == debugmode_pos)
						objforMake[currentMakerType]->Move(KeyloggerGetDirByTriggerTypefor3D(isTriggerMode));

					//�T�C�Y�ύX
					else if (currentDebugType == debugmode_size)
						objforMake[currentMakerType]->ChangeSize(KeyloggerGetDirByTriggerTypefor3D(isTriggerMode));

					//�ݒu
					if (KeyloggerTrigger(KL_SPACE))
					{
						objforMake[currentMakerType]->Create(
							objforMake[currentMakerType]->GetPos(),
							objforMake[currentMakerType]->GetSize(),
							true, objforMake[currentMakerType]->GetDetailType(),
							objforMake[currentMakerType]->GetObjType());
					}
					//���̃^�C�v
					/*if (KeyloggerTrigger(KL_F))
					{
						objforMake[currentMakerType]->SetType(-1);
					}*/
					//�폜
					/*if (KeyloggerTrigger(KL_T))
					{
						if (currentMakerType == GameObjectClass::objtype_block)
						{
							for (auto& i : BlockClass::blockList)
							{
								if (Collision::GetBoxOnlyHit(objforMake[currentMakerType]->GetPos(),
									objforMake[currentMakerType]->GetSize(), i->GetPos(),i->GetSize()))
								{
									BlockClass::DeleteByUniqueNumber(i->GetUniqueNum());
								}
							}
						}
					}*/
				}
			}
			//�v���C���[�h�؂�ւ�
			if (KeyloggerTrigger(KL_ENTER))
			{
				isPlayMode = (isPlayMode + 1) % 2;
				g_player->ResetStatus();

				if (isPlayMode)
				{
					objforMake[currentMakerType]->ChangeIsActive();//�J�����g���\��
				}
				else
				{
					objforMake[currentMakerType]->ChangeIsActive();//�\��
				}
			}
		}
	}

	objforMake[0]->UpdateAll();
	objforMake[1]->UpdateAll();

	EffectClass::UpdateAll();
	EffectRandomClass::UpdateAll();

	BillBoardSetViewMatrix(camera->GetView());
}

void GameSystem::Draw(void)
{
	camera->SetViewMatrix();

	if (g_isDebug)
	{
		GridDraw();
	}

	objforMake[0]->DrawAll();
	objforMake[1]->DrawAll();

	EffectClass::DrawAll();

	g_player->Draw();

	//�N���A�ƃQ�[���I�[�o�[�\��
	const vec2f size = {512.0f * 0.8f,256.0f * 0.8f };
	if (isClear)
	{
		FaceGenforTex({ CENTERPOS(size).x,32.0f }, size, true, clearTex, frame);

		if (sceneChangeCnt >= SCENECHANGE_WAIT_CNT)
		{
			//�{�^���t���[���\��
			FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,
					SCREEN_HEIGHT * 0.3f + (512.0f / 3.0f) },
				{ 512.0f,512.0f / 3.0f }, true, bottunFrameTex, frame);

			//�����\��
			FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,
					SCREEN_HEIGHT * 0.3f + (512.0f / 3.0f) }, 
				{ 512.0f,512.0f / 3.0f }, true, gameoverCharaTextTex, 0);
		}
	}
	else if(isGameOver)
	{
		//�w�i
		int tempalpah = sceneChangeCnt >= 255 ? 255 : sceneChangeCnt;
		FaceGenSimple({ 0,0 }, SCREENSIZE, D3DCOLOR_RGBA(32, 0, 32, tempalpah));

		FaceGenforTex({ CENTERPOS(size).x,32.0f }, size, true, gameoverTex, frame);

		if (sceneChangeCnt >= SCENECHANGE_WAIT_CNT)
		{
			//�{�^���t���[���\��
			for (int i = 0; i < 2; i++)
			{
				//�{�^��
				FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,SCREEN_HEIGHT * 0.2f + (512.0f / 3) * (i + 1) },
					{ 512.0f,512.0f / 3.0f }, true, bottunFrameTex, frame);
			}

			//�J�����g�t���[��
			FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,SCREEN_HEIGHT * 0.2f + (512.0f / 3) * (currentGameoverMenu + 1) },
				{ 512.0f,512.0f / 3.0f }, true, currentbottunFrameTex, frame, COLOR_ORANGE);

			//�����\��
			FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,SCREEN_HEIGHT * 0.2f + (512.0f / 3) },//���v���C
				{ 512.0f,512.0f / 3.0f }, true, gameoverCharaTextTex, 1);

			FaceGenforTex({ (SCREEN_WIDTH - 512.0f) * 0.5f,SCREEN_HEIGHT * 0.2f + (512.0f / 3) * 2},//�I���
				{ 512.0f,512.0f / 3.0f }, true, gameoverCharaTextTex, 2);
		}
	}

	//�f�o�b�O�\��
	if(g_isDebug)
	{
		//camera->Draw();
		Int2 setpos = {};
		char temp[96] = {};

		//�J�����g�|�W�V����
		sprintf(temp, "%3.f %3.f %3.f", objforMake[currentMakerType]->GetPos().x, 
			objforMake[currentMakerType]->GetPos().y, objforMake[currentMakerType]->GetPos().z);
		TextGenforEng({ 0.0f,0.0f }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�J�����g���[�J�[�I�u�W�F�N�g
		const char* debugtext_maker[] =
		{
			{"block"},{"item"},
		};
		sprintf(temp, "%s", debugtext_maker[currentMakerType]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�J�����g���[�h
		static const char* debugtext_currentMode[] = 
		{
			{"POS"},{"SIZE"},{"ROLE"},
		};
		sprintf(temp, "OPMODE %s",debugtext_currentMode[currentDebugType]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		const static char* debugtext_bool[] =
		{
			{"FALSE"},{"TRUE"},
		};
		//�J�������[�h���ǂ���
		sprintf(temp, "ISCAMERA %s", debugtext_bool[isCameraMode]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		const static char* debugtext_triggermode[] = 
		{
			{"TRIGGER"},{"PRESS"},
		};
		//���샂�[�h
		sprintf(temp, "TRIGGERMODE %s", debugtext_triggermode[isTriggerMode]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�v���C���[pos
		sprintf(temp, "PLAYER %3.f %3.f %3.f", g_player->GetPos().x, g_player->GetPos().y, g_player->GetPos().z);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;	
		//�v���C���[�����ɂ��邩
		sprintf(temp, "ISONFLOOR %s", debugtext_bool[PlayerClass::GetIsOnFloor()]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//���[�J�[�̃J�����g�^�C�v
		sprintf(temp, "TYPE %d", objforMake[currentMakerType]->GetDetailType());
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�N���A
		sprintf(temp, "ISCLEAR %s", debugtext_bool[isClear]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�Q�[���I�[�o�[
		sprintf(temp, "ISGAMEOVER %s", debugtext_bool[isGameOver]);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;
		//�X�e�[�W��
		sprintf(temp, "STAGE %d", GameObjectClass::GetCurrentStage() + 1);
		TextGenforEng({ 0.0f,32.0f * setpos.y }, TEXTSIZE_NORMAL, temp);
		setpos.y++;

		objforMake[currentMakerType];
	}

	if (isDispManual)
	{
		FaceGenSimple({0.0f,0.0f},SCREENSIZE,D3DCOLOR_RGBA(0,0,0,128));

		FaceGen({0.0f,0.0f},SCREENSIZE,true, manualTex);
	}
}

void GameSystem::SetIsClear(bool set)
{
	isClear = set;
}

void GameSystem::SetIsGameOver(bool set)
{
	isGameOver = set;
}

bool GameSystem::GetIsDebug(void)
{
	return isDebug;
}

bool GameSystem::GetIsPlayMode(void)
{
	return isPlayMode;
}

bool GameSystem::GetisTriggerMode(void)
{
	return isTriggerMode;
}

bool GameSystem::GetIsCameraMode(void)
{
	return isCameraMode;
}

GameObjectClass::OBJECTTYPE GameSystem::GetCurrentMakerType(void)
{
	return currentMakerType;
}

bool GameSystem::GetIsClear(void)
{
	return isClear;
}

bool GameSystem::GetIsGameOver(void)
{
	return isGameOver;
}
