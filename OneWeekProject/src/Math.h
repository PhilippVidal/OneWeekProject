#pragma once
#include "Vec2.h"


#define PI 3.14159265358979323846
#define HALF_PI (PI / 2)

class Math
{
public:

	static bool EvaluateCollisionAABBs(const Vec2f& positionA, const Vec2f& halfSizeA, const Vec2f& positionB, const Vec2f& halfSizeB);

	static bool EvaluateCollisionCircles(const Vec2f& positionA, float radiusA, const Vec2f& positionB, float radiusB);

	//** A = Circle, B = AABB */
	static bool EvaluateCollisionCircleAABB(const Vec2f& positionA, float radiusA, const Vec2f& positionB, const Vec2f& halfSizeB);

	static float GetRandomFloat(float min, float max);

	template<class T>
	static T Lerp(T a, T b, float t);

	template<class T>
	static float InverseLerp(T a, T b, T c);

	template<class T>
	static T Remap(T fromValue, T fromMin, T fromMax, T toMin, T toMax);
};

template<class T>
inline T Math::Lerp(T a, T b, float t)
{
	return a + (b - a) * t;
}

template<class T>
inline float InverseLerp(T a, T b, T v)
{
	return (v - a) / (b - a);
}

template<class T>
inline T Math::Remap(T fromValue, T fromMin, T fromMax, T toMin, T toMax)
{
	float t = InverseLerp(fromMin, fromMax, fromValue);
	return Lerp(toMin, toMax, t);
}