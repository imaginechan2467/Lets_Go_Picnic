#ifndef GAMESPEED_H_
#define GAMESPEED_H_

class GameSpeed
{
public:

	enum GAMESPEED
	{
		gamespeed_stop,
		gamespeed_normal,
		gamespeed_slow,
		gamespeed_MAX,
	};

	GameSpeed();
	~GameSpeed();

	//��{�֐�
	static void Init(void);
	static void Uninit(void);

	//�Z�b�g�n�֐�
	void SetGameSpeed(GAMESPEED setspeed);

	//�擾�n�֐�
	GAMESPEED GetGameSpeedforEnum(void);
	float GetGameSpeedforFloat(void);

private:

	static GAMESPEED currentGameSpeed;

};

#endif