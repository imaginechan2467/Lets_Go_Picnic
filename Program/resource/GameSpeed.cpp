#include "GameSpeed.h"

GameSpeed::GAMESPEED GameSpeed::currentGameSpeed;

GameSpeed::GameSpeed()
{
}

GameSpeed::~GameSpeed()
{
}

void GameSpeed::Init(void)
{
	currentGameSpeed = gamespeed_normal;
}

void GameSpeed::Uninit(void)
{
}

void GameSpeed::SetGameSpeed(GAMESPEED setspeed)
{
	if (setspeed == gamespeed_MAX)
	{
		currentGameSpeed = (GAMESPEED)((currentGameSpeed + 1) % gamespeed_MAX);
	}
	currentGameSpeed = setspeed;
}

GameSpeed::GAMESPEED GameSpeed::GetGameSpeedforEnum(void)
{
	return currentGameSpeed;
}

float GameSpeed::GetGameSpeedforFloat(void)
{
	float ret = currentGameSpeed == gamespeed_normal ? 1.0f :
		currentGameSpeed == gamespeed_slow ? 0.5f : 0.0f;

	return ret;
}
