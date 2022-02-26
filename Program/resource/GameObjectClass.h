#ifndef GAMEOBJECTCLASS_H_
#define GAMEOBJECTCLASS_H_

#include <d3dx9.h>
#include "Config.h"
#include "Collision.h"
#include <vector>

#define OBJECT_SAVEDATA_FILENAME_START ("asset/data/")
#define OBJECT_SAVEDATA_FILENAME_END ("savedata.dat")
#define OBJECT_MOVE_SPEED (1.0f)
#define GRAVITY (0.01f)
#define GRAVITY_FOR_UMBRELLA (0.0005f)



class GameObjectClass
{
public:

	enum OBJECTTYPE
	{
		objtype_block,
		objtype_item,
		objtype_player,
		objtype_system,

		objtype_MAX,
	};

	enum STAGE
	{
		stage_1,
		stage_2,
		stage_3,
		stage_4,
		stage_5,

		stage_MAX,
	};

	//コンスト/デスト
	GameObjectClass(vec3f setpos, vec3f setsize,
		bool setisActive, OBJECTTYPE settype, STAGE setstage);//オブジェクト作成時
	GameObjectClass();
	~GameObjectClass();

	//クラス管理
	static void Init(void);

	//継承する基本関数
	virtual void Draw(void) = 0;
	virtual void Update(void) = 0;
	virtual void UpdateAll(void) = 0;
	virtual void DrawAll(void) = 0;

	//データ操作系
	virtual void Save(FILE* fp, int setobjtypedetail);//一個一個のセーブ
	virtual void Delete(int index) = 0;
	virtual void Load(bool isreset) = 0;
	virtual void Create(vec3f setpos, vec3f setsize, bool setIsActive,
		int setobjDetailType, OBJECTTYPE setobjType,
		float setangle = 0.0f, vec3f setDir = { 0.0f,0.0f,0.0f }) = 0;

	//セット系
	static void SetCurrentStage(STAGE setstage);
	void SetPos(vec3f setpos);
	void SetSize(vec3f setsize);
	void SetIsActive(bool set);
	void SetColor(D3DCOLOR setcolor);
	virtual void ChangeSize(DIRfor3D dir);
	virtual void SetType(int type) = 0;
	void ChangeIsActive(void);
	virtual void ResetStatus(void);
	virtual void ResetAll(bool isalldelete) = 0;

	//動作系
	virtual void Move(DIRfor3D dir);

	//取得系
	static STAGE GetCurrentStage(void);
	vec3f GetPos(void);
	vec3f GetSize(void);
	bool GetIsActive(void);
	int GetUniqueNum(void);
	D3DCOLOR GetColor(void);
	STAGE GetStageNumber(void);
	OBJECTTYPE GetObjType(void);
	virtual int GetDetailType(void) = 0;
	static char* MakeSaveDataFileName(char* Out, OBJECTTYPE objType);
	Collision GetCollision(void);

protected:

	OBJECTTYPE objType;
	int uniqueNumber;
	vec3f pos;
	vec3f size;
	bool isActive;
	D3DCOLOR color;
	STAGE stageNumber;
	Collision collision;

	vec3f front;

	int cnt;
	int frame;
	int waitCnt;

private:

	static STAGE currentStage;
	static int objectCount;
	static const char* ObjectTypeName[];

};

#endif
