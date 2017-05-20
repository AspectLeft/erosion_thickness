#ifndef __UTIL_H__
#define __UTIL_H__

#include <algorithm>
#include <vector>

namespace ErosionThickness
{
	struct Vector
	{
		float m_x, m_y, m_z;

		Vector() : m_x(0.0f), m_y(0.0f), m_z(0.0f) { }
		Vector(const Vector& v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) { }
		Vector(float x, float y, float z) : m_x(x), m_y(y), m_z(z) { }
		explicit Vector(float f) : m_x(f), m_y(f), m_z(f) { }


		float length2() const { return m_x * m_x + m_y * m_y + m_z * m_z; }
		float length()  const { return std::sqrt(length2()); }

		// Returns old length from before normalization (ignore the return value if you don't need it)
		float  normalize() { float len = length(); if (len > 0) *this /= len; return len; }
		// Return a vector in this same direction, but normalized
		Vector normalized() const { Vector r(*this); r.normalize(); return r; }

		float maxComponent() const { return std::max(std::max(m_x, m_y), m_z); }
		float minComponent() const { return std::min(std::min(m_x, m_y), m_z); }

		Vector& operator =(const Vector& v)
		{
			m_x = v.m_x;
			m_y = v.m_y;
			m_z = v.m_z;
			return *this;
		}

		Vector& operator +=(const Vector& v)
		{
			m_x += v.m_x;
			m_y += v.m_y;
			m_z += v.m_z;
			return *this;
		}

		Vector& operator -=(const Vector& v)
		{
			m_x -= v.m_x;
			m_y -= v.m_y;
			m_z -= v.m_z;
			return *this;
		}

		Vector& operator *=(const Vector& v)
		{
			m_x *= v.m_x;
			m_y *= v.m_y;
			m_z *= v.m_z;
			return *this;
		}

		Vector& operator *=(float f)
		{
			m_x *= f;
			m_y *= f;
			m_z *= f;
			return *this;
		}

		Vector& operator /=(const Vector& v)
		{
			m_x /= v.m_x;
			m_y /= v.m_y;
			m_z /= v.m_z;
			return *this;
		}

		Vector& operator /=(float f)
		{
			m_x /= f;
			m_y /= f;
			m_z /= f;
			return *this;
		}

		Vector operator -() const
		{
			return Vector(-m_x, -m_y, -m_z);
		}

	};

	inline Vector operator +(const Vector& v1, const Vector& v2)
	{
		return Vector(v1.m_x + v2.m_x,
			v1.m_y + v2.m_y,
			v1.m_z + v2.m_z);
	}


	inline Vector operator -(const Vector& v1, const Vector& v2)
	{
		return Vector(v1.m_x - v2.m_x,
			v1.m_y - v2.m_y,
			v1.m_z - v2.m_z);
	}


	inline Vector operator *(const Vector& v1, const Vector& v2)
	{
		return Vector(v1.m_x * v2.m_x,
			v1.m_y * v2.m_y,
			v1.m_z * v2.m_z);
	}


	inline Vector operator *(const Vector& v, float f)
	{
		return Vector(f * v.m_x,
			f * v.m_y,
			f * v.m_z);
	}


	inline Vector operator *(float f, const Vector& v)
	{
		return Vector(f * v.m_x,
			f * v.m_y,
			f * v.m_z);
	}


	inline Vector operator /(const Vector& v1, const Vector& v2)
	{
		return Vector(v1.m_x / v2.m_x,
			v1.m_y / v2.m_y,
			v1.m_z / v2.m_z);
	}


	inline Vector operator /(float f, const Vector& v)
	{
		return Vector(f / v.m_x,
			f / v.m_y,
			f / v.m_z);
	}


	inline float dot(const Vector& v1, const Vector& v2)
	{
		return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
	}


	inline Vector cross(const Vector& v1, const Vector& v2)
	{
		return Vector(v1.m_y * v2.m_z - v1.m_z * v2.m_y,
			v1.m_z * v2.m_x - v1.m_x * v2.m_z,
			v1.m_x * v2.m_y - v1.m_y * v2.m_x);
	}


	inline Vector max(const Vector& v1, const Vector& v2)
	{
		return Vector(std::max(v1.m_x, v2.m_x),
			std::max(v1.m_y, v2.m_y),
			std::max(v1.m_z, v2.m_z));
	}

	inline Vector min(const Vector& v1, const Vector& v2)
	{
		return Vector(std::min(v1.m_x, v2.m_x),
			std::min(v1.m_y, v2.m_y),
			std::min(v1.m_z, v2.m_z));
	}

	typedef Vector Point;

	inline void makeCoordinateSpace(const Vector& normalRef,
		Vector& outXAxis, Vector& outYAxis, Vector& outZAxis)
	{
		outZAxis = normalRef.normalized();
		Vector v2 = (outZAxis.m_x != 0.0f || outZAxis.m_z != 0.0f) ?
			Vector(0.0f, 1.0f, 0.0f) :
			Vector(1.0f, 0.0f, 0.0f);
		outXAxis = cross(v2, outZAxis).normalized();
		outYAxis = cross(outZAxis, outXAxis);
	}

	inline void makeCoordinateSpace(const Vector& normalRef, const Vector& tangent,
		Vector& outXAxis, Vector& outYAxis, Vector& outZAxis)
	{
		outZAxis = normalRef.normalized();
		outYAxis = cross(tangent, outZAxis).normalized();
		outXAxis = cross(outZAxis, outYAxis);
	}

	inline Vector transformToLocalSpace(const Vector& v,
		const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
	{
		return Vector(dot(v, xAxis), dot(v, yAxis), dot(v, zAxis));
	}

	inline Vector transformFromLocalSpacex(const Vector& v,
		const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
	{
		return Vector(v.m_x * xAxis.m_x + v.m_y * yAxis.m_x + v.m_z * zAxis.m_x,
			v.m_x * xAxis.m_y + v.m_y * yAxis.m_y + v.m_z * zAxis.m_y,
			v.m_x * xAxis.m_z + v.m_y * yAxis.m_z + v.m_z * zAxis.m_z);
	}

	struct Face
	{
		inline Face() { };
		std::vector<int> points; //index of points
	};

}


#endif // !__UTIL_H__

