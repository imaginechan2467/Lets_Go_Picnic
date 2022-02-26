#ifndef GAME_H_
#define GAME_H_

#include "GameObjectClass.h"
#include "Camera.h"

#define SCENECHANGE_WAIT_CNT (60 * 3)

class GameSystem
{
public:

	enum DEBUGMODE
	{
		debugmode_pos,
		debugmode_size,
		debugmode_MAX,
	};

	enum GameOverMenu
	{
		go_restart,
		go_end,
		go_MAX,
	};

	GameSystem();
	~GameSystem();

	static void Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	//ÉZÉbÉgån
	static void SetIsClear(bool set);
	static void SetIsGameOver(bool set);

	//ìÆçÏån

	//éÊìæån
	static bool GetIsDebug(void);
	static bool GetIsPlayMode(void);
	static bool GetisTriggerMode(void);
	static bool GetIsCameraMode(void);
	static GameObjectClass::OBJECTTYPE GetCurrentMakerType(void);
	static bool GetIsClear(void);
	static bool GetIsGameOver(void);

private:

	static GameObjectClass::OBJECTTYPE currentMakerType;
	static bool isDebug;
	static bool isPlayMode;
	static bool isTriggerMode;
	static GameObjectClass* objforMake[GameObjectClass::objtype_MAX];
	static DEBUGMODE currentDebugType;
	static bool isCameraMode;
	static CameraClass* camera;

	static bool isClear;
	static bool isGameOver;

	static int clearTex;
	static int gameoverTex;
	static int cnt;
	static int frame;
	static int sceneChangeCnt;
	static int waitCnt;
	static GameOverMenu currentGameoverMenu;
	static int bottunFrameTex;
	static int currentbottunFrameTex;
	static int gameoverCharaTextTex;
	static int manualTex;
	static bool isDispManual;
};

#endif