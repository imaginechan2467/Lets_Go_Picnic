#ifndef EFFECT_H_
#define EFFECT_H_

#include "Config.h"
#include <vector>

#define EFFECT_DEFAULT_SIZE (vec2f(64.0f,64.0f))
#define EFFECT_DEFAULT_COUNT (5)
#define EFFECT_GRAVITY ( 0.0005f)

enum EffectType
{
	eftype_fire,
	eftype_moya,
	eftype_fireworks,
	eftype_confetti,
	eftype_kirakira,
	eftype_shy,
	eftype_bigshy,

	eftype_MAX,
};

enum EffectActionType
{
	efact_none,
	efact_fall,
	efact_up,
	efact_jump,

	efact_MAX,
};

class EffectClass
{
public:

	static std::vector<EffectClass*>effectList;

	//コンストデスト
	EffectClass(vec3f setpos, vec2f setsize, EffectType settype,
		int setRepeatNum = 1, EffectActionType setaction = efact_none,
		float startgravity = 0.0f, D3DCOLOR setcolor = COLOR_NORMAL);
	~EffectClass();

	//基本関数
	static void Init(void);
	static void Uninit(void);
	void Draw(void);
	void Update(void);
	static void DrawAll(void);
	static void UpdateAll(void);


private:

	EffectType effectType;
	vec3f pos;
	vec2f size;
	bool isActive;
	int cnt;
	int frame;
	D3DCOLOR color;
	int repeatNum;
	int repeatMax;
	int uniqueNum;
	float fallSpeed;
	EffectActionType effectAction;
	float value;

	static int effectCnt;
	static int effectTexture[eftype_MAX];

};

class EffectRandomClass
{
public:

	//コンストデスト
	EffectRandomClass();
	~EffectRandomClass();

	//基本関数
	void Update(void);
	static void UpdateAll(void);
	static void Uninit(void);

	//セット系
	static void SetEffectRandomforSecond(EffectType settype,
		int setcreateCnt = 5, int setcreateSecondMax = 60 * 5,
		Int3 setCreateSpace = { 128,128,128 }, bool setisUseRandomSize = false,
		vec2f setsize = EFFECT_DEFAULT_SIZE, vec3f centerpos = {0.0f,0.0f,0.0f},
		EffectActionType setAction = efact_none, bool isUseRandomColor = false,
		bool isUseRandomRepeat = true, int setrepeatNum = 3,bool setIsUseRandomGraviry = true);

private:

	static std::vector<EffectRandomClass*>effectRandomList;

	int createCntMax;//作るまでのカウント
	int createSecondMax;//作る時間
	int cntforCreate;//ただのカウント
	int craeteTimeCnt;
	Int3 createSpace;
	EffectType createType;
	vec2f createSize;
	vec3f centerpos;
	bool isUseRandomSize;
	EffectActionType craeteActionType;
	bool isUserandomColor;
	int uniqueNum;
	int repeatNum;
	bool isUseRandomRepeat;
	bool isUseRandomGravity;

	static int effectRandomCnt;
};


#endif