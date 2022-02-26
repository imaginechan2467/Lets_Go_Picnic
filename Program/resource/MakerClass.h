#ifndef MAKERCLASS_H_
#define MAKERCLASS_H_

#include "GameObjectClass.h"

class MakerClass :virtual public GameObjectClass
{
public:

	//コンストデスト
	MakerClass();
	~MakerClass();

	//基本関数
	void Draw(void);
	void Update(void);
	void DrawAll(void);
	void UpdateAll(void);

	//セット系
	void SetType(int type);
	void Refresh(void);

	//動作系
	virtual void DeleteEntity(void) = 0;
	virtual void ResetCurrentObj(void) = 0;

private:


};

#endif