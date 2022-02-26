/*==============================================================================

   [FaceGen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------

	memo:

==============================================================================*/

#ifndef FACEGEN_H_
#define FACEGEN_H_

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Config.h"
#pragma warning(disable:26812)
//*****************************************************************************
// �񋓌^�錾
//*****************************************************************************
enum GAGEMODE
{
	gage_add,
	gage_sub,
};

//*****************************************************************************
// �v���g�^�C�v�֐��錾
//*****************************************************************************

enum FACEGENTYPE
{
	facegentype_center,
	facegentype_leftup,

	facegentype_MAX,
};

void FaceGenInit();
void FaceGenUninit();

//�m�[�}��
void FaceGen(vec2f pos, vec2f size, bool IsUseTex,
	int TexId, D3DCOLOR Color = COLOR_NORMAL);

//�V���v���|���S���\��
void FaceGenSimple(vec2f pos, vec2f size,
	D3DCOLOR Color = COLOR_NORMAL);

void FaceGen3D(vec3f pos, vec2f scale, FACEGENTYPE typebool,
	bool IsUseTex, int TexId, float radian = 0.0f, D3DCOLOR Color = COLOR_NORMAL);

void FaceGenforTex3D(vec3f pos, vec2f scale, FACEGENTYPE type, bool IsUseTex,
	int TexId, int CurrentFrame, float radian = 0.0f, D3DCOLOR Color = COLOR_NORMAL);

//�e�L�X�`���؂���
void FaceGenforTex(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, D3DCOLOR Color = COLOR_NORMAL);

//�e�N�X�`���؂��� && �������]���[�h�t���B
//��7������dir_left����ꂽ�ꍇ�A�e�N�X�`����X�𔽓]����B
void FaceGenforTexInvert(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

//��]
void FaceGenRotate(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, float angle, D3DCOLOR Color = COLOR_NORMAL);

//�Q�[�W
void FaceGenforGage(vec2f pos, vec2f size, float GageNum, DIR dir,
	GAGEMODE mode, bool isUI, D3DCOLOR Color = COLOR_NORMAL);

//�~
//�ǉ��F�n�C�N�I���e�B���[�h�AHQ�̏ꍇR�ɍ��킹�č����xvertex�̍��o��(���\�����̂Ŏg��Ȃ���������)
void CircleGen(vec2f pos, float R, int vertexNum, D3DCOLOR color);

//�~���C��
void CircleGenForLine(vec2f pos, float R, D3DCOLOR color = COLOR_NORMAL);

//�l�p�̃��C���B�X�|���W�{�u�A�Y�{���͎l�p�B
void LineGenforSquare(vec2f pos, vec2f size, D3DCOLOR Color = COLOR_NORMAL);

void LineGenforSquare3D(vec2f pos, vec2f size, D3DCOLOR Color = COLOR_NORMAL);

//��{�̃��C�����쐬�B
void LineGen(vec2f startpos, vec2f endpos, D3DCOLOR Color = COLOR_NORMAL);

void LineGen3D(vec2f startpos, float size, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

//�O�p�`�B
void DeltaGen(vec2f pos, vec2f size, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

void CircleLightGen(vec2f pos, float R, unsigned Intensity, D3DCOLOR Color);
//�����_�����O�o�b�t�@��p�B
void FaceGenRenderRotate(vec2f pos, vec2f size,
	IDirect3DTexture9* Render, float w, float h,
	float angel = 0.f, D3DCOLOR Color = COLOR_NORMAL);

void RingGen(vec2f pos, float R1, float R2, D3DCOLOR color1, D3DCOLOR color2, int Prec = 5);
#endif