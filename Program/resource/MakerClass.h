#ifndef MAKERCLASS_H_
#define MAKERCLASS_H_

#include "GameObjectClass.h"

class MakerClass :virtual public GameObjectClass
{
public:

	//�R���X�g�f�X�g
	MakerClass();
	~MakerClass();

	//��{�֐�
	void Draw(void);
	void Update(void);
	void DrawAll(void);
	void UpdateAll(void);

	//�Z�b�g�n
	void SetType(int type);
	void Refresh(void);

	//����n
	virtual void DeleteEntity(void) = 0;
	virtual void ResetCurrentObj(void) = 0;

private:


};

#endif