#pragma once

#include <ostream>
#include <cmath>
#include <stdexcept>

namespace Ridge {

	// Forward declarations ----------------------------
	template<typename T> class Vector2;
	template<typename T> class Vector3;
	// -------------------------------------------------

	template<typename T>
	class Vector4 {
	public:
		// Implicit basic constructors
		Vector4()
			: Vector4<T>(0, 0, 0, 0) {}

		Vector4(const Vector4<T>& v) 
			= default;

		// Explicit basic constructors
		explicit Vector4(T scalar)
			: x(scalar)
			, y(scalar)
			, z(scalar)
			, w(scalar)
		{}

		Vector4(T x, T y, T z, T w)
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{}
		
		// Conversion constructors
		template<typename A, typename B, typename C, typename D>
		Vector4(A x, B y, C z, D w)
			: x(static_cast<T>(x))
			, y(static_cast<T>(y))
			, z(static_cast<T>(z))
			, w(static_cast<T>(w))
		{}

		// Conversion Vector constructors
		template<typename U>
		Vector4(const Vector2<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(0))
			, w(static_cast<T>(0))
		{}

		template<typename U>
		Vector4(const Vector3<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z))
			, w(static_cast<T>(0))
		{}

		template<typename U>
		Vector4(const Vector4<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z))
			, w(static_cast<T>(v.w))
		{}

		// Default assignment operator
		Vector4<T>& operator=(const Vector4& v) = default;

		// Unary operators
		inline Vector4<T> operator+() const 
		{
			return *this;
		}

		inline Vector4<T> operator-() const
		{
			return Vector4<T>(0) - *this;
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
			return static_cast<T>(std::sqrt(x*x + y*y + z*z + w*w));
		}

		inline Vector4<T> Normalize() const
		{
			return *this / Length();
		}

		inline T Dot(const Vector4<T>& b) const
		{
			return x * b.x + y * b.y + z * b.z + w * b.w;
		}

	public:
		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};
	};

	// Arithmetic operators
	
	// Add -----------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector4<A> operator+(const Vector4<A>& a, const Vector4<B>& b)
	{
		return Vector4<A>(
			a.x + static_cast<A>(b.x),
			a.y + static_cast<A>(b.y),
			a.z + static_cast<A>(b.z),
			a.w + static_cast<A>(b.w));
	}

	template<typename A, typename B>
	inline Vector4<A> operator+(const Vector4<A>& a, B scalar)
	{
		return Vector4<A>(
			a.x + static_cast<A>(scalar),
			a.y + static_cast<A>(scalar),
			a.z + static_cast<A>(scalar),
			a.w + static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector4<B> operator+(A scalar, const Vector4<B>& b)
	{
		return Vector4<B>(
			static_cast<B>(scalar) + b.x,
			static_cast<B>(scalar) + b.y,
			static_cast<B>(scalar) + b.z,
			static_cast<B>(scalar) + b.w);
	}

	// Subtract ------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector4<A> operator-(const Vector4<A>& a, const Vector4<B>& b)
	{
		return Vector4<A>(
			a.x - static_cast<A>(b.x),
			a.y - static_cast<A>(b.y),
			a.z - static_cast<A>(b.z),
			a.w - static_cast<A>(b.w));
	}

	template<typename A, typename B>
	inline Vector4<A> operator-(const Vector4<A>& a, B scalar)
	{
		return Vector4<A>(
			a.x - static_cast<A>(scalar),
			a.y - static_cast<A>(scalar),
			a.z - static_cast<A>(scalar),
			a.w - static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector4<B> operator-(A scalar, const Vector4<B>& b)
	{
		return Vector4<B>(
			static_cast<B>(scalar) - b.x,
			static_cast<B>(scalar) - b.y,
			static_cast<B>(scalar) - b.z,
			static_cast<B>(scalar) - b.w);
	}

	// Multiply ------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector4<A> operator*(const Vector4<A>& a, B scalar)
	{
		return Vector4<A>(
			a.x * static_cast<A>(scalar),
			a.y * static_cast<A>(scalar),
			a.z * static_cast<A>(scalar),
			a.w * static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector4<B> operator*(A scalar, const Vector4<B>& b)
	{
		return Vector4<B>(
			static_cast<B>(scalar) * b.x,
			static_cast<B>(scalar) * b.y,
			static_cast<B>(scalar) * b.z,
			static_cast<B>(scalar) * b.w);
	}

	// Divide ------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector4<A> operator/(const Vector4<A>& a, B scalar)
	{
		return Vector4<A>(
			a.x / static_cast<A>(scalar),
			a.y / static_cast<A>(scalar),
			a.z / static_cast<A>(scalar),
			a.w / static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector4<B> operator/(A scalar, const Vector4<B>& b)
	{
		return Vector4<B>(
			static_cast<B>(scalar) / b.x,
			static_cast<B>(scalar) / b.y,
			static_cast<B>(scalar) / b.z,
			static_cast<B>(scalar) / b.w);
	}

	// Comparison operators
	template<typename T>
	inline bool operator==(const Vector4<T>& a, const Vector4<T>& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	template<typename T>
	inline bool operator!=(const Vector4<T>& a, const Vector4<T>& b)
	{
		return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
	}

	// Stream output operator
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector4<T>& v)
	{
		os << "{x=" << v.x << ",y=" << v.y << ",z=" << v.z << ",w=" << v.w << "}";
		return os;
	}

	// Typed definitions ------------------------------------
	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	// ------------------------------------------------------
}