////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "math.h"

////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>::Vector2() :
x(0),
y(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>::Vector2(T X, T Y) :
x(X),
y(Y)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U>& vector) :
x(static_cast<T>(vector.x)),
y(static_cast<T>(vector.y))
{
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator -(const Vector2<T>& right)
{
    return Vector2<T>(-right.x, -right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x + right.x, left.y + right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x - right.x, left.y - right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x * right, left.y * right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator *(T left, const Vector2<T>& right)
{
    return Vector2<T>(right.x * left, right.y * left);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator *=(Vector2<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator /(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x / right, left.y / right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x == right.x) && (left.y == right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x != right.x) || (left.y != right.y);
}

////////////////////////////////////////////////////////////
template <typename T>
inline float operator|(const Vector2<T>& left, const Vector2<T>& rhs)
{
	return left.x * rhs.x + left.y * rhs.y;
}

////////////////////////////////////////////////////////////
template <typename T>
inline float operator^(const Vector2<T>& left, const Vector2<T>& rhs)
{
	return left.x * rhs.y - left.y * rhs.x;
}

////////////////////////////////////////////////////////////
template <typename T>
inline float Vector2<T>::GetLength() const
{
	return sqrtf(x*x + y*y);
}

////////////////////////////////////////////////////////////
template <typename T>
inline float Vector2<T>::GetSqrLength() const
{
	return x*x + y*y;
}

////////////////////////////////////////////////////////////
template <typename T>
inline void	Vector2<T>::Normalize()
{
	float length = GetLength();
	x /= length;
	y /= length;
}

////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>	Vector2<T>::Normalized() const
{
	Vector2<T> res = *this;
	res.Normalize();
	return res;
}

////////////////////////////////////////////////////////////
template <typename T>
inline float	Vector2<T>::Angle(const Vector2<T>& to)
{
	float cosAngle = Clamp(Normalized() | to.Normalized(), -1.0f, 1.0f);
	float angle = ((acosf(cosAngle))*180.f/ 3.14159265358979323846) * Sign(*this ^ to);
	return angle;
}

////////////////////////////////////////////////////////////
template <typename T>
inline void Reflect(Vector2<T> normal, float elasticity/* = 1.0f*/)
{
	*this = *this - normal * (1.0f + elasticity) * (*this | normal);
}
