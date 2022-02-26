#ifndef STARTMENU_H_
#define STARTMENU_H_

class StartMenuClass
{
public:

	enum STARTMENUBOTTUN
	{
		startmenu_play,
		startmenu_end,

		startmenu_MAX,
	};

	static void Init(void);
	static void Update(void);
	static void Draw(void);
	static void Uninit(void);

private:

	static int backgroundTexture;
	static int bottunNormalBackgroundTexture;
	static int bottunChooseBackgroundTexture;
	static int bottunNormalTextTexture;
	static int titleCharaTexture;
	static int titleBackgroundTexture;

	static STARTMENUBOTTUN currentBottun;

	static int cnt;
	static int frame;

};

#endif