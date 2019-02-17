#pragma once

#include <ostream>
#include <cmath>
#include <stdexcept>

namespace Ridge {

	// Forward declarations ----------------------------
	template<typename T> class Vector3;
	template<typename T> class Vector4;
	// -------------------------------------------------

	template<typename T = float>
	class Vector2 {
	public:
		// Implicit basic constructors
		Vector2() 
			: Vector2<T>(0, 0) {}

		Vector2(const Vector2<T>& v) 
			= default;

		// Explicit basic constructors
		explicit Vector2(T scalar) 
			: x(scalar)
			, y(scalar) 
		{}

		Vector2(T x, T y) 
			: x(x)
			, y(y) 
		{}

		// Conversion constructors
		template<typename A, typename B>
		Vector2(A x, B y)
			: x(static_cast<T>(x))
			, y(static_cast<T>(y))
		{}

		// Conversion Vector constructors
		template<typename U>
		Vector2(const Vector2<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
		{}

		template<typename U>
		Vector2(const Vector3<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
		{}

		template<typename U>
		Vector2(const Vector4<U>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
		{}
		
		// Default assignment operator
		Vector2<T>& operator=(const Vector2& v) 
			= default;

		// Unary operators
		inline Vector2<T> operator+() const
		{
			return *this;
		}

		inline Vector2<T> operator-() const
		{
			return Vector2<T>(0) - *this;
		}

		// Component access operators
		template<typename U>
		inline T& operator[](int i)
		{
			switch (i) {
			case 0:
				return x;
			case 1:
				return y;
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
			default:
				throw std::runtime_error("index i out of bounds");
			}
		}

		// Vector functions
		inline T Length() const
		{
			return static_cast<T>(std::sqrt(x*x + y*y));
		}

		inline Vector2<T> Normalize() const
		{
			return *this / Length();
		}

		inline T Dot(const Vector2<T>& b) const
		{
			return x * b.x + y * b.y;
		}

	public:
		union {
			struct { T x, y; };
			struct { T r, g; };
		};
	};

	// Arithmetic operators
	// Add -----------------------------------------------------------------------------------
	template<typename A, typename B>
	inline Vector2<A> operator+(const Vector2<A>& a, const Vector2<B>& b) 
	{
		return Vector2<A>(
			a.x + static_cast<A>(b.x), 
			a.y + static_cast<A>(b.y));
	}

	template<typename A, typename B>
	inline Vector2<A> operator+(const Vector2<A>& a, B scalar) 
	{
		return Vector2<A>(
			a.x + static_cast<A>(scalar),
			a.y + static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector2<B> operator+(A scalar, const Vector2<B>& b) 
	{
		return Vector2<B>(
			static_cast<B>(scalar) + b.x,
			static_cast<B>(scalar) + b.y);
	}

	// Subtract ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector2<A> operator-(const Vector2<A>& a, const Vector2<B>& b) 
	{
		return Vector2<A>(
			a.x - static_cast<A>(b.x),
			a.y - static_cast<A>(b.y));
	}

	template<typename A, typename B>
	inline Vector2<A> operator-(const Vector2<A>& a, B scalar) 
	{
		return Vector2<A>(
			a.x - static_cast<A>(scalar),
			a.y - static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector2<B> operator-(A scalar, const Vector2<B>& b) 
	{
		return Vector2<B>(
			static_cast<B>(scalar) - b.x,
			static_cast<B>(scalar) - b.y);
	}

	// Multiply ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector2<A> operator*(const Vector2<A>& a, B scalar) 
	{
		return Vector2<A>(
			a.x * static_cast<A>(scalar),
			a.y * static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector2<B> operator*(A scalar, const Vector2<B>& b) 
	{
		return Vector2<B>(
			static_cast<B>(scalar) * b.x,
			static_cast<B>(scalar) * b.y);
	}

	// Divide ------------------------------------------------------------------------------

	template<typename A, typename B>
	inline Vector2<A> operator/(const Vector2<A>& a, B scalar) 
	{
		return Vector2<A>(
			a.x / static_cast<A>(scalar),
			a.y / static_cast<A>(scalar));
	}

	template<typename A, typename B>
	inline Vector2<B> operator/(A scalar, const Vector2<A>& b) 
	{
		return Vector2<B>(
			static_cast<B>(scalar) / b.x,
			static_cast<B>(scalar) / b.y);
	}

	// Comparison operators
	template<typename T>
	inline bool operator==(const Vector2<T>& a, const Vector2<T>& b) 
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	inline bool operator!=(const Vector2<T>& a, const Vector2<T>& b) 
	{
		return a.x != b.x || a.y != b.y;
	}

	// Stream output operator
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) 
	{
		return os << "{x=" << v.x << ",y=" << v.y << "}";
	}
}