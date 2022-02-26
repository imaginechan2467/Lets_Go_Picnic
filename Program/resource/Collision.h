#ifndef COLLISION_H_
#define COLLISION_H_

#include<d3dx9.h>
#include "Config.h"

enum CubeDir
{
	cubedir_RUF,//�E����O
	cubedir_RUB,//�E����
	cubedir_RTF,//�E���O
	cubedir_RTB,//�E�㉜

	cubedir_LUF,//������O
	cubedir_LUB,//������
	cubedir_LTF,//�����O
	cubedir_LTB,//���㉜

	cubedir_MAX,
};

class Collision
{
public:

	typedef struct
	{
		vec3f center;
		float radius;
	}CollisionCircle;

	typedef struct
	{
		vec3f adjustpos;
		bool isHit;
		DIRfor3D hitDir;
		bool hitdir3[3];
	}ColInfo;

	typedef struct
	{
		vec3f adjustpos;
		bool isHit;
		bool hitvertex[cubedir_MAX];
	}ColInfo_2;

	//�~�ǂ����̓����蔻��
	static bool CollisionCircleAndCircleHit(const CollisionCircle* pA, const CollisionCircle* pB);

	static ColInfo_2 GetBoxVertexHit(const vec3f& Apos,
		const vec3f& Asize, const vec3f& Bpos, const vec3f& Bsize, DIRfor3D preDir);

	static bool GetBoxColSimple(const vec3f& Apos,
		const vec3f& Asize, const vec3f& Bpos, const vec3f& Bsize);

	static bool GetBoxOnlyHit(const vec3f& Apos,
		const vec3f& Asize, const vec3f& Bpos, const vec3f& Bsize);

private:



};

#endif