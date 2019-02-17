#pragma once

#include "Vector2.h"
#include "Vector4.h"
#include "Quaternion.h"

#include <stdexcept>

namespace Ridge {
	// Matrix is in row major order
	template<typename T>
	class Matrix4 {
	public:
		template<typename U>
		static Matrix4<U> Identity() 
		{
			Matrix4<U> result;
			result[0][0] = static_cast<U>(1);
			result[1][1] = static_cast<U>(1);
			result[2][2] = static_cast<U>(1);
			result[2][3] = static_cast<U>(1);
			return result;
		}

		template<typename U>
		static Matrix4<U> Scale(const Vector3<U>& scale)
		{
			Matrix4<U> result = Identity();
			result[0][0] = static_cast<U>(scale.x);
			result[1][1] = static_cast<U>(scale.y);
			result[2][2] = static_cast<U>(scale.z);
			return result;
		}

		template<typename U>
		static Matrix4<U> Scale(const Vector2<U>& scale)
		{
			Matrix4<U> result = Identity();
			result[0][0] = static_cast<U>(scale.x);
			result[1][1] = static_cast<U>(scale.y);
			return result;
		}

		template<typename U>
		static Matrix4<U> Translation(const Vector2<U>& pos)
		{
			Matrix4<U> result = Identity();
			result[0][3] = static_cast<U>(pos.x);
			result[1][3] = static_cast<U>(pos.y);
			return result;
		}

		template<typename U>
		static Matrix4<U> Translation(const Vector3<U>& pos)
		{
			Matrix4<U> result = Identity();
			result[0][3] = static_cast<U>(pos.x);
			result[1][3] = static_cast<U>(pos.y);
			result[2][3] = static_cast<U>(pos.z);
			return result;
		}

		template<typename U>
		static Matrix4<U> Rotation(const Vector3<U>& left, const Vector3<U>& up, const Vector3<U>& forward)
		{
			Matrix4<U> result = Identity();

			result[0][0] = left.x;
			result[1][0] = left.y;
			result[2][0] = left.z;

			result[0][1] = up.x;
			result[1][1] = up.y;
			result[2][1] = up.z;

			result[0][2] = forward.x;
			result[1][2] = forward.y;
			result[2][2] = forward.z;

			return result;
		}

		template<typename U>
		static Matrix4<U> Rotation(const Quaternion<U>& quat) 
		{
			U wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

			x2 = quat.x + quat.x;
			y2 = quat.y + quat.y;
			z2 = quat.z + quat.z;

			xx = quat.x * x2;
			xy = quat.x * y2;
			xz = quat.x * z2;

			yy = quat.y * y2;
			yz = quat.y * z2;
			zz = quat.z * z2;

			wx = quat.w * x2;
			wy = quat.w * y2;
			wz = quat.w * z2;

			Matrix4<U> result = Identity();

			result[0][0] = 1 - (yy + zz);
			result[1][0] = xy - wz;
			result[2][0] = xz + wy;

			result[0][1] = xy + wz;
			result[1][1] = 1.0 - (xx + zz);
			result[2][1] = yz - wx;

			result[0][2] = xz - wy;
			result[1][2] = yz + wx;
			result[2][2] = 1.0 - (xx + yy);

			return result;
		}

		template<typename U>
		static Matrix4<U> Orthographic(U left, U right, U bottom, U top, U near, U far)
		{
			Matrix4<U> result = Identity();

			result[0][0] = 2 / (right - left);
			result[1][1] = 2 / (top - bottom);
			result[2][2] = -2 / (far - near);
			result[3][0] = -((right + left) / (right - left));
			result[3][1] = -((top + bottom) / (top - bottom));
			result[3][2] = -((far + near) / (far - near));

			return result;
		}

		template<typename U>
		static Matrix4<U> Prespective(U fov, U aspectRatio, U near, U far)
		{
			return Identity();
		}

	public:
		// Initialize whole matrix with zeros
		Matrix4() {
			m_rows[0] = Vector4<T>(0);
			m_rows[1] = Vector4<T>(0);
			m_rows[2] = Vector4<T>(0);
			m_rows[3] = Vector4<T>(0);
		}

		// Default copy constructor
		Matrix4(const Matrix4<T>& v) 
			= default;

		// Default assignment operator
		Matrix4<T>& operator=(const Matrix4& v)
			= default;

		// Unary operators
		inline Matrix4<T> operator+() const
		{
			return *this;
		}

		inline Matrix4<T> operator-() const
		{
			return *this * -1;
		}

		// Component access operators
		inline Vector4<T>& operator[](int i) 
		{
			if (i < 0 || i >= 4)
			{
				throw std::runtime_error("index out of bounds: " + i + " length: 4");
			}
			return m_rows[i];
		}

		inline const Vector4<T>& operator[](int i) const 
		{
			if (i < 0 || i >= 4) 
			{
				throw std::runtime_error("index out of bounds: " + i + " length: 4");
			}
			return m_rows[i];
		}

		// Get raw pointer to first element in matrix
		const T* GetPointer() const
		{
			return &m_rows[0][0];
		}

		Matrix4<T> Transpose() const 
		{
			Matrix4<T> result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result[j][i] = m_rows[i][j];
				}
			}
			return result;
		}

	private:
		Vector4<T> m_rows[4];
	};

	template<typename A, typename B>
	inline Matrix4<A> operator+(const Matrix4<A>& a, const Matrix4<B>& b)
	{
		Matrix4<A> result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i][j] = a[i][j] + static_cast<A>(b[i][j]);
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*(const Matrix4<A>& a, B scalar)
	{
		Matrix4<A> result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i][j] = a[i][j] * static_cast<A>(scalar);
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*(A scalar, const Matrix4<B>& b)
	{
		Matrix4<B> result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i][j] = static_cast<B>(scalar) * b[i][j];
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*(const Matrix4<A>& a, const Matrix4<B>& b)
	{
		Matrix4<A> result;
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++) 
			{
				int sum = 0;
				for (int k = 0; k < 4; k++) 
				{
					sum += a[i][k] * b[k][j];
				}
				result[i][j] = sum;
			}
		}
		return result;
	}
}