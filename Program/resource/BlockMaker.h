#ifndef BLOCKMAKER_H_
#define BLOCKMAKER_H_

#include "ObjectEntityClass.h"
#include "Block.h"

class BlockMakerClass :public GameObjectClass
{
public:

	//コンストデスト
	BlockMakerClass();
	~BlockMakerClass();

	//基本関数
	static void Init(void);
	static void Uninit(void);
	void UpdateAll(void);
	void DrawAll(void);
	void Update(void);
	void Draw(void);

	//データ操作系
	void SaveAll(void);
	void LoadAll(void);
	void ResetAll(bool isDeleteAll);
	void Delete(int index);
	void Create(void);

	//セット系
	void SetType(int type);

	//取得系
	int GetDetailType(void);


private:

	static BlockClass::BlockType currentBlockType;
	static int blockMakerTexture;

};

#endif 