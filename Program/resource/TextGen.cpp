/*==============================================================================

   [TextGen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:特定の文字入力で改行するようにこれからする。
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<d3dx9.h>
#include"texture.h"
#include"Facegen.h"
#include"Config.h"
#include<stdio.h>
#include "TextGen.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define TEXT_WIDTH (32)
#define TEXT_HEIGHT (64)
#define TEXTMAX (256)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	vec2f pos;
	vec2f size;
	D3DCOLOR color;
	char text[256];
	bool isUse;
	int alphaNum;
	bool isUseAlpha;
	int disappearSpeed;
	int disappearCntIndex;

}TEXTINDEX;//todo:これ使って、登録されてたら計算しないようにしたらいいかも。

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static int g_textTex;
static int g_hiraTex;
static int g_kanaTex;
static int g_kigouTex;
static int g_engTex;
static TEXTINDEX g_textIndex[TEXTMAX] = {};
static int g_UsingNum;
static char g_hira[] =
{
	"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん"
};
static char g_kana[] =
{
	"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミ・ムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ"
};
static char g_kigou[] =
{
	"　、。，．・：；？！゛゜´｀¨＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／＼～∥｜…‥‘’“”（）〔〕［］｛｝〈〉《》「」『』【】＋－±×÷＝≠＜＞≦≧"
};
static char g_eng[] =
{
	"０１２３４５６７８９・・・・・・・ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ・・・・・・ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"
};

//*****************************************************************************
// 関数実体
//*****************************************************************************
void TextGenInit(void)
{
	g_textTex = SetTexture("asset/texture/text_2.png", { 16,6 });
	g_hiraTex = SetTexture("asset/texture/font_hira.png", { 16,6 });
	g_kanaTex = SetTexture("asset/texture/font_kana.png", { 16,6 });
	g_kigouTex = SetTexture("asset/texture/text_kigou.png", { 16,5 });
	g_engTex = SetTexture("asset/texture/font_eng.png", { 16,5 });
	TextureLoad("テキスト");

	memset(g_textIndex, 0, sizeof(g_textIndex));
	for (int i = 0; i < TEXTMAX; i++)
	{
		g_textIndex[i].alphaNum = 255;
	}
	g_UsingNum = 0;
}

void TextGenUninit(void)
{
	TextureRelease(&g_textTex, 1);
	TextureRelease(&g_hiraTex, 1);
	TextureRelease(&g_kanaTex, 1);

	g_textTex = NULL;
	g_hiraTex = NULL;
	g_kanaTex = NULL;
}

void TextGenUpdate(void)
{
	for (int i = 0; i < TEXTMAX; i++)
	{
		if (!g_textIndex[i].isUse)continue;

		g_textIndex[i].alphaNum -= g_textIndex[i].disappearSpeed;//アルファ値をマイナス
		if (g_textIndex[i].alphaNum <= 0)
		{
			g_textIndex[i].alphaNum = 255;
			memset(&g_textIndex[i].text, 0, sizeof(g_textIndex[i].text));
			g_textIndex[i].isUse = false;
			g_UsingNum--;
		}
	}
}

//todo:効率よくして～(ﾖｸｼﾃ～)
//todo:アルファベットが無いことが前提。
void TextGenforJp(vec2f pos, vec2f size, const char* text, D3DCOLOR color)
{
	//全角は2バイト。
	for (unsigned int i = 0; i < strlen(text); i += 2)
	{
		int texture = 0;
		bool isHira = false;
		bool isKigou = false;
		bool isEng = false;

		//ひらがな、カタカナ、記号を判定
		if (text[i] == (char)0x82) isHira = true;//ひらがな

		else if (text[i] == (char)0x83) isHira = false;//カタカナ

		else if (text[i] == (char)0x81) isKigou = true;//記号

		else continue;

		//offset(分割されたテクスチャの何番目かを計算)
		int offset = 0;
		if (isKigou)//記号
		{
			for (int k = 0, m = 1; m < 73; k += 2, m++)
			{
				if (text[i + 1] == g_kigou[k + 1])
				{
					offset = m;
				}
			}
		}
		else
		{
			for (int k = 0, m = 1; m < 85; k += 2, m++)
			{
				if (isHira)
				{

					if (text[i + 1] == g_hira[k + 1])//ひらがな
					{
						offset = m;
						break;
					}
					else if (text[i + 1] == g_eng[k + 1])//英語
					{
						offset = m - 1;
						isEng = true;
						break;
					}
				}
				else//カタカナ
				{
					if (text[i + 1] == g_kana[k + 1])
					{
						offset = m;
						break;
					}
				}
			}
		}

		//テクスチャ選択
		texture = isHira ? g_hiraTex : g_kanaTex;

		texture = isKigou ? g_kigouTex : texture;

		texture = isEng ? g_engTex : texture;

		if (isKigou || isEng)
		{
			//描画
			FaceGenforTex(vec2f(pos.x + (size.x / 2) * i, pos.y), size, true,
				texture, offset);
		}
		else//記号だったら
		{
			//描画
			FaceGenforTex(vec2f(pos.x + (size.x / 2) * i, pos.y), size, true,
				texture, offset);
		}
	}
}

//全角文字が入力されないことが前提
void TextGenforEng(vec2f pos, vec2f size, const char * text, D3DCOLOR color)
{
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		int offset = text[i] - ' ';

		FaceGenforTex(vec2f(pos.x + size.x * i, pos.y), vec2f(size.x, size.y), true,
			g_textTex, offset);
	}
}

//全角文字が入力されないことが前提　
//todo:TextGenDrawこれ1個にまとめられるようなコード思いついたら作る
void TextGenDrawforSecond(void)
{
	for (unsigned int k = 0; k < TEXTMAX; k++)
	{
		//配列を使ってないか、全角だったらcontinue。
		if (!g_textIndex[k].isUse)continue;

		for (unsigned int i = 0; i < strlen(g_textIndex[k].text); i++)
		{
			int offset = g_textIndex[k].text[i] - ' ';

			//FaceGenforTex(vec2f(g_textIndex[k].pos.x + g_textIndex[k].size.x * i, g_textIndex[k].pos.y),
			//	g_textIndex[k].size, true, g_textTex, offset,
			//	D3DCOLOR_RGBA(255, 255, 255, g_textIndex[k].alphaNum));

			FaceGenforTex({ g_textIndex[k].pos.x + g_textIndex[k].size.x * i, g_textIndex[k].pos.y },
				g_textIndex[k].size, true, g_textTex, offset,
				D3DCOLOR_RGBA(255, 255, 255, g_textIndex[k].alphaNum));
		}
	}
}

//全角文字が入力されないことが前提
void TextGenSetTextforEng(const char* text, int subAlphaSpeed,
	vec2f size, bool isUseAlpha,
	D3DCOLOR color)
{
	//文字が登録されてたら登録をやめる。
	//for (int i = 0; i < TEXTMAX; i++)
	//{
	//	if (!g_textIndex[i].isUse)continue;

	//	if (strcmp(g_textIndex[i].text, text) == 0)return;
	//}

	//登録
	for (int i = 0; i < TEXTMAX; i++)
	{
		if (g_textIndex[i].isUse)continue;

		g_textIndex[i].isUse = true;
		g_textIndex[i].isUseAlpha = isUseAlpha;
		g_textIndex[i].disappearSpeed = subAlphaSpeed;
		g_textIndex[i].alphaNum = 255;
		g_textIndex[i].size = size;
		strcpy(g_textIndex[i].text, text);
		//g_textIndex[i].pos = { pos.x,pos.y + g_textIndex[i].size.y * g_UsingNum };
		g_textIndex[i].pos = { SCREEN_WIDTH - strlen(text) * TEXTSIZE_NORMAL.x,g_textIndex[i].size.y * g_UsingNum };
		g_textIndex[i].color = color;
		g_UsingNum++;

		break;
	}
}