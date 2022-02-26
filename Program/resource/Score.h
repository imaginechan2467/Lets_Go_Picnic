#ifndef SCORE_H_
#define SCORE_H_



enum ScoreMenu
{
	scoremenu_next,
	scoremenu_replay,
	scoremenu_satrtmenu,
	scoremenu_MAX,
};

class ScoreClass
{
public:

	ScoreClass();
	~ScoreClass();

	static void Init(void);
	static void Update(void);
	static void Draw(void);
	static void Uninit(void);

private:

	static ScoreMenu currentMenu;
	static int BottunTexture;
	static int CurrentBottunFrameTexture;
	static int CoinTexture;
	static int CharaTexture;
	static int BottunCharaTexture;

	static int cnt;
	static int frame;

};

#endif