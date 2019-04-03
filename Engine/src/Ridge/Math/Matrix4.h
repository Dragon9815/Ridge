#pragma once

#include "Vector2.h"
#include "Vector4.h"
#include "Quaternion.h"

#include <stdexcept>
#include <sstream>

namespace Ridge {
	// Matrix is in row major order
	template<typename T>
	class Matrix4
	{
	public:

		static Matrix4<T> Identity( )
		{
			Matrix4<T> result;
			result[ 0 ][ 0 ] = static_cast<T>( 1 );
			result[ 1 ][ 1 ] = static_cast<T>( 1 );
			result[ 2 ][ 2 ] = static_cast<T>( 1 );
			result[ 3 ][ 3 ] = static_cast<T>( 1 );
			return result;
		}

		static Matrix4<T> Scale( const Vector3<T> & scale )
		{
			Matrix4<T> result = Identity( );
			result[ 0 ][ 0 ] = static_cast<T>( scale.x );
			result[ 1 ][ 1 ] = static_cast<T>( scale.y );
			result[ 2 ][ 2 ] = static_cast<T>( scale.z );
			return result;
		}

		static Matrix4<T> Scale( const Vector2<T> & scale )
		{
			Matrix4<T> result = Identity( );
			result[ 0 ][ 0 ] = static_cast<T>( scale.x );
			result[ 1 ][ 1 ] = static_cast<T>( scale.y );
			return result;
		}

		static Matrix4<T> Translation( const Vector2<T> & pos )
		{
			Matrix4<T> result = Identity( );
			result[ 0 ][ 3 ] = static_cast<T>( pos.x );
			result[ 1 ][ 3 ] = static_cast<T>( pos.y );
			return result;
		}

		static Matrix4<T> Translation( const Vector3<T> & pos )
		{
			Matrix4<T> result = Identity( );
			result[ 0 ][ 3 ] = static_cast<T>( pos.x );
			result[ 1 ][ 3 ] = static_cast<T>( pos.y );
			result[ 2 ][ 3 ] = static_cast<T>( pos.z );
			return result;
		}

		static Matrix4<T> Rotation( const Vector3<T> & left, const Vector3<T> & up, const Vector3<T> & forward )
		{
			Matrix4<T> result = Identity( );

			result[ 0 ][ 0 ] = left.x;
			result[ 1 ][ 0 ] = left.y;
			result[ 2 ][ 0 ] = left.z;

			result[ 0 ][ 1 ] = up.x;
			result[ 1 ][ 1 ] = up.y;
			result[ 2 ][ 1 ] = up.z;

			result[ 0 ][ 2 ] = forward.x;
			result[ 1 ][ 2 ] = forward.y;
			result[ 2 ][ 2 ] = forward.z;

			return result;
		}

		static Matrix4<T> Rotation( const Quaternion<T> & quat )
		{
			T wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

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

			Matrix4<T> result = Identity( );

			result[ 0 ][ 0 ] = 1 - ( yy + zz );
			result[ 1 ][ 0 ] = xy - wz;
			result[ 2 ][ 0 ] = xz + wy;

			result[ 0 ][ 1 ] = xy + wz;
			result[ 1 ][ 1 ] = 1.0 - ( xx + zz );
			result[ 2 ][ 1 ] = yz - wx;

			result[ 0 ][ 2 ] = xz - wy;
			result[ 1 ][ 2 ] = yz + wx;
			result[ 2 ][ 2 ] = 1.0 - ( xx + yy );

			return result;
		}

		template<typename A, typename B, typename C, typename D, typename E, typename F>
		static Matrix4<T> Orthographic( A left, B right, C bottom, D top, E nearZ, F farZ )
		{
			Matrix4<T> result = Identity( );

			T leftT = static_cast<T>( left );
			T rightT = static_cast<T>( right );
			T topT = static_cast<T>( top );
			T bottomT = static_cast<T>( bottom );
			T nearT = static_cast<T>( nearZ );
			T farT = static_cast<T>( farZ );

			result[ 0 ][ 0 ] = 2 / ( rightT - leftT );
			result[ 1 ][ 1 ] = 2 / ( topT - bottomT );
			result[ 2 ][ 2 ] = 2 / ( farT - nearT );
			result[ 0 ][ 3 ] = -( ( rightT + leftT ) / ( rightT - leftT ) );
			result[ 1 ][ 3 ] = -( ( topT + bottomT ) / ( topT - bottomT ) );
			result[ 2 ][ 3 ] = -( ( farT + nearT ) / ( farT - nearT ) );

			return result;
		}

		static Matrix4<T> Prespective( T fov, T aspectRatio, T nearZ, T farZ )
		{
			return Identity( );
		}

	public:
		// Initialize whole matrix with zeros
		Matrix4( )
		{
			m_rows[ 0 ] = Vector4<T>( 0 );
			m_rows[ 1 ] = Vector4<T>( 0 );
			m_rows[ 2 ] = Vector4<T>( 0 );
			m_rows[ 3 ] = Vector4<T>( 0 );
		}

		// Default copy constructor
		Matrix4( const Matrix4<T> & v )
			= default;

		// Default assignment operator
		Matrix4<T> & operator=( const Matrix4 & v )
			= default;

		// Unary operators
		inline Matrix4<T> operator+( ) const
		{
			return *this;
		}

		inline Matrix4<T> operator-( ) const
		{
			return *this * -1;
		}

		// Component access operators
		inline Vector4<T> & operator[]( int i )
		{
			if( i < 0 || i >= 4 )
			{
				std::stringstream ss;
				ss << "index out of bounds: " << i << " length: 4";
				throw std::runtime_error( ss.str( ) );
			}
			return m_rows[ i ];
		}

		inline const Vector4<T> & operator[]( int i ) const
		{
			if( i < 0 || i >= 4 )
			{
				std::stringstream ss;
				ss << "index out of bounds: " << i << " length: 4";
				throw std::runtime_error( ss.str( ) );
			}
			return m_rows[ i ];
		}

		// Get raw pointer to first element in matrix
		const T * GetPointer( ) const
		{
			return &m_rows[ 0 ][ 0 ];
		}

		Matrix4<T> Transpose( ) const
		{
			Matrix4<T> result;
			for( int i = 0; i < 4; i++ )
			{
				for( int j = 0; j < 4; j++ )
				{
					result[ j ][ i ] = m_rows[ i ][ j ];
				}
			}
			return result;
		}

	private:
		Vector4<T> m_rows[ 4 ];
	};

	template<typename A, typename B>
	inline Matrix4<A> operator+( const Matrix4<A> & a, const Matrix4<B> & b )
	{
		Matrix4<A> result;
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				result[ i ][ j ] = a[ i ][ j ] + static_cast<A>( b[ i ][ j ] );
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*( const Matrix4<A> & a, B scalar )
	{
		Matrix4<A> result;
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				result[ i ][ j ] = a[ i ][ j ] * static_cast<A>( scalar );
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*( A scalar, const Matrix4<B> & b )
	{
		Matrix4<B> result;
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				result[ i ][ j ] = static_cast<B>( scalar ) * b[ i ][ j ];
			}
		}
		return result;
	}

	template<typename A, typename B>
	inline Matrix4<A> operator*( const Matrix4<A> & a, const Matrix4<B> & b )
	{
		Matrix4<A> result;
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				A sum = static_cast<A>(0);
				for( int k = 0; k < 4; k++ )
				{
					sum += a[ i ][ k ] * b[ k ][ j ];
				}
				result[ i ][ j ] = sum;
			}
		}
		return result;
	}
}