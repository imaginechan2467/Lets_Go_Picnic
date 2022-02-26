#ifndef KEYLOGGER_H_
#define KEYLOGGER_H_

#include "Config.h"

//有効キー列挙型
typedef enum  KeyloggerKey_tag
{
	KL_W,
	KL_S,
	KL_A,
	KL_D,
	KL_P,
	KL_0,
	KL_E,
	KL_Q,
	KL_R,
	KL_J,
	KL_K,

	KL_RIGHT,
	KL_LEFT,
	KL_UP,
	KL_DOWN,

	KL_ENTER,
	KL_SPACE,

	KL_TAB,
	KL_LEFTCTRL,
	KL_LEFTSHIFT,

	KL_MAX
}KeyloggerKey;

void KeyloggerINIT(void);
void KeyloggerUPDATE(void);
void KeyloggerUNINIT(void);

//キー入力状態の取得	押されてたらtrue
bool KeyloggerPress(KeyloggerKey kl);//押してる瞬間
bool KeyloggerTrigger(KeyloggerKey kl);//押した間
bool KeyloggerRelease(KeyloggerKey kl);//離した瞬間

//WASDで方向を取得
DIR KeyloggerGetDirTrigger(void);
DIR KeyloggerGetDirPress(void);

void Keylogger_RecordStart(int frame_max);
void Keylogger_RecordEnd(void);
void Keylogger_RecordLoad(void);
void Keylogger_RecordPlay(void);
bool Keylogger_IsRecordPlay(void);

DIR KeyloggerGetDirbyTriggerType(bool isTriggerMode);
DIRfor3D KeyloggerGetDirByTriggerTypefor3D(bool isTriggerMode);

#endif