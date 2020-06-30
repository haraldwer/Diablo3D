#pragma once

#include <DirectXMath.h>

class Frustum
{
public:
	void ConstructFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix);
	bool CheckPoint(float, float, float) const;
	bool CheckCube(float, float, float, float) const;
	bool CheckSphere(float, float, float, float) const;
	bool CheckRectangle(float, float, float, float, float, float) const;
private:
	DirectX::XMVECTOR m_planes[6];
};

