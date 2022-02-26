#include "Fade.h"
#include "texture.h"
#include "Facegen.h"


FadeType FadeClass::fadeType;
int FadeClass::alpha;
Int3 FadeClass::color;
bool FadeClass::isActive;
bool FadeClass::isBack;
bool FadeClass::isEnd;

FadeClass::FadeClass()
{
}

FadeClass::~FadeClass()
{
}

void FadeClass::Init(void)
{
	fadeType = fade_none;
	color = {255,255,255};
	alpha = 0;
	isActive = false;
	isBack = false;
	isEnd = false;
}

void FadeClass::Uninit(void)
{

}

void FadeClass::Update(void)
{
	if (isActive)
	{
		if (fadeType == fade_in)
		{
			alpha--;
			if (alpha < 0)
			{
				alpha = 0;
				isEnd = true;
			}
		}
		else if (fadeType == fade_out)
		{
			alpha++;
			if (alpha > 255)
			{
				alpha = 255;
				isEnd = true;
			}
		}
		else
		{
			if (!isBack)
			{
				alpha++;
				if (alpha > 255)
				{
					alpha = 255;
					isBack = true;
				}
			}
			else
			{
				alpha--;
				if (alpha < 0)
				{
					alpha = 0;
					isBack = false;
					isEnd = true;
				}
			}
		}
	}
}

void FadeClass::Draw(void)
{
	if (isActive)
	{
		FaceGenSimple({0.0f,0.0f}, SCREENSIZE, D3DCOLOR_RGBA(color.x,color.y,color.z,alpha));
	}
}

void FadeClass::SetFade(FadeType settype, Int3 setcolor)
{
	if (!isActive)
	{
		isActive = true;
		isEnd = false;
		color = setcolor;	
		fadeType = settype;
		alpha = fadeType == fade_in ? 255 : 0;
	}
}

void FadeClass::SetFadeFinish(void)
{
	if (isActive || isEnd)
	{
		isActive = false;
		isEnd = false;
		isBack = false;
		color = {0,0,0};
		alpha = 0;
		fadeType = fade_none;
	}
}

void FadeClass::SetFadeEndFalse(void)
{
	isEnd = false;
}



bool FadeClass::GetIsFadeFinish(void)
{
	return isEnd;
}
