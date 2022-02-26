

#include "Xmodel.h"
#include "mydirect3D.h"
#include "texture.h"
#include "Config.h"
#include "Light.h"

#define XMODEL_MAX (64)

struct XMODEL
{
	LPD3DXMESH pMesh;//���b�V���@�C���f�b�N�X�Ƃ����_�Ƃ����Ǘ����Ă郄�c�B
	//LPD3DXBUFFER pAdjacency;//�אڏ��o�b�t�@
	DWORD materialCnt;//�}�e���A����
	//�}�e���A�����
	D3DMATERIAL9* pMaterials;
	//�e�N�X�`��ID
	int* pTextureIds;
};


static XMODEL g_XModels[XMODEL_MAX] = {};


void XModelInit(void)
{
	XModelReleaseAll();
}

int XModelLoad(const char * pFileName)
{
	//g_XModels���疢�g�p�̈������

	int id = XMODEL_INVALID_ID;
	for (int i = 0; i < XMODEL_MAX; i++)
	{
		if (g_XModels[i].pMesh == NULL)
		{
			id = i;
			break;
		}
	}

	if (id == XMODEL_INVALID_ID)
	{
		return XMODEL_INVALID_ID;//���g�p�̈悪�Ȃ�����
	}

	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	LPD3DXBUFFER pAdjacency;	//�אڏ��o�b�t�@(�ꎞ�g�p)
	LPD3DXMESH pTempMesh;		//���f�����b�V��(�ꎞ�g�p)
	LPD3DXBUFFER pMaterial;
	DWORD materialCount;

	//�Ƃ肠�����ҏW�\�ȏ�Ԃœǂݍ���
	hr = D3DXLoadMeshFromX(
		pFileName,			//�t�@�C����
		D3DXMESH_SYSTEMMEM,	//�I�v�V����(���C���������ɏ����쐬)
		pDevice,			//Direct3D�f�o�C�X
		&pAdjacency,		//�אڏ��(�擾)
		&pMaterial,			//�}�e���A�����(�擾)
		NULL,				//�G�t�F�N�g�i�V�F�[�_�[�j(�擾)
		&materialCount,		//�}�e���A����(�擾)
		&pTempMesh
	);

	g_XModels[id].materialCnt = materialCount;
	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterial->GetBufferPointer();

	g_XModels[id].pMaterials = new D3DMATERIAL9[materialCount];
	g_XModels[id].pTextureIds = new int[materialCount];

	for (unsigned i = 0; i < materialCount; i++)
	{
		//�}�e���A���l�̃R�s�[
		//�v�f��I��ŃR�s�[�����������������B
		g_XModels[id].pMaterials[i] = pD3DXMaterial[i].MatD3D;
		//g_XModels[id].pMaterials[i].Diffuse = pD3DXMaterial[i].MatD3D.Emissive;
		pD3DXMaterial[i].MatD3D.Emissive = {1,1,1};

		pD3DXMaterial[i].pTextureFilename;

		//�e�N�X�`���t�@�C�����̎擾
		if (pD3DXMaterial[i].pTextureFilename)
		{
			char buf[256] = {};
			strcpy(buf, pFileName);

			char* pSplit = strrchr(buf, '/');
			*(pSplit + 1) = '\0';

			strcat(buf, pD3DXMaterial[i].pTextureFilename);
			g_XModels[id].pTextureIds[i] = SetTexture(buf);
		}
		else
		{
			g_XModels[id].pTextureIds[i] = TEXTURE_INVALID_ID;
		}
	}
	pMaterial->Release();

	if (FAILED(hr))
	{
		MessageBox(NULL, "���f���ǂݍ��ݎ��s", "�G���[", NULL);
		return XMODEL_INVALID_ID;
	}

	//���b�V���̍œK��������
	pTempMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);

	//�����Ŗʓ|�����Ă���钸�_�o�b�t�@����舵�����b�V���Ƃ��ăN���[�����쐬����
	pTempMesh->CloneMeshFVF(
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		pTempMesh->GetFVF(),
		pDevice,
		&g_XModels[id].pMesh
	);

	//�p�ς݂̃o�b�t�@���폜
	pAdjacency->Release();
	pTempMesh->Release();

	return id;
}

void XModelDraw(int id, D3DXMATRIX * mtxModel)
{
	if (!g_XModels[id].pMesh)return;//�w�肵�����f���ԍ������g�p�Ȃ�B

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, mtxModel);

	SetLight(true);

	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	for (DWORD i = 0; i < g_XModels[id].materialCnt; i++)
	{
		pDevice->SetMaterial(&g_XModels[id].pMaterials[i]);//�}�e���A����ݒ�

		pDevice->SetTexture(0, Texture_GetTexture(g_XModels[id].pTextureIds[i]));

		//FVF�̐ݒ�A���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���f�o�C�X�ɐݒ�->DrawPrimitive
		g_XModels[id].pMesh->DrawSubset(i);
	}
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	SetLight(false);
}

void XModelUninit(void)
{

}

void XModelRelease(const int * pID, int cnt)
{
	for (int i = 0; i < cnt; i++)
	{
		if (!g_XModels[i].pMesh)continue;

		g_XModels[pID[i]].pMesh->Release();
		g_XModels[pID[i]].pMesh = NULL;

		delete[] g_XModels[pID[i]].pTextureIds;
		delete[] g_XModels[pID[i]].pMaterials;
	}
}

void XModelReleaseAll(void)
{
	for (int i = 0; i < XMODEL_MAX; i++)
	{
		XModelRelease(&i, 1);
	}
}