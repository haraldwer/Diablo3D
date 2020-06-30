#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

namespace CommonUtilities
{
	template <class T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();
		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		void operator=(const Matrix3x3<T>& aMatrix);
		bool operator==(const Matrix3x3<T>& aMatrix) const;

		const Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix);
		void operator+=(const Matrix3x3<T>& aMatrix);

		const Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix);
		void operator-=(const Matrix3x3<T>& aMatrix);

		const Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix);
		void operator*=(const Matrix3x3<T>& aMatrix);

		const Matrix3x3<T> operator*(const T aScalar);
		void operator*=(const T aScalar);

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);
		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix3x3<T> GetFastInverse(const Matrix3x3<T>& aTransform);
	private:
		T myMatrix[3][3];
	};


	template <class T>
	Matrix3x3<T>::Matrix3x3()
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] = row == column;
			}
		}
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		(*this) = aMatrix;
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] = aMatrix(row + 1,column + 1);
			}
		}
	}

	template <class T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		// ASSERT IF OUTSIDE OF BOUNDS
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <class T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow - 1][aColumn - 1];
	}

	template <class T>
	void Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] = aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aMatrix) const
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
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
	const Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result = (*this);
		result += aMatrix;
		return result;
	}

	template <class T>
	void Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] += aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	const Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result = (*this);
		result -= aMatrix;
		return result;
	}

	template <class T>
	void Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] -= aMatrix.myMatrix[row][column];
			}
		}
	}

	template <class T>
	const Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result;
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				result.myMatrix[row][column] = 0;
				for (unsigned int size = 0; size < 3; size++)
				{
					result.myMatrix[row][column] += myMatrix[row][size] * aMatrix.myMatrix[size][column];
				}
			}
		}
		return result;
	}

	template <class T>
	void Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		(*this) = (*this) * aMatrix;
	}

	template <class T>
	const Matrix3x3<T> Matrix3x3<T>::operator*(const T aScalar)
	{
		Matrix3x3<T> result = (*this);
		result *= aScalar;
		return result;
	}

	template <class T>
	void Matrix3x3<T>::operator*=(const T aScalar)
	{
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				myMatrix[row][column] *= aScalar;
			}
		}
	}

	template<class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> result;
		result.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		result.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		result.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);
		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> result;
		result.myMatrix[1][1] = cos(aAngleInRadians);
		result.myMatrix[1][2] = sin(aAngleInRadians);
		result.myMatrix[2][1] = -result.myMatrix[1][2];
		result.myMatrix[2][2] = result.myMatrix[1][1];
		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> result;
		result.myMatrix[0][0] = cos(aAngleInRadians);
		result.myMatrix[0][2] = -sin(aAngleInRadians);
		result.myMatrix[2][0] = -result.myMatrix[0][2];
		result.myMatrix[2][2] = result.myMatrix[0][0];
		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> result;
		result.myMatrix[0][0] = cos(aAngleInRadians);
		result.myMatrix[0][1] = sin(aAngleInRadians);
		result.myMatrix[1][0] = -result.myMatrix[0][1];
		result.myMatrix[1][1] = result.myMatrix[0][0];
		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> base;
		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int column = 0; column < 3; column++)
			{
				base.myMatrix[column][row] = aMatrixToTranspose.myMatrix[row][column];
			}
		}
		return base;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::GetFastInverse(const Matrix3x3<T>& aTransform)
	{
		Matrix3x3<T> rot;
		for (unsigned int row = 1; row < 3; row++)
		{
			for (unsigned int column = 1; column < 3; column++)
			{
				rot(row, column) = aTransform(column, row);
			}
		}

		Matrix3x3<T> pos;
		pos(3, 1) = -aTransform(3, 1);
		pos(3, 2) = -aTransform(3, 2);
		pos(3, 3) = aTransform(3, 3);
		return pos * rot;
	}
}
