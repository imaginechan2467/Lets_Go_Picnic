//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
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
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
