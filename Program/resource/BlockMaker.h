#ifndef BLOCKMAKER_H_
#define BLOCKMAKER_H_

#include "ObjectEntityClass.h"
#include "Block.h"

class BlockMakerClass :public GameObjectClass
{
public:

	//�R���X�g�f�X�g
	BlockMakerClass();
	~BlockMakerClass();

	//��{�֐�
	static void Init(void);
	static void Uninit(void);
	void UpdateAll(void);
	void DrawAll(void);
	void Update(void);
	void Draw(void);

	//�f�[�^����n
	void SaveAll(void);
	void LoadAll(void);
	void ResetAll(bool isDeleteAll);
	void Delete(int index);
	void Create(void);

	//�Z�b�g�n
	void SetType(int type);

	//�擾�n
	int GetDetailType(void);


private:

	static BlockClass::BlockType currentBlockType;
	static int blockMakerTexture;

};

#endif 