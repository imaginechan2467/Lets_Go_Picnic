/*==============================================================================

   [texture.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<d3dx9.h>
#include<string>
#include"texture.h"
#include"mydirect3D.h"
#include<Windows.h>


//*****************************************************************************
// マクロ
//*****************************************************************************
#define TEXTURE_MAX (64)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct TEXTURE_tag//テクスチャ管理テーブル構造体
{
	LPDIRECT3DTEXTURE9 pTexture;//NULLなら登録されてない
	float width;
	float height;
	Int2 cut;
	char filename[TEXTURE_FILENAME_MAX];//0なら登録されてない
}Texture;

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static Texture g_Texture[TEXTURE_MAX];//テクスチャテーブル管理数


void TextureINIT(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i].pTexture = NULL;
		g_Texture[i].width = 0.0;
		g_Texture[i].height = 0.0;
		g_Texture[i].filename[0] = 0;
	}
}

void TextureUNINIT(void)
{
	TextureAllRelease();
}

int SetTexture(const char* pFileName, Int2 Cut)
{
	for (int i = 0; i < TEXTURE_MAX; i++)//ファイル名検索	同じテクスチャがすでに読まれてないかチェック
	{
		if (g_Texture[i].filename[0] == 0)//テーブルが空いてたらスキップ
		{
			continue;
		}

		//既に指定のファイルが予約されてるか確認する
		if (strcmp(pFileName, g_Texture[i].filename) == 0)//名前じゃなくてハッシュのほうが効率がいいかも（ハッシュのリソースファイルとか作っといて）
		{
			return i;//予約されてる
		}
	}

	for (int i = 0; i < TEXTURE_MAX; i++)//新規予約
	{
		if (g_Texture[i].filename[0] != 0)//テーブルが使用中だったらスキップ
		{
			continue;
		}

		strcpy(g_Texture[i].filename, pFileName);//ファイル名をテーブルにコピー
		g_Texture[i].cut = Cut;
		//g_Texture[i].cut = Cut;
		return i;
	}

	return TEXTURE_INVALID_ID;//1025人いる!!(MAX + 1)
}

int TextureLoad(const char* text)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return -1;
	}

	int err_cnt = 0;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i].filename[0] == 0)
		{
			continue;//空のテーブルなのでスキップ
		}

		if (g_Texture[i].pTexture != NULL)//既に読み込まれてる
		{
			continue;
		}

		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].filename, &g_Texture[i].pTexture);
		if (FAILED(hr))
		{
			char tempbuf[48] = { "テクスチャを読み込めませんでした" };
			char tempbuf2[128] = {};
			strcpy(tempbuf2, text);
			strcat(tempbuf2, tempbuf);

			MessageBox(NULL, tempbuf2, "エラー", MB_OK);
			err_cnt++;
			return false;
		}
		else
		{
			//テクスチャの解像度（幅・高さ）の調査
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Texture[i].filename, &info);
			g_Texture[i].width = (float)info.Width;
			g_Texture[i].height = (float)info.Height;
		}
	}
	return err_cnt;//読み込めなかった数
}

void TextureRelease(int textureIds[], int count)//別のcppとかで同じテクスチャを使ってたりするから、予約するたびに予約カウントを++して、開放時にcount--して0ならアンロードするなどするといい。
{
	for (int i = 0; i < count; i++)
	{
		if (g_Texture[textureIds[i]].pTexture)
		{
			g_Texture[textureIds[i]].pTexture->Release();
			g_Texture[textureIds[i]].pTexture = NULL;

			g_Texture[textureIds[i]].filename[0] = 0;
			g_Texture[textureIds[i]].width = 0.0;
			g_Texture[textureIds[i]].height = 0.0;
		}
	}
}

void TextureAllRelease(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i].pTexture)
		{
			g_Texture[i].pTexture->Release();
			g_Texture[i].pTexture = NULL;

			g_Texture[i].filename[0] = 0;
			g_Texture[i].width = 0;
			g_Texture[i].height = 0;
		}
	}
}

float TextureGetTextureSize(int textureId, char WorH)
{
	if (WorH == 'W')
	{
		return g_Texture[textureId].width;
	}
	else
	{
		return g_Texture[textureId].height;
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureId)
{
	return g_Texture[textureId].pTexture;
}

Int2 Texture_GetFrameMax(int textureId)
{
	return g_Texture[textureId].cut;
}