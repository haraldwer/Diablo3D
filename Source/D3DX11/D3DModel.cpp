#include "D3DModel.h"
#include "D3DSystem.h"

D3DModel::D3DModel(const char* aModelPath, const char* aTexturePath): myPosition(), myRotation(), myScale(), myModel(nullptr)
{
	myModel = D3DSystem::CreateModel(aModelPath);
	myTexture = D3DSystem::CreateTexture(aTexturePath);
	SetScale(1, 1, 1);
}

void D3DModel::SetPosition(float x, float y, float z)
{
	myPosition = DirectX::XMFLOAT3(x, y, z);
}

void D3DModel::SetRotation(float x, float y, float z)
{
	myRotation = DirectX::XMFLOAT3(x, y, z);
}

void D3DModel::SetScale(float x, float y, float z)
{
	myScale = DirectX::XMFLOAT3(x, y, z);
}

void D3DModel::Render()
{
	D3DSystem::RenderModel(this);
}
