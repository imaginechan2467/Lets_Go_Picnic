#ifndef ITEM_H_
#define ITEM_H_

#include "GameObjectClass.h"
#include "Config.h"

#define ITEM_START_SIZE (vec3f(0.5f,0.5f,0.5f))
#define ITEM_BOMB_COUNT (60 * 3)
#define ITEM_BOMB_UI_POS (vec2f(0.0f,SCREEN_HEIGHT - 64.0f))
#define ITEM_BOMB_CNT_POS (vec2f(64.0f + 8.0f,SCREEN_HEIGHT - 64.0f))
#define ITEM_BOMB_UI_SIZE (vec2f(64.0f,64.0f))

#define ITEM_SPRING_JUMP_SIZE (0.3f)

class ItemClass :public GameObjectClass
{
public:

	enum ItemType
	{
		ItemType_spring,		//装備したらジャンプしてる間しか移動できないけど高く飛べる
		ItemType_bomb,			//通常の地面を壊す
		ItemType_umbrella,		//落下を遅くする
		ItemType_coin,			//取得でスコア
		ItemType_goalpost,		//動作無し

		ItemType_MAX,
	};

	static std::vector<ItemClass*> itemList;

	//コンストデスト
	ItemClass();
	ItemClass(vec3f setpos, vec3f setsize, bool setisActive,
		ItemType settype, STAGE setstagenum, float setangle, vec3f setRotationDir);
	~ItemClass();

	//基本関数
	static void Init(void);
	static void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateAll(void);
	void DrawAll(void);

	//データ操作系
	static void SaveAll(void);
	void Load(bool isreset);
	void ResetAll(bool isDeleteAll);
	void Delete(int index);
	void Save(FILE* fp, int setobjtypedetail);
	void Create(vec3f setpos, vec3f setsize, bool setIsActive,
		int setobjDetailType, OBJECTTYPE setobjType, float setangle,vec3f setRotationDir);
	static void DeleteByUniqueNumber(int unique);

	//動作系
	void Move(DIRfor3D dir);

	//セット系
	void SetType(int type);
	void SetMatrix(void);
	void ChangeSize(DIRfor3D dir);
	static void SetBomb(vec3f setpos);
	static void SetUmbrella(bool set);
	static void UmbrellaSetPos(vec3f setpos);
	
	//取得系
	int GetDetailType(void);
	static bool GetIsActiveUmbrella(void);
	static ItemClass* GetUmbrella(void);

private:

	ItemType itemType;
	D3DXMATRIX mtxModel;
	float angle;
	vec3f AxisDir;

	static int itemModel[ItemType_MAX];
	static int bombUiTexture;

	bool isSet;
	static int iteminventry[ItemType_MAX];
	static ItemClass* pUmbrella;

};

#endif 