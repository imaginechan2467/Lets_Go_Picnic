#ifndef FADE_H_
#define FADE_H_

#include "Config.h"

enum FadeType
{
	fade_none,
	fade_in,
	fade_out,
	fade_inout,
	fade_MAX,
};

class FadeClass
{
public:

	//コンストデスト
	FadeClass();
	~FadeClass();

	//基本関数
	static void Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);


	//セット系関数
	static void SetFade(FadeType settype, Int3 setcolor = {255,255,255});
	static void SetFadeFinish(void);
	static void SetFadeEndFalse(void);

	//取得系関数
	static bool GetIsFadeFinish(void);


private:

	static FadeType fadeType;
	static int alpha;
	static Int3 color;
	static bool isActive;
	static bool isBack;
	static bool isEnd;

};

#endif