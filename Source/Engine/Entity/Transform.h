#pragma once
#include "../CommonUtilities/Vector3.h"

typedef CommonUtilities::Vector3<float> Vec3F;

class Transform
{
	friend class Rigidbody;
	friend class Collider;
	
public:
	Transform();
	void SetPosition(const Vec3F aPosition) { myPosition = aPosition; }
	void SetRotation(const Vec3F aRotation) { myRotation = aRotation; }
	void SetScale(const Vec3F aScale) { myScale = aScale; }

	Vec3F GetPosition() const { return myPosition; }
	Vec3F GetRotation() const { return myRotation; }
	Vec3F GetScale() const { return myScale; }
	
private:
	Vec3F myPosition;
	Vec3F myRotation;
	Vec3F myScale;
};

