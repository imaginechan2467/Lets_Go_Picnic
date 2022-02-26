//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	bgm1,
	bgm2,
	bgm3,

	se_pop,
	se_burn,
	se_clear,

	se_gameover,
	se_coin,

	se_jump,
	se_bomb,
	se_fall,
	se_umbrella,

	se_swap1,
	se_swap2,
	se_swap3,
	se_switch,

	sound_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
