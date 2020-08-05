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
	myMatrix(4, 1) = aPosition.x;
	myMatrix(4, 2) = aPosition.y;
	myMatrix(4, 3) = aPosition.z;
}

void Transform::SetRotation(const Vec3F& aRotation)
{
	// This one doesn't work yet
	auto pos = GetPosition();
	auto scale = GetScale();
	myMatrix = CommonUtilities::Matrix4x4<float>();
	Rotate(aRotation);
	SetPosition(pos);
	SetScale(scale);
}

void Transform::Rotate(const Vec3F& aRotation)
{
	myMatrix = myMatrix * 
		Mat4f::CreateRotationAroundX(aRotation.x) *
		Mat4f::CreateRotationAroundY(aRotation.y) * 
		Mat4f::CreateRotationAroundZ(aRotation.z);
}

void Transform::SetScale(Vec3F aScale)
{
	const Vec3F currScale = GetScale();
	aScale.x /= currScale.x;
	aScale.y /= currScale.y;
	aScale.z /= currScale.z;

	for (int i = 0; i < 4; i++)
		myMatrix(1, i + 1) *= aScale.x;
	for (int i = 0; i < 4; i++)
		myMatrix(2, i + 1) *= aScale.y;
	for (int i = 0; i < 4; i++)
		myMatrix(3, i + 1) *= aScale.z;
}

void Transform::SetMatrix(const Mat4f& aMatrix)
{
	myMatrix = aMatrix;
}

void Transform::SetMatrix(const float* aMatrix)
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			myMatrix(y + 1, x + 1) = aMatrix[y * 4 + x];
		}
	}
}

Vec3F Transform::GetPosition() const
{
	return {
		myMatrix(4, 1),
		myMatrix(4, 2),
		myMatrix(4, 3)
	};
}

Vec3F Transform::GetRotation() const
{
	Vec3F rot;
	GetMatrixRot(rot.x, rot.y, rot.z, myMatrix);
	return rot;
}

Vec3F Transform::GetScale() const
{
	Vec3F scale;
	GetMatrixScale(scale.x, scale.y, scale.z, myMatrix);
	return scale;
}

Mat4f Transform::GetMatrix() const
{
	return myMatrix;
}

void Transform::GetMatrix(float* aMatrix)
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			aMatrix[y * 4 + x] = myMatrix(y + 1, x + 1);
		}
	}
}

physx::PxTransform& Transform::GetPhysXRef()
{
	return myTransform;
}
