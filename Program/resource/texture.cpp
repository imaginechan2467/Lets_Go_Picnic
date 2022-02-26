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
// �}�N��
//*****************************************************************************
#define TEXTURE_MAX (64)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct TEXTURE_tag//�e�N�X�`���Ǘ��e�[�u���\����
{
	LPDIRECT3DTEXTURE9 pTexture;//NULL�Ȃ�o�^����ĂȂ�
	float width;
	float height;
	Int2 cut;
	char filename[TEXTURE_FILENAME_MAX];//0�Ȃ�o�^����ĂȂ�
}Texture;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
static Texture g_Texture[TEXTURE_MAX];//�e�N�X�`���e�[�u���Ǘ���


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
	for (int i = 0; i < TEXTURE_MAX; i++)//�t�@�C��������	�����e�N�X�`�������łɓǂ܂�ĂȂ����`�F�b�N
	{
		if (g_Texture[i].filename[0] == 0)//�e�[�u�����󂢂Ă���X�L�b�v
		{
			continue;
		}

		//���Ɏw��̃t�@�C�����\�񂳂�Ă邩�m�F����
		if (strcmp(pFileName, g_Texture[i].filename) == 0)//���O����Ȃ��ăn�b�V���̂ق������������������i�n�b�V���̃��\�[�X�t�@�C���Ƃ�����Ƃ��āj
		{
			return i;//�\�񂳂�Ă�
		}
	}

	for (int i = 0; i < TEXTURE_MAX; i++)//�V�K�\��
	{
		if (g_Texture[i].filename[0] != 0)//�e�[�u�����g�p����������X�L�b�v
		{
			continue;
		}

		strcpy(g_Texture[i].filename, pFileName);//�t�@�C�������e�[�u���ɃR�s�[
		g_Texture[i].cut = Cut;
		//g_Texture[i].cut = Cut;
		return i;
	}

	return TEXTURE_INVALID_ID;//1025�l����!!(MAX + 1)
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
			continue;//��̃e�[�u���Ȃ̂ŃX�L�b�v
		}

		if (g_Texture[i].pTexture != NULL)//���ɓǂݍ��܂�Ă�
		{
			continue;
		}

		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].filename, &g_Texture[i].pTexture);
		if (FAILED(hr))
		{
			char tempbuf[48] = { "�e�N�X�`����ǂݍ��߂܂���ł���" };
			char tempbuf2[128] = {};
			strcpy(tempbuf2, text);
			strcat(tempbuf2, tempbuf);

			MessageBox(NULL, tempbuf2, "�G���[", MB_OK);
			err_cnt++;
			return false;
		}
		else
		{
			//�e�N�X�`���̉𑜓x�i���E�����j�̒���
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Texture[i].filename, &info);
			g_Texture[i].width = (float)info.Width;
			g_Texture[i].height = (float)info.Height;
		}
	}
	return err_cnt;//�ǂݍ��߂Ȃ�������
}

void TextureRelease(int textureIds[], int count)//�ʂ�cpp�Ƃ��œ����e�N�X�`�����g���Ă��肷�邩��A�\�񂷂邽�тɗ\��J�E���g��++���āA�J������count--����0�Ȃ�A�����[�h����Ȃǂ���Ƃ����B
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