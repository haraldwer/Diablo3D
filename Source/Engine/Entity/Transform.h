#pragma once
#include "../CommonUtilities/Vector3.h"
#include "../CommonUtilities/Matrix4x4.h"
#include <PhysX/foundation/PxTransform.h>

typedef CommonUtilities::Vector3<float> Vec3F;
typedef CommonUtilities::Matrix4x4<float> Mat4f;

class Transform
{
	friend class Rigidbody;
	friend class Collider;
	
public:
	// World position
	void SetPosition(const Vec3F& aPosition);
	// Rotation in Euler-angles
	void SetRotation(const Vec3F& aRotation);
	void Rotate(const Vec3F& aRotation);
	void SetScale(Vec3F aScale);
	void SetMatrix(const Mat4f& aMatrix);
	// Input param needs to be float[16], representing a 4x4 matrix.
	void SetMatrix(const float* aMatrix);
	
	Vec3F GetPosition() const;
	// Rotation in Euler-angles
	Vec3F GetRotation() const;
	Vec3F GetScale() const;
	Mat4f GetMatrix() const;
	// Input param needs to be float[16], representing a 4x4 matrix. 
	void GetMatrix(float* aMatrix);
	physx::PxTransform& GetPhysXRef();

private:
	CommonUtilities::Matrix4x4<float> myMatrix;
	physx::PxTransform myTransform;
};

