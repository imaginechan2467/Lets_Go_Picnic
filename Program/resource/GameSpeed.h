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

	//基本関数
	static void Init(void);
	static void Uninit(void);

	//セット系関数
	void SetGameSpeed(GAMESPEED setspeed);

	//取得系関数
	GAMESPEED GetGameSpeedforEnum(void);
	float GetGameSpeedforFloat(void);

private:

	static GAMESPEED currentGameSpeed;

};

#endif