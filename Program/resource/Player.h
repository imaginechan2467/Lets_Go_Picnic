#ifndef PLAYER_H_
#define PLAYER_H_

#include <d3dx9.h>
#include "GameObjectClass.h"


class PlayerClass:public GameObjectClass
{
public:
	enum PLAYERTYPE
	{
		playerType_normal,
		playerType_rock,
		playerType_fire,
		playerType_ice,
		playerType_rubber,
		playerType_MAX,
	};

	enum MOVESTATE
	{
		movestate_walk,
		movestate_run,
		movestate_push,
		movestate_MAX,
	};

	PlayerClass();
	~PlayerClass();

	//��{�֐�
	static void Init(void);
	static void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateAll(void);
	void DrawAll(void);

	//�f�[�^����n�֐�
	void Create(vec3f setpos, vec3f setsize, bool setIsActive,
		int setobjDetailType, OBJECTTYPE setobjType,
		float setangle = 0.0f, vec3f setDir = { 0.0f,0.0f,0.0f });
	void Delete(int index);
	void Load(bool isreset);

	//�Z�b�g�n�֐�
	void SetType(int type);
	void ResetAll(bool isalldelete);
	void ResetStatus(void);
	void SetOnFloorIndex(int set);
	void SetGravity(bool isnormal);

	//����n
	void Move(DIRfor3D dir);
	//todo:�A�C�e���n������l����

	//�擾�n
	int GetDetailType(void);
	int GetOnFloorIndex(void);
	static bool GetIsOnFloor(void);
	DIRfor3D GetPlayerMoveDir(void);
	bool GetIsUp(void);
	static int GetTakenCoinNum(void);

private:

	PLAYERTYPE playerType;
	static int playerTexture;
	float weight;
	int HP;
	MOVESTATE moveState;

	static 	bool isTouchGround;
	float fallSpeed;
	int onFloorIndex;
	DIRfor3D oldMoveDir;
	float playerGravity;
	vec3f oldShadePos;

	static int HPTexture;
	static int takenCoinNum;
	static int coinUITex;

};

#endif