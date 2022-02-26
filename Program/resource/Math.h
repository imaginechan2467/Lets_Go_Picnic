#ifndef MATH_H_
#define MATH_H_

#include<d3dx9.h>
#include "Config.h"
#include<vector>

namespace MyMath
{
	//ベクトルの基本計算
	float Dot(const vec2f& v1, const vec2f& v2);		//内積
	float Dot(const vec3f& v1, const vec3f& v2);		//3D内積
	float Length2(const vec2f& v);
	float Length(const vec2f& v);						//長さ
	float Cross(const vec2f& v1, const vec2f& v2);		//外積
	vec3f Cross3D(const vec3f& v1, const vec3f& v2);	//3D外積
	float Angle(const vec2f& v1, const vec2f& v2);		//角度
	vec2f Normalize(const vec2f& vec);					//標準化

	//ベクトルの基本判断
	bool SameDirection(const vec2f& a, const vec2f& b);	//同方向判断
	bool SameDirection(const vec3f& a, const vec3f& b);	//3D同方向判断
}

#endif