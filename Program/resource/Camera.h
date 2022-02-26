#ifndef CAMERA_H_
#define CAMERA_H_

#include "Config.h"

#define CAMERA_ROTATION_SPEED (0.001f)
#define ROTATION_SPEED_FOR_PLAY (0.03f)
#define NEAR_MAX (3.0f)
#define FAR_MAX (20.0f)

class CameraClass
{
public:

	CameraClass();
	~CameraClass();

	//äÓñ{ä÷êî
	static void Init(void);
	static void Uninit(void);
	void Update(void);
	void Draw(void);

	//ìÆçÏånä÷êî
	void SetViewMatrix(void);
	void Move(DIRfor3D setdir);
	void Role(DIR setdir, bool isFollowPlayer);
	void RoleforClear(DIR setdir);
	void Refresh(void);
	void SetAngleByDir(DIR setdir);
	void SetPos(vec3f setpos);
	static void SetDistance(bool isAdd);

	//éÊìæånä÷êî
	static const vec3f& GetFront(void);
	static const vec3f& GetAt(void);
	static const vec3f& GetRight(void);
	static const vec3f& GetPos(void);
	static const D3DXMATRIX& GetView(void);

private:

	static vec3f pos;
	static vec3f vAt;
	static vec3f vUp;
	static vec3f vFront;
	static vec3f vRight;
	static D3DXMATRIX mtxView;
	static float distance;


	bool isActive;
	static bool isFps;
	DIR frontDir;

};

#endif