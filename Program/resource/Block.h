#ifndef BLOCK_H_
#define BLOCK_H_

#include "GameObjectClass.h"

#define BLOCK_TEXTURE_FILENAME ("asset/texture/spice_and_wolf.png")
#define BLOCK_ONE_FACE_VERTEX_NUM (6)
#define FVF_BLOCK (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define BLOCK_INDEX_MAX (36)
#define BLOCK_VERTEX_MAX (24)
#define BLOCK_PRIMITIVE_MAX (12)

#define BLOCK_MOVE_UP_MAX ( 5.0f)
#define BLOCK_MOVE_SIDE_MAX ( 5.0f)
#define BLOCK_FALL_COUNT (60 * 3)
#define BLOCK_MOVE_SPEED (0.01f)
#define BLOCK_SWITCH_COUNT ( 60 * 5)
#define BLOCK_FALL_MAX_SIZE (5.0f)

#define BLOCK_NOTINDEX_VERTEX_MAX (36)

typedef struct
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;		//�@���̒����͕K��1�B
	D3DCOLOR Color;
	D3DXVECTOR2 TexCord;
}Vertex3D;

class BlockBaseClass
{
public:

	static void Init(void);
	static void Uninit(void);

	//�擾�n�֐�
	static LPDIRECT3DVERTEXBUFFER9 GetVerTexBuffer(void);
	static LPDIRECT3DINDEXBUFFER9 GetIndexBuffer(void);
	static void DrawBoxNotIndex(D3DXMATRIX mtx, int texID, D3DCOLOR setcolor = COLOR_NORMAL);

	static void DrawBoxNotIndexforTex(D3DXMATRIX mtx, int texID,
		Int2 cut = { 1,1 }, D3DCOLOR setcolor = COLOR_NORMAL);
	

private:

	static Vertex3D CubeVertex[BLOCK_VERTEX_MAX];
	static LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
};

class BlockClass:public GameObjectClass
{
public:

	enum BlockType
	{
		BlockType_normal,		//�ʏ�
		BlockType_rock,			//��
		BlockType_fire,			//�}�O�}
		//BlockType_ice,		//�X
		//BlockType_cushion,	//�N�b�V����
		BlockType_fall,			//�����鏰
		BlockType_elevator,		//�G���x�[�^�[�c
		BlockType_horizontal,	//�G���x�[�^�[��
		BlockType_goal,			//�S�[��(�Ő��̏�)

		BlockType_MAX,			
	};

	enum BlockFace
	{
		BlockFace_front = 0,
		BlockFace_right = 6,
		BlockFace_back = 12,
		BlockFace_left = 18,
		BlockFace_top = 24,
		BlockFace_under = 30,

		BlockFace_MAX,
	};

	static std::vector<BlockClass*> blockList;

	//�R���X�g�f�X�g
	BlockClass();
	BlockClass(vec3f setpos, vec3f setsize, bool setisActive,
		BlockType settype, STAGE setstagenum);
	~BlockClass();

	//��{�֐�
	static void Init(void);
	static void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawAll(void);
	void UpdateAll(void);

	//�f�[�^����n
	static void SaveAll(void);
	void Load(bool isreset);
	void ResetAll(bool isDeleteAll);
	void Delete(int index);
	void Create(vec3f setpos, vec3f setsize, bool setIsActive,
		int setobjDetailType, OBJECTTYPE setobjType,
		float setangle = 0.0f, vec3f setDir = { 0.0f,0.0f,0.0f });
	static void DeleteByUniqueNumber(int unique);
	static void DeleteByType(BlockType deletetype);
	
	//�Z�b�g�n
	void SetType(int type);
	void SetFallCountAdd(void);
	void SetIsTouched(bool set);

	//�擾�n
	int GetDetailType(void);
	
	//�`��n
	static void DrawCube(const D3DXMATRIX& mtxModel, int texID);

private:

	BlockType blockType;
	static int blockTexture[BlockType_MAX];
	float fallspeed;
	bool isBack;
	bool isTouched;
	vec3f tempvec3f_1;
	bool isSet;

};

#endif 