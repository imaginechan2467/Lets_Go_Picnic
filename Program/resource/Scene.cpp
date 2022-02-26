/*==============================================================================

   [Scene.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/


//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Scene.h"
#include "Game.h"
#include "StartMenu.h"
#include "Score.h"
#include "StageMenu.h"
#include "Fade.h"


//*****************************************************************************
// �v���g�^�C�v�֐��錾
//*****************************************************************************

typedef void(*SceneFunction)(void);//�֐��|�C���^

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
SceneClass::SCENE SceneClass::currentScene = scene_startmenu;
SceneClass::SCENE SceneClass::nextScene = currentScene;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
static const SceneFunction g_pSceneInit[] =
{
	StartMenuClass::Init,
	StageMenuClass::Init,
	GameSystem::Init,
	ScoreClass::Init,
};

static const SceneFunction g_pSceneUpdate[] =
{
	StartMenuClass::Update,
	StageMenuClass::Update,
	GameSystem::Update,
	ScoreClass::Update,
};

static const SceneFunction g_pSceneDraw[] =
{
	StartMenuClass::Draw,
	StageMenuClass::Draw,
	GameSystem::Draw,
	ScoreClass::Draw,
};

static const SceneFunction g_pSceneUninit[] =
{
	StartMenuClass::Uninit,
	StageMenuClass::Uninit,
	GameSystem::Uninit,
	ScoreClass::Uninit,
};

//*****************************************************************************
// �֐���`
//*****************************************************************************
void SceneClass::Init(void)
{
	g_pSceneInit[currentScene]();
}

void SceneClass::Update(void)
{
	g_pSceneUpdate[currentScene]();
}

void SceneClass::Draw(void)
{
	g_pSceneDraw[currentScene]();
}

void SceneClass::Uninit(void)
{
	g_pSceneUninit[currentScene]();
}

void SceneClass::ExecuteChangeScene(void)
{
	if (currentScene == nextScene)return;

	Uninit();

	currentScene = nextScene;

	Init();

	FadeClass::SetFadeFinish();
}

SceneClass::SCENE SceneClass::GetCurrentScene(void)
{
	return currentScene;
}

void SceneClass::ChangeScene(SCENE next)
{
	nextScene = next;
}
