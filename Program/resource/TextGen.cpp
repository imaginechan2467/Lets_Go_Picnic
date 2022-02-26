/*==============================================================================

   [TextGen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:����̕������͂ŉ��s����悤�ɂ��ꂩ�炷��B
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
// �}�N��
//*****************************************************************************
#define TEXT_WIDTH (32)
#define TEXT_HEIGHT (64)
#define TEXTMAX (256)

//*****************************************************************************
// �\���̐錾
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

}TEXTINDEX;//todo:����g���āA�o�^����Ă���v�Z���Ȃ��悤�ɂ����炢�������B

//*****************************************************************************
// �O���[�o���ϐ��錾
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
	"�����������������������������������������������������������������������ÂĂłƂǂȂɂʂ˂̂͂΂ςЂт҂ӂԂՂւׂ؂قڂۂ܂݂ނ߂��������������������"
};
static char g_kana[] =
{
	"�@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�^�_�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~�E����������������������������������������������"
};
static char g_kigou[] =
{
	"�@�A�B�C�D�E�F�G�H�I�J�K�L�M�N�O�P�Q�R�S�T�U�V�W�X�Y�Z�[�\�]�^�_�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|�}�~��������������"
};
static char g_eng[] =
{
	"�O�P�Q�R�S�T�U�V�W�X�E�E�E�E�E�E�E�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y�E�E�E�E�E�E����������������������������������������������������"
};

//*****************************************************************************
// �֐�����
//*****************************************************************************
void TextGenInit(void)
{
	g_textTex = SetTexture("asset/texture/text_2.png", { 16,6 });
	g_hiraTex = SetTexture("asset/texture/font_hira.png", { 16,6 });
	g_kanaTex = SetTexture("asset/texture/font_kana.png", { 16,6 });
	g_kigouTex = SetTexture("asset/texture/text_kigou.png", { 16,5 });
	g_engTex = SetTexture("asset/texture/font_eng.png", { 16,5 });
	TextureLoad("�e�L�X�g");

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

		g_textIndex[i].alphaNum -= g_textIndex[i].disappearSpeed;//�A���t�@�l���}�C�i�X
		if (g_textIndex[i].alphaNum <= 0)
		{
			g_textIndex[i].alphaNum = 255;
			memset(&g_textIndex[i].text, 0, sizeof(g_textIndex[i].text));
			g_textIndex[i].isUse = false;
			g_UsingNum--;
		}
	}
}

//todo:�����悭���ā`(ָ�Á`)
//todo:�A���t�@�x�b�g���������Ƃ��O��B
void TextGenforJp(vec2f pos, vec2f size, const char* text, D3DCOLOR color)
{
	//�S�p��2�o�C�g�B
	for (unsigned int i = 0; i < strlen(text); i += 2)
	{
		int texture = 0;
		bool isHira = false;
		bool isKigou = false;
		bool isEng = false;

		//�Ђ炪�ȁA�J�^�J�i�A�L���𔻒�
		if (text[i] == (char)0x82) isHira = true;//�Ђ炪��

		else if (text[i] == (char)0x83) isHira = false;//�J�^�J�i

		else if (text[i] == (char)0x81) isKigou = true;//�L��

		else continue;

		//offset(�������ꂽ�e�N�X�`���̉��Ԗڂ����v�Z)
		int offset = 0;
		if (isKigou)//�L��
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

					if (text[i + 1] == g_hira[k + 1])//�Ђ炪��
					{
						offset = m;
						break;
					}
					else if (text[i + 1] == g_eng[k + 1])//�p��
					{
						offset = m - 1;
						isEng = true;
						break;
					}
				}
				else//�J�^�J�i
				{
					if (text[i + 1] == g_kana[k + 1])
					{
						offset = m;
						break;
					}
				}
			}
		}

		//�e�N�X�`���I��
		texture = isHira ? g_hiraTex : g_kanaTex;

		texture = isKigou ? g_kigouTex : texture;

		texture = isEng ? g_engTex : texture;

		if (isKigou || isEng)
		{
			//�`��
			FaceGenforTex(vec2f(pos.x + (size.x / 2) * i, pos.y), size, true,
				texture, offset);
		}
		else//�L����������
		{
			//�`��
			FaceGenforTex(vec2f(pos.x + (size.x / 2) * i, pos.y), size, true,
				texture, offset);
		}
	}
}

//�S�p���������͂���Ȃ����Ƃ��O��
void TextGenforEng(vec2f pos, vec2f size, const char * text, D3DCOLOR color)
{
	for (unsigned int i = 0; i < strlen(text); i++)
	{
		int offset = text[i] - ' ';

		FaceGenforTex(vec2f(pos.x + size.x * i, pos.y), vec2f(size.x, size.y), true,
			g_textTex, offset);
	}
}

//�S�p���������͂���Ȃ����Ƃ��O��@
//todo:TextGenDraw����1�ɂ܂Ƃ߂���悤�ȃR�[�h�v����������
void TextGenDrawforSecond(void)
{
	for (unsigned int k = 0; k < TEXTMAX; k++)
	{
		//�z����g���ĂȂ����A�S�p��������continue�B
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

//�S�p���������͂���Ȃ����Ƃ��O��
void TextGenSetTextforEng(const char* text, int subAlphaSpeed,
	vec2f size, bool isUseAlpha,
	D3DCOLOR color)
{
	//�������o�^����Ă���o�^����߂�B
	//for (int i = 0; i < TEXTMAX; i++)
	//{
	//	if (!g_textIndex[i].isUse)continue;

	//	if (strcmp(g_textIndex[i].text, text) == 0)return;
	//}

	//�o�^
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