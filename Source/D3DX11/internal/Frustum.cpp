#include "Frustum.h"

void Frustum::ConstructFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix)
{
	float zMinimum, r;
	DirectX::XMMATRIX matrix;


	// Calculate the minimum Z distance in the frustum.
	
	
	zMinimum = -projectionMatrix.r[3].m128_f32[2] / projectionMatrix.r[2].m128_f32[2];
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix.r[2].m128_f32[2] = r;
	projectionMatrix.r[3].m128_f32[2] = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.

	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	
	// Calculate near plane of frustum.
	m_planes[0].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[2];
	m_planes[0].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[2];
	m_planes[0].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[2];
	m_planes[0].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[2];
	m_planes[0] = DirectX::XMPlaneNormalize(m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[2];
	m_planes[1].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[2];
	m_planes[1].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[2];
	m_planes[1].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[2];
	m_planes[1] = DirectX::XMPlaneNormalize(m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[0];
	m_planes[2].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[0];
	m_planes[2].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[0];
	m_planes[2].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[0];
	m_planes[2] = DirectX::XMPlaneNormalize(m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[0];
	m_planes[3].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[0];
	m_planes[3].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[0];
	m_planes[3].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[0];
	m_planes[3] = DirectX::XMPlaneNormalize(m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].m128_f32[0] = matrix.r[0].m128_f32[3] - matrix.r[0].m128_f32[1];
	m_planes[4].m128_f32[1] = matrix.r[1].m128_f32[3] - matrix.r[1].m128_f32[1];
	m_planes[4].m128_f32[2] = matrix.r[2].m128_f32[3] - matrix.r[2].m128_f32[1];
	m_planes[4].m128_f32[3] = matrix.r[3].m128_f32[3] - matrix.r[3].m128_f32[1];
	m_planes[4] = DirectX::XMPlaneNormalize(m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].m128_f32[0] = matrix.r[0].m128_f32[3] + matrix.r[0].m128_f32[1];
	m_planes[5].m128_f32[1] = matrix.r[1].m128_f32[3] + matrix.r[1].m128_f32[1];
	m_planes[5].m128_f32[2] = matrix.r[2].m128_f32[3] + matrix.r[2].m128_f32[1];
	m_planes[5].m128_f32[3] = matrix.r[3].m128_f32[3] + matrix.r[3].m128_f32[1];
	m_planes[5] = DirectX::XMPlaneNormalize(m_planes[5]);
}

inline float PlaneDotVec(const DirectX::XMVECTOR& plane, float x, float y, float z, DirectX::XMVECTOR& vec)
{
	vec.m128_f32[0] = x;
	vec.m128_f32[1] = y;
	vec.m128_f32[2] = z;
	vec.m128_f32[3] = 1.0f;
	return DirectX::XMPlaneDotCoord(plane, vec).m128_f32[0];
}

bool Frustum::CheckPoint(float x, float y, float z) const
{
	DirectX::XMVECTOR vec;
	// Check if the point is inside all six planes of the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotVec(m_planes[i], x, y, z, vec) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckCube(float x, float y, float z, float r) const
{
	DirectX::XMVECTOR vec;
	
	// Check if any one point of the cube is in the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotVec(m_planes[i], (x - r), (y - r), (z - r), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (x + r), (y - r), (z - r), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (x - r), (y + r), (z - r), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (x + r), (y + r), (z - r), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (x - r), (y - r), (z + r), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (x + r), (y - r), (z + r), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (x - r), (y + r), (z + r), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (x + r), (y + r), (z + r), vec) >= 0.0f)
			continue;

		return false;
	}
	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius) const
{
	DirectX::XMVECTOR vec;
	// Check if the radius of the sphere is inside the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotVec(m_planes[i], xCenter, yCenter, zCenter, vec) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) const
{
	DirectX::XMVECTOR vec;
	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotVec(m_planes[i], (xCenter - xSize), (yCenter - ySize), (zCenter - zSize), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (xCenter + xSize), (yCenter - ySize), (zCenter - zSize), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (xCenter - xSize), (yCenter + ySize), (zCenter - zSize), vec) >= 0.0f ||
			PlaneDotVec(m_planes[i], (xCenter - xSize), (yCenter - ySize), (zCenter + zSize), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (xCenter + xSize), (yCenter + ySize), (zCenter - zSize), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (xCenter + xSize), (yCenter - ySize), (zCenter + zSize), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (xCenter - xSize), (yCenter + ySize), (zCenter + zSize), vec) >= 0.0f || 
			PlaneDotVec(m_planes[i], (xCenter + xSize), (yCenter + ySize), (zCenter + zSize), vec) >= 0.0f)
			continue;
		return false;
	}
	return true;
}
