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

	//Šî–{ŠÖ”
	static void Init(void);
	static void Update(void);
	static void Draw(void);
	static void Uninit(void);

	//ŠÄ‹ŒnŠÖ”
	static void ExecuteChangeScene(void);

	//æ“¾ŒnŠÖ”
	static SCENE GetCurrentScene(void);

	//ƒZƒbƒgŒnŠÖ”
	static void ChangeScene(SCENE next);

private:

	static SCENE currentScene;
	static SCENE nextScene;

};

#endif