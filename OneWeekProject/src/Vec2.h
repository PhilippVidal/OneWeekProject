#pragma once

#include <cmath>

template<typename T>
struct Vec2
{
	union 
	{
		struct
		{
			T x;
			T y;
		};

		struct
		{
			T min;
			T max;
		};
	};

	Vec2<T>()
		: x(0), y(0) {}

	template<typename TypeParam1, typename TypeParam2>
	Vec2<T>(TypeParam1 x, TypeParam2 y)
		: x(x), y(y) {}


	template<typename TypeOther>
	Vec2<T>(const Vec2<TypeOther>& other)
		: x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

	float Magnitude() const
	{
		return sqrtf(MagnitudeSqr());
	}

	float MagnitudeSqr() const
	{
		return x * x + y * y;
	}

	template<typename TypeOther>
	Vec2<T> Multiply(const Vec2<TypeOther>& other) const
	{
		return { x * other.x, y * other.y };
	}


	template<typename TypeParam1, typename TypeParam2>
	Vec2<T> Clamped(TypeParam1 min, TypeParam2 max) const
	{
		float magnitudeSqr = MagnitudeSqr();
		if (magnitudeSqr < (min * min))
		{
			float sqrt = sqrtf(magnitudeSqr);
			return Vec2f(x / sqrt, y / sqrt) * min;
		}
		else if (magnitudeSqr > (max * max))
		{
			float sqrt = sqrtf(magnitudeSqr);
			return Vec2f(x / sqrt, y / sqrt) * max;
		}

		return { x, y };
	}

	template<typename TypeOther>
	float Distance(const Vec2<TypeOther>& other) const
	{
		return (*this - other).Magnitude();
	}

	Vec2<T> Normalized() const
	{
		float magnitude = Magnitude();
		return { x / magnitude, y / magnitude };
	}


	template<typename TypeParam1>
	Vec2<T> operator+(const TypeParam1 other) const
	{
		return { x + other, y + other };
	}

	template<typename TypeOther>
	Vec2<T> operator+(const Vec2<TypeOther>& other) const
	{
		return { x + other.x, y + other.y };
	}

	template<typename TypeParam1>
	Vec2<T> operator-(const TypeParam1 other) const
	{
		return { x - other, y - other };
	}

	template<typename TypeOther>
	Vec2<T> operator-(const Vec2<TypeOther>& other) const
	{
		return { x - other.x, y - other.y };
	}

	template<typename TypeOther>
	float operator*(const Vec2<TypeOther>& other) const
	{
		return x * other.y - y * other.x;
	}

	template<typename TypeParam1>
	Vec2<T> operator*(const TypeParam1 other) const
	{
		return { x * other, y * other };
	}

	template<typename TypeParam1>
	Vec2<T> operator/(const TypeParam1 other) const
	{
		return { x / other, y / other };
	}

	//Should add epsilon for float vectors
	template<typename TypeOther>
	bool operator==(const Vec2<TypeOther>& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	template<typename TypeOther>
	bool operator!=(const Vec2<TypeOther>& other) const
	{
		return !(*this == other);
	}
};


typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2i PixelCoords;