/*==============================================================================

   [Keylogger.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Keylogger.h"
#include "keyboard.h"

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
typedef unsigned int ULKeyBufSize;
static ULKeyBufSize g_CurrentKeyState = 0;//そのフレームでのキーの状態保持用
static ULKeyBufSize g_PrevKeyState = 0;//前フレームでのキー状態保持用
static ULKeyBufSize g_TryggerKeyState = 0;
static ULKeyBufSize g_ReleaceKeyState = 0;

static ULKeyBufSize g_KeyloggerMode = 0;
static ULKeyBufSize* g_pRecordCurrentData = NULL;
static ULKeyBufSize g_RecordFrame = 0;
static ULKeyBufSize g_RecordPlayFrame = 0;
static ULKeyBufSize g_RecordFrameMax = 0;

//*****************************************************************************
// プロトタイプ関数宣言
//*****************************************************************************
DIRfor3D KeyloggerGetDirTrigger3D(void);
DIRfor3D KeyloggerGetDirPress3D(void);

//*****************************************************************************
// ENUM
//*****************************************************************************
static const Keyboard_Keys g_KKs[KL_MAX] = 
{
	KK_W,
	KK_S,
	KK_A,
	KK_D,
	KK_P,
	KK_D0,
	KK_E,
	KK_Q,
	KK_R,
	KK_J,
	KK_K,

	KK_RIGHT,
	KK_LEFT,
	KK_UP,
	KK_DOWN,

	KK_ENTER,
	KK_SPACE,

	KK_TAB,
	KK_LEFTCONTROL,
	KK_LEFTSHIFT,
};

void KeyloggerINIT(void)
{
	g_CurrentKeyState = 0;
	g_PrevKeyState = 0;
	g_TryggerKeyState = 0;
	g_ReleaceKeyState = 0;

	g_KeyloggerMode = 0;
	g_pRecordCurrentData = NULL;
	g_RecordFrame = 0;
	g_RecordPlayFrame = 0;
	g_RecordFrameMax = 0;
}

void KeyloggerUPDATE(void)
{
	g_PrevKeyState = g_CurrentKeyState;
	g_CurrentKeyState = 0;

	if (g_KeyloggerMode <= 1)
	{
		//KeyloggerKeyの状態取得
		for (int i = 0; i < KL_MAX; i++)
		{
			if (Keyboard_IsKeyDown(g_KKs[i]))
			{
				g_CurrentKeyState |= 1u << i;//unsignec charだから8個までKeyを記録できる
			}
		}

		//キー記録モード
		if (g_KeyloggerMode == 1)
		{
			if (g_RecordFrame < g_RecordFrameMax)
			{
				g_pRecordCurrentData[g_RecordFrame] = g_CurrentKeyState;
				g_RecordFrame++;
			}
		}
	}
	else
	{
		//記録フレームより多く再生しようとした時の安全処理
		if (g_RecordPlayFrame < g_RecordFrame)
		{
			g_CurrentKeyState = g_pRecordCurrentData[g_RecordPlayFrame];
		}
		else
		{
			g_CurrentKeyState = 0;
		}
	}
	//押した瞬間と話した瞬間のキーを取得
	g_TryggerKeyState = (g_PrevKeyState ^ g_CurrentKeyState) & g_CurrentKeyState;
	g_ReleaceKeyState = (g_PrevKeyState ^ g_CurrentKeyState) & g_PrevKeyState;
}

void KeyloggerUNINIT(void)
{
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
		g_pRecordCurrentData = NULL;
	}
}

bool KeyloggerPress(KeyloggerKey kl)
{
	return g_CurrentKeyState & (1u << kl);
}

bool KeyloggerTrigger(KeyloggerKey kl)
{
	return g_TryggerKeyState & (1u << kl);
}

bool KeyloggerRelease(KeyloggerKey kl)
{
	return g_ReleaceKeyState & (1u << kl);
}

DIR KeyloggerGetDirTrigger(void)
{
	if (KeyloggerTrigger(KL_W)) return dir_top;
	if (KeyloggerTrigger(KL_S)) return dir_under;
	if (KeyloggerTrigger(KL_D)) return dir_right;
	if (KeyloggerTrigger(KL_A)) return dir_left;
	return dir_MAX;
}

DIR KeyloggerGetDirPress(void)
{
	if (KeyloggerPress(KL_W)) return dir_top;
	if (KeyloggerPress(KL_S)) return dir_under;
	if (KeyloggerPress(KL_D)) return dir_right;
	if (KeyloggerPress(KL_A)) return dir_left;
	return dir_MAX;
}

void Keylogger_RecordStart(int frame_max)
{
	if (g_pRecordCurrentData)
	{
		free(g_pRecordCurrentData);
	}
	g_pRecordCurrentData = (ULKeyBufSize*)malloc(sizeof(ULKeyBufSize) * frame_max);
	g_KeyloggerMode = 1;
	g_RecordFrame = 0;
}

void Keylogger_RecordEnd(void)
{

}

void Keylogger_RecordLoad(void)
{
	FILE* fp = fopen("keylogger.dat", "wb");
	fwrite(&g_RecordFrame, sizeof(ULKeyBufSize), 1, fp);
	fwrite(g_pRecordCurrentData, sizeof(ULKeyBufSize), g_RecordFrame, fp);
	fclose(fp);

	g_KeyloggerMode = 0;
}

void Keylogger_RecordPlay(void)
{
	g_RecordPlayFrame = 0;
	g_KeyloggerMode = 2;
}

bool Keylogger_IsRecordPlay(void)
{
	return g_KeyloggerMode == 2 ? true : false;
}

DIR KeyloggerGetDirbyTriggerType(bool isTriggerMode)
{
	if (isTriggerMode)
	{
		return KeyloggerGetDirTrigger();
	}
	else
	{
		return KeyloggerGetDirPress();
	}
}


DIRfor3D KeyloggerGetDirTrigger3D(void)
{
	if (KeyloggerTrigger(KL_W)) return dir3_forward;
	if (KeyloggerTrigger(KL_S)) return dir3_back;
	if (KeyloggerTrigger(KL_D)) return dir3_right;
	if (KeyloggerTrigger(KL_A)) return dir3_left;
	if (KeyloggerTrigger(KL_E)) return dir3_top;
	if (KeyloggerTrigger(KL_Q)) return dir3_under;
	return dir3_MAX;
}

DIRfor3D KeyloggerGetDirPress3D(void)
{
	if (KeyloggerPress(KL_W)) return dir3_forward;
	if (KeyloggerPress(KL_S)) return dir3_back;
	if (KeyloggerPress(KL_D)) return dir3_right;
	if (KeyloggerPress(KL_A)) return dir3_left;
	if (KeyloggerPress(KL_E)) return dir3_top;
	if (KeyloggerPress(KL_Q)) return dir3_under;
	return dir3_MAX;
}

DIRfor3D KeyloggerGetDirByTriggerTypefor3D(bool isTriggerMode)
{
	if (isTriggerMode)
	{
		return KeyloggerGetDirTrigger3D();
	}
	else
	{
		return KeyloggerGetDirPress3D();
	}
}
