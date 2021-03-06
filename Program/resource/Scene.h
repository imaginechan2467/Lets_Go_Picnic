#ifndef SCENE_H_
#define SCENE_H_


class SceneClass
{
public:

	enum SCENE
	{
		scene_startmenu,
		scene_stagemenu,
		scene_game,
		scene_score,

		SCENEMAX,
	};

	//基本関数
	static void Init(void);
	static void Update(void);
	static void Draw(void);
	static void Uninit(void);

	//監視系関数
	static void ExecuteChangeScene(void);

	//取得系関数
	static SCENE GetCurrentScene(void);

	//セット系関数
	static void ChangeScene(SCENE next);

private:

	static SCENE currentScene;
	static SCENE nextScene;

};

#endif