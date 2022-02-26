/*==============================================================================

   [myDirect3D.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include <d3d9.h>
#include"Config.h"

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
static LPDIRECT3D9 g_pD3D = NULL;//Direct3D�C���^�[�t�F�[�X
static PDIRECT3DDEVICE9 g_pDevice = NULL;//Direct3D�f�o�C�X�C���^�[�t�F�[�X


bool MyDirect3D_INIT(HWND hWnd)
{
	//Direct3D�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		//Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s
		MessageBox(NULL, "Direct3D�C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	/*Direct3D�f�o�C�X�̎擾*/

	//�f�o�C�X�̎擾�ɕK�v�ȏ��\���̂̍쐬
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = (UINT)SCREEN_WIDTH;					//�o�b�N�o�b�t�@�̉����@�_�u���o�b�t�@�̌�둤�H
	d3dpp.BackBufferHeight = (UINT)SCREEN_HEIGHT;				//�o�b�N�o�b�t�@�̏c��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					//�o�b�N�o�b�t�@�̃t�H�[�}�b�g�@�t���X�N���[���ɂ���Ƃ��͋C�����镔��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�X���b�v���@�̐ݒ�@D3DSWAPEFFECT_DISCARD�ł�������Oke
	d3dpp.Windowed = TRUE;										//�E�B���h�E or �t���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;						//�[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//�[�x�o�b�t�@�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//�t���X�N���[�����̃��t���b�V�����[�g�̎w��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//���t���b�V�����[�g��Present�����̊֌W	�t���X�N���[���Ȃ�30fps�Ƃ�60�Ƃ������������߂��
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//���t���b�V�����[�g��Present�����̊֌W	�t���X�N���[���Ȃ�30fps�Ƃ�60�Ƃ������������߂��

	//Direct3D�f�o�C�X�̎擾
	HRESULT hr;
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);//D3DCREATE_HARDWARE_VERTEXPROCESSING�͂ǂ��Œ��_�v�Z�����邩�i�f�o�C�X���\�t�g���j

	//HRESULT�^�ϐ��͐����⎸�s���l�Ƃ��ē����Ă���̂Ŏ��s�␬���������肷��ꍇ�̓}�N���𗘗p����@FAILED�ihr�j SUCCEEDED(hr)=�������Ă���^
	if (FAILED(hr))//���s���^
	{
		//�f�o�C�X�̎擾�Ɏ��s
		MessageBox(NULL, "Direct3D�f�o�C�X�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	//UV�A�h���b�V���O���[�h�̐ݒ�
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//BORDER MIRROR CLAMP WRAP
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 255, 255, 255));

	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

	//�A���t�@�u�����h
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�A���t�@�u�����h��L����
	g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���_�J���[�ƃe�N�X�`���̃u�����h�ݒ�
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//�@���x�N�g���͕K�����K�����Ďg�p����ݒ�ɂ���
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//���e�X�g
	/*g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	return true;//����������
}


void MyDirect3D_FIN(void)
{
	if (g_pDevice)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}