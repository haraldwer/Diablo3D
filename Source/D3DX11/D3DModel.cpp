#include "D3DModel.h"
#include "D3DSystem.h"

D3DModel::D3DModel(const char* aModelPath, const char* aTexturePath): myMatrix(), myModel(nullptr)
{
	myModel = D3DSystem::CreateModel(aModelPath);
	myTexture = D3DSystem::CreateTexture(aTexturePath);
}

void D3DModel::SetMatrix(float* aMatrix)
{
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 4; x++)
		{
			myMatrix.r[y].m128_f32[x] = aMatrix[y * 4 + x];
		}
	}
}

void D3DModel::Render()
{
	D3DSystem::RenderModel(this);
}
