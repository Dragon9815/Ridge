#pragma once

#include "Quaternion.h"

#include <ostream>
#include <cmath>
#include <stdexcept>

namespace Ridge {

	// Forward declarations ----------------------------
	template<typename T> class Vector2;
	template<typename T> class Vector4;
	// -------------------------------------------------

	template<typename T>
	class Vector3 {
	public:
		// Implicit basic constructors
		Vector3()
			: Vector3<T>(0, 0, 0) {}

		Vector3(const Vector3<T>& v)
			= default;

		// Explicit basic constructors
		explicit Vector3(T scalar)
			: x(scalar)
			, y(scalar)
			, z(scalar)
		{}

		Vector3(T x, T y, T z)
			: x(x)
			, y(y)
			, z(z)
		{}

		// Conversion constructors
		template<typename A, typename B, typename C>
		Vector3(A x, B y, C z)
			: x(static_cast<T>(x))
			, y(static_cast<T>(y))
			, z(static_cast<T>(z))
		{}

		// Conversion Vector constructors
		template<typename U>
		Vector3(const Vector2<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(0))
		{}

		template<typename U>
		Vector3(const Vector3<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z))
		{}

		template<typename U>
		Vector3(const Vector4<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z))
		{}

		// Default assignment operator
		Vector3<T>& operator=(const Vector3& v) 
			= default;

		// Unary operators
		inline Vector3<T> operator+() const
		{
			return *this;
		}

		inline Vector3<T> operator-() const
		{
			return Vector3<T>(0) - *this;
		}

		// Component access operators
		inline T& operator[](int i)
		{
			switch (i) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				throw std::runtime_error("index i out of bounds");
			}
		}

		inline const T& operator[](int i) const
		{
			switch (i) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				throw std::runtime_error("index i out of bounds");
			}
		}

		inline T Length() const
		{
			return static_cast<T>(std::sqrt(x*x + y*y + z*z));
		}

		inline Vector3<T> Normalize() const
		{
			return *this / Length();
		}


		inline T Dot(const Vector3<T>& other) const
		{
			return x * b.x + y * b.y + z * b.z;
		}

		inline Vector3<T> Cross(const Vector3<T>& b) const
		{
			Vector3<T> result;
			result.x = y * b.z - z * b.y;
			result.y = z * b.x - x * b.z;
			result.z = x * b.y - y * b.x;
			return result;
		}

		inline Vector3<T> Rotate(const Vector3<T>& axis, T angle)
		{
			Rotate(Quaternion::FromAxisAngle<T>(axis, angle))
		}

		inline Vector3<T> Rotate(const Quaternion<T>& quat)
		{
			Quaternion<T> conjugate = quat.Conjugate();
			return quat * Quaternion(*this) * conjugate;
		}

	public:
		union {
			struct { T x, y, z; };
			struct { T r, g, b; };
		};
	};

	// Arithmetic operators
	// Add -----------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector3<A> operator+(const Vector3<A>& a, const Vector3<B>& b) 
	{
		Vector3<A> result;
		result.x = a.x + static_cast<A>(b.x);
		result.y = a.y + static_cast<A>(b.y);
		result.z = a.z + static_cast<A>(b.z);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<A> operator+(const Vector3<A>& a, B scalar) 
	{
		Vector3<A> result;
		result.x = a.x + static_cast<A>(scalar);
		result.y = a.y + static_cast<A>(scalar);
		result.z = a.z + static_cast<A>(scalar);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<B> operator+(A scalar, const Vector3<B>& b) 
	{
		Vector3<B> result;
		result.x = static_cast<B>(scalar) + b.x;
		result.y = static_cast<B>(scalar) + b.y;
		result.z = static_cast<B>(scalar) + b.z;
		return result;
	}

	// Subtract ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector3<A> operator-(const Vector3<A>& a, const Vector3<B>& b)
	{
		Vector3<A> result;
		result.x = a.x - static_cast<A>(b.x);
		result.y = a.y - static_cast<A>(b.y);
		result.z = a.z - static_cast<A>(b.z);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<A> operator-(const Vector3<A>& a, B scalar)
	{
		Vector3<A> result;
		result.x = a.x - static_cast<A>(scalar);
		result.y = a.y - static_cast<A>(scalar);
		result.z = a.z - static_cast<A>(scalar);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<B> operator-(A scalar, const Vector3<B>& b)
	{
		Vector3<B> result;
		result.x = static_cast<B>(scalar) - b.x;
		result.y = static_cast<B>(scalar) - b.y;
		result.z = static_cast<B>(scalar) - b.z;
		return result;
	}

	// Multiply ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector3<A> operator*(const Vector3<A>& a, B scalar)
	{
		Vector3<A> result;
		result.x = a.x * static_cast<A>(scalar);
		result.y = a.y * static_cast<A>(scalar);
		result.z = a.z * static_cast<A>(scalar);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<B> operator*(A scalar, const Vector3<B>& b)
	{
		Vector3<B> result;
		result.x = static_cast<B>(scalar) * b.x;
		result.y = static_cast<B>(scalar) * b.y;
		result.z = static_cast<B>(scalar) * b.z;
		return result;
	}

	// Divide ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector3<A> operator/(const Vector3<A>& a, B scalar)
	{
		Vector3<A> result;
		result.x = a.x / static_cast<A>(scalar);
		result.y = a.y / static_cast<A>(scalar);
		result.z = a.z / static_cast<A>(scalar);
		return result;
	}

	template<typename A, typename B>
	inline Vector3<B> operator/(A scalar, const Vector3<B>& b)
	{
		Vector3<B> result;
		result.x = static_cast<B>(scalar) / b.x;
		result.y = static_cast<B>(scalar) / b.y;
		result.z = static_cast<B>(scalar) / b.z;
		return result;
	}

	// Comparison operators
	template<typename T>
	inline bool operator==(const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template<typename T>
	inline bool operator!=(const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x != b.x || a.y != b.y || a.z != b.z;
	}

	// Stream output operator
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v)
	{
		os << "{x=" << v.x << ",y=" << v.y << ",z=" << v.z << "}";
		return os;
	}

	// Typed definitions ------------------------------------
	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int>;
	// ------------------------------------------------------
}