

#include<d3dx9.h>
#include"mydirect3D.h"


void LightUpdate(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-1.0f, -1.0f, 0.5f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	light.Ambient.a = 1.0f;

	pDevice->SetLight(0, &light);//������:���C�gNUM�B�Ȃ񂱂��ݒ�ł���B
	pDevice->LightEnable(0, TRUE);

	//�f�B�t���[�Y�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);//D3DMCS_COLOR1�Œ��_�J���[���}�e���A���ɂȂ� 2�̓X�y�L�����[

	//�A���r�G���g���C�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(128, 0, 128, 128));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(0, 0, 0, 255));

	//�A���r�G���g�̃}�e���A����ݒ�
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	//���C�g���g�p���邩�ݒ肷��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void SetLight(bool set)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, set);
}