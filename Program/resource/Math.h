#ifndef MATH_H_
#define MATH_H_

#include<d3dx9.h>
#include "Config.h"
#include<vector>

namespace MyMath
{
	//�x�N�g���̊�{�v�Z
	float Dot(const vec2f& v1, const vec2f& v2);		//����
	float Dot(const vec3f& v1, const vec3f& v2);		//3D����
	float Length2(const vec2f& v);
	float Length(const vec2f& v);						//����
	float Cross(const vec2f& v1, const vec2f& v2);		//�O��
	vec3f Cross3D(const vec3f& v1, const vec3f& v2);	//3D�O��
	float Angle(const vec2f& v1, const vec2f& v2);		//�p�x
	vec2f Normalize(const vec2f& vec);					//�W����

	//�x�N�g���̊�{���f
	bool SameDirection(const vec2f& a, const vec2f& b);	//���������f
	bool SameDirection(const vec3f& a, const vec3f& b);	//3D���������f
}

#endif