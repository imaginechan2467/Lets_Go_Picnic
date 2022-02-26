#ifndef STAGEMENU_H_
#define STAGEMENU_H_

#include "GameObjectClass.h"
#define STAGEMAX 5


class StageMenuClass
{
public:

	StageMenuClass();
	~StageMenuClass();

	static void Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);
	

private:

	static int stageNameTexture;
	static int bottunFrameTex;
	static int charaTex;
	static int chooseFrameTex;
	static int clearTexture;
	static int coinTexture;
	static int deleteKeyTexture;

	static int stageTexture[STAGEMAX];

	static int cnt;
	static int frame;

	static int coinNum[STAGEMAX];
	static bool isClear[STAGEMAX];

	static GameObjectClass::STAGE chooseStage;
	 
};

#endif