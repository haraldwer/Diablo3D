#include "Transform.h"

inline void GetMatrixRot(float& x, float& y, float& z, const Mat4f& object)
{
	x = atan2(-object(2, 3), object(3, 3));
	const float cosYangle = sqrt(pow(object(1, 1), 2) + pow(object(1, 2), 2));
	y = atan2(object(1, 3), cosYangle);
	const float sinXangle = sin(x);
	const float cosXangle = cos(x);
	z = atan2(cosXangle * object(2, 1) + sinXangle * object(3, 1), cosXangle * object(2, 2) + sinXangle * object(3, 2));
}

inline float Pow2(const float x)
{
	return x * x;
}

inline float Length(const float x, const float y, const float z)
{
	return sqrtf(Pow2(x) + Pow2(y) + Pow2(z));
}

inline void GetMatrixScale(float& x, float& y, float& z, const Mat4f& object)
{
	x = Length(object(1, 1), object(1, 2), object(1, 3));
	y = Length(object(2, 1), object(2, 2), object(2, 3));
	z = Length(object(3, 1), object(3, 2), object(3, 3));
}

void Transform::SetPosition(const Vec3F& aPosition)
{
	myTransform.p = { aPosition.x, aPosition.y, aPosition.z };
}

void Transform::SetRotation(const Vec3F& aRotation)
{
	myTransform.q.x = aRotation.x;
	myTransform.q.y = aRotation.y;
	myTransform.q.z = aRotation.z;
}

void Transform::Rotate(const Vec3F& aRotation)
{
	myTransform.rotate({ 
		aRotation.x,
		aRotation.y,
		aRotation.z
	});
}

void Transform::SetScale(Vec3F aScale)
{
	// Doesn't currently work
}

void Transform::SetMatrix(const Mat4f& aMatrix)
{
	physx::PxMat44 mat;
	mat.column0 = {
		aMatrix(1, 1),
		aMatrix(2, 1),
		aMatrix(3, 1),
		aMatrix(4, 1)
	};
	mat.column1 = {
		aMatrix(1, 2),
		aMatrix(2, 2),
		aMatrix(3, 2),
		aMatrix(4, 2)
	};
	mat.column2 = {
		aMatrix(1, 3),
		aMatrix(2, 3),
		aMatrix(3, 3),
		aMatrix(4, 3)
	};
	mat.column3 = {
		aMatrix(1, 4),
		aMatrix(2, 4),
		aMatrix(3, 4),
		aMatrix(4, 4)
	};
	myTransform = physx::PxTransform(mat);
}

void Transform::SetMatrix(const float* aMatrix)
{
	physx::PxMat44 mat;
	mat.column0 = {
		aMatrix[0 * 4 + 0],
		aMatrix[1 * 4 + 0],
		aMatrix[2 * 4 + 0],
		aMatrix[3 * 4 + 0]
	};
	mat.column1 = {
		aMatrix[0 * 4 + 1],
		aMatrix[1 * 4 + 1],
		aMatrix[2 * 4 + 1],
		aMatrix[3 * 4 + 1]
	};
	mat.column2 = {
		aMatrix[0 * 4 + 2],
		aMatrix[1 * 4 + 2],
		aMatrix[2 * 4 + 2],
		aMatrix[3 * 4 + 2]
	};
	mat.column3 = {
		aMatrix[0 * 4 + 3],
		aMatrix[1 * 4 + 3],
		aMatrix[2 * 4 + 3],
		aMatrix[3 * 4 + 3]
	};
	myTransform = physx::PxTransform(mat);
}

Vec3F Transform::GetPosition() const
{
	return {
		myTransform.q.x,
		myTransform.q.y,
		myTransform.q.z
	};
}

Vec3F Transform::GetRotation() const
{
	return {
		myTransform.p.x,
		myTransform.p.y,
		myTransform.p.z,
	};
}

Vec3F Transform::GetScale() const
{
	Vec3F scale;
	//GetMatrixScale(scale.x, scale.y, scale.z, myMatrix);
	return scale;
}

Mat4f Transform::GetMatrix() const
{
	Mat4f mat;
	//mat(0, 0);
	return mat;
}

void Transform::GetMatrix(float* aMatrix)
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			//aMatrix[y * 4 + x] = myMatrix(y + 1, x + 1);
		}
	}
}

physx::PxTransform& Transform::GetPhysXRef()
{
	return myTransform;
}
