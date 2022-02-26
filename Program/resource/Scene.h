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

	//��{�֐�
	static void Init(void);
	static void Update(void);
	static void Draw(void);
	static void Uninit(void);

	//�Ď��n�֐�
	static void ExecuteChangeScene(void);

	//�擾�n�֐�
	static SCENE GetCurrentScene(void);

	//�Z�b�g�n�֐�
	static void ChangeScene(SCENE next);

private:

	static SCENE currentScene;
	static SCENE nextScene;

};

#endif