#ifndef KEYLOGGER_H_
#define KEYLOGGER_H_

#include "Config.h"

//�L���L�[�񋓌^
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

//�L�[���͏�Ԃ̎擾	������Ă���true
bool KeyloggerPress(KeyloggerKey kl);//�����Ă�u��
bool KeyloggerTrigger(KeyloggerKey kl);//��������
bool KeyloggerRelease(KeyloggerKey kl);//�������u��

//WASD�ŕ������擾
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