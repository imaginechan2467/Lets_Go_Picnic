/*==============================================================================

   [Grid.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<d3dx9.h>
#include"mydirect3D.h"
#include"Config.h"
#include<stdlib.h>

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
}Vertex3D;

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
static Vertex3D* g_pVertex = NULL;
static int g_VertexCount = 0;
static int g_LineCount = 0;

void GridInit(float size, float count, D3DCOLOR color)
{
	//���C�����@�w��+1
	g_LineCount = (int)(count + 1) * 2;
	//���_��
	g_VertexCount = g_LineCount * 2;

	//�w�肵�����_�����������m��
	g_pVertex = (Vertex3D*)malloc(g_VertexCount * sizeof(Vertex3D));

	float length = size * count;//���C��1�{�̒���
	float start = -length * 0.5f;//���C���̔����̒����@�����ɂ��Ȃ��ƂQ�{�ɂȂ�@�������l
	float end   = length * 0.5f;
	int n = g_LineCount;//���s�̃��C���̒��_��

	//���C���̕`���n�߂̒��_�ƁA�`���I���̒��_�̈ʒu��ݒ�i�c�̃��C���Ɖ��̃��C���j
	for (int i = 0; i < count + 1; i++)
	{
		g_pVertex[i * 2    ].Position = D3DXVECTOR3(start + size * i,0.0f, start);
		g_pVertex[i * 2 + 1].Position = D3DXVECTOR3(start + size * i,0.0f, end  );
		g_pVertex[n + i * 2    ].Position = D3DXVECTOR3(start, 0.0f, start + size * i);
		g_pVertex[n + i * 2 + 1].Position = D3DXVECTOR3(end  , 0.0f, start + size * i);
	}

	//�F��ݒ�
	D3DCOLOR centercolor = D3DCOLOR_RGBA(255, 0, 0, 255);
	for (int i = 0; i < g_VertexCount; i++)
	{
		g_pVertex[i].Color = color;

		if ((i == (g_VertexCount / 4) || i == (g_VertexCount / 4) - 1) ||
			(i == (g_VertexCount / 4) * 3 || i == (g_VertexCount / 4) * 3 - 1))
		{
			g_pVertex[i].Color = centercolor;
		}
	}
}

void GridDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//���[���h���W�ϊ��s��̍쐬
	D3DXMATRIX mtxWorld;
	//�P�ʍs��̍쐬
	D3DXMatrixIdentity(&mtxWorld);//mtxWorld = 1;
	//��]�s��̍쐬
	//D3DXMatrixRotationY(&mtxWorld, g_angle);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX3D);//���[�J����FVF�B�e�N�X�`���Ȃ��B

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, NULL);

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�|���S����`�悷��
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, g_LineCount, g_pVertex, sizeof(Vertex3D));
}


void GridUninit(void)
{
	free(g_pVertex);
	g_pVertex = NULL;
}
