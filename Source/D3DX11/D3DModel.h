#pragma once
#include <DirectXMath.h>

class Model;
class Texture;

class D3DModel
{
	friend class Graphics;
public:
	D3DModel() = delete;
	D3DModel(const char* aModelPath, const char* aTexturePath);
	void SetMatrix(float* aMatrix);
	
	// Add to render queue in D3DX11
	void Render();
	
private:
	DirectX::XMMATRIX myMatrix;
	
	Model* myModel;
	Texture* myTexture;
};

