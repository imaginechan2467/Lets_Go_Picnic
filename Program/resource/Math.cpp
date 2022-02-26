#include "Math.h"

float MyMath::Dot(const vec2f& v1, const vec2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float MyMath::Dot(const vec3f& v1, const vec3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float MyMath::Length2(const vec2f& v)
{
	return v.x * v.x + v.y * v.y;
}

float MyMath::Length(const vec2f& v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

float MyMath::Cross(const vec2f& v1, const vec2f& v2)
{
	float result = (v1.x * v2.y) - (v1.y * v2.x);
	result = roundf(result * 100) / 100;
	return result;
}

vec3f MyMath::Cross3D(const vec3f& v1, const vec3f& v2)
{
	vec3f result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}
float MyMath::Angle(const vec2f& v1, const vec2f& v2)
{
	return atan2(MyMath::Cross(v1, v2), MyMath::Dot(v1, v2));
}

vec2f MyMath::Normalize(const vec2f& vec)
{
	return vec / Length(vec);
}

bool MyMath::SameDirection(const vec2f& a, const vec2f& b)
{
	return (MyMath::Dot(a, b) > 0);
}

bool MyMath::SameDirection(const vec3f& a, const vec3f& b)
{
	return (MyMath::Dot(a, b) > 0);
}