#pragma once

#include <cmath>

namespace Ridge {
	// Forward declarations ----------------------------
	template<typename T> class Vector3;
	// -------------------------------------------------

	template<typename T>
	class Quaternion {
	public:
		template<typename U>
		static Quaternion<U> Identity()
		{
			return Quaternion<U>(1, 0, 0, 0);
		}

		template<typename U>
		static Quaternion<U> FromAxisAngle(const Vector3<U>& axis, U angle) 
		{
			U cosHalfAngle = static_cast<U>(std::cos(angle / 2));
			U sinHalfAngle = static_cast<U>(std::sin(angle / 2));

			return Quaternion<U>(
				cosHalfAngle,
				sinHalfAngle * axis.x,
				sinHalfAngle * axis.y,
				sinHalfAngle * axis.z);
		}

		template<typename U>
		static Quaternion<U> FromEuler(const Vector3<U>& euler)
		{
			return FromEuler(euler.x, euler.y, euler.z);
		}

		template<typename U>
		static Quaternion<U> FromEuler(U yaw, U roll, U pitch)
		{
			U cy = static_cast<U>(std::cos(yaw / 2));
			U sy = static_cast<U>(std::sin(yaw / 2));
			U cp = static_cast<U>(std::cos(pitch / 2));
			U sp = static_cast<U>(std::sin(pitch / 2));
			U cr = static_cast<U>(std::cos(roll / 2));
			U sr = static_cast<U>(std::sin(roll / 2));

			U w = cy * cp * cr + sy * sp * sr;
			U x = cy * cp * sr - sy * sp * cr;
			U y = sy * cp * sr + cy * sp * cr;
			U z = sy * cp * cr - cy * sp * sr;

			return Quaternion<U>(w, x, y, z);
		}

	public:
		// Implicit basic constructors
		Quaternion()
			: Quaternion<T>(0, 0, 0, 0) {}

		Quaternion(const Quaternion<T>& v)
			= default;

		// Explicit basic constructors
		Quaternion(T w, T x, T y, T z)
			: w(w)
			, x(x)
			, y(y)
			, z(z)
		{}

		// Conversion constructor
		template<typename A, typename B, typename C, typename D>
		Quaternion(A w, A x, A y, A z)
			: w(static_cast<T>(w))
			, x(static_cast<T>(x))
			, y(static_cast<T>(y))
			, z(static_cast<T>(z))
		{}

		Quaternion(const Vector3<T>& vec)
			: w(0)
			, x(vec.x)
			, y(vec.y)
			, z(vec.z)
		{}

		// Default assignment operator
		Quaternion<T>& operator=(const Quaternion<T>& v) = default;

		// Unary operators
		inline Quaternion<T> operator+() const
		{
			return *this;
		}

		inline Quaternion<T> operator-() const
		{
			return Quaternion(0, 0, 0, 0) - *this;
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
			case 3:
				return w;
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
			case 3:
				return w;
			default:
				throw std::runtime_error("index i out of bounds");
			}
		}

		inline T Length() const
		{
			return static_cast<T>(std::sqrt(w*w + x*x + y*y + z*z));
		}

		inline Quaternion<T> Normalize() 
		{
			return *this / Length();
		}

		inline Quaternion<T> Conjugate()
		{
			return Quaternion<T>(w, -x, -y, -z);
		}

	public:
		struct { T w, x, y, z; };
	};

	template<typename T>
	inline Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b)
	{
		Quaternion<T> result;
		result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
		result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		return result;
	}
}