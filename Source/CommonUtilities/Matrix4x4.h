#pragma once
#include "Vector4.h"
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		void operator=(const Matrix4x4<T>& aMatrix);
		bool operator==(const Matrix4x4<T>& aMatrix) const;

		const Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix);
		void operator+=(const Matrix4x4<T>& aMatrix);

		const Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix);
		void operator-=(const Matrix4x4<T>& aMatrix);

		Matrix4x4<T>operator*(const Matrix4x4<T>& aMatrix);
		void operator*=(const Matrix4x4<T>& aMatrix);

		Matrix4x4<T>operator*(const T aScalar);
		void operator*=(const T aScalar);

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
	private:
		T myMatrix[4][4];
	};

	template <class T>
	Matrix4x4<T>::Matrix4x4()
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				myMatrix[row][column] = row == column;
			}
		}
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		(*this) = aMatrix;
	}

	template <class T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		// ASSERT IF OUTSIDE OF BOUNDS
		assert(aRow > 0 && aRow <= 4 && aColumn > 0 && aColumn <= 4);
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <class T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <class T>
	void Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				myMatrix[row][column] = aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aMatrix) const
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				if (myMatrix[row][column] != aMatrix.myMatrix[row][column])
				{
					return false;
				}
			}
		}
		return true;
	}

	template <class T>
	const Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result = (*this);
		result += aMatrix;
		return result;
	}

	template <class T>
	void Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				myMatrix[row][column] += aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	const Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result = (*this);
		result -= aMatrix;
		return result;
	}

	template <class T>
	void Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				myMatrix[row][column] -= aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result;
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				result.myMatrix[row][column] = 0;
				for (unsigned int size = 0; size < 4; size++)
				{
					result.myMatrix[row][column] += myMatrix[row][size] * aMatrix.myMatrix[size][column];
				}
			}
		}
		return result;
	}

	template <class T>
	void Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		(*this) = (*this) * aMatrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const T aScalar)
	{
		Matrix4x4<T> result = (*this);
		result *= aScalar;
		return result;
	}

	template <class T>
	void Matrix4x4<T>::operator*=(const T aScalar)
	{
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				myMatrix[row][column] *= aScalar;
			}
		}
	}

	template<class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> result;
		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		result.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);
		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> result;
		result.myMatrix[1][1] = cos(aAngleInRadians);
		result.myMatrix[1][2] = sin(aAngleInRadians);
		result.myMatrix[2][1] = -result.myMatrix[1][2];
		result.myMatrix[2][2] = result.myMatrix[1][1];
		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> result;
		result.myMatrix[0][0] = cos(aAngleInRadians);
		result.myMatrix[0][2] = -sin(aAngleInRadians);
		result.myMatrix[2][0] = -result.myMatrix[0][2];
		result.myMatrix[2][2] = result.myMatrix[0][0];
		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> result;
		result.myMatrix[0][0] = cos(aAngleInRadians);
		result.myMatrix[0][1] = sin(aAngleInRadians);
		result.myMatrix[1][0] = -result.myMatrix[0][1];
		result.myMatrix[1][1] = result.myMatrix[0][0];
		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> base;
		for (unsigned int row = 0; row < 4; row++)
		{
			for (unsigned int column = 0; column < 4; column++)
			{
				base.myMatrix[column][row] = aMatrixToTranspose.myMatrix[row][column];
			}
		}
		return base;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> rot;
		for (unsigned int row = 1; row < 4; row++)
		{
			for (unsigned int column = 1; column < 4; column++)
			{
				rot(row, column) = aTransform(column, row);
			}
		}

		Matrix4x4<T> pos;
		pos(4, 1) = -aTransform(4, 1);
		pos(4, 2) = -aTransform(4, 2);
		pos(4, 3) = -aTransform(4, 3);
		pos(4, 4) = aTransform(4, 4);
		return pos * rot;
	}
}
