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
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	// Add to render queue in D3DX11
	void Render();
	
private:
	DirectX::XMFLOAT3 myPosition;
	DirectX::XMFLOAT3 myRotation;
	DirectX::XMFLOAT3 myScale;
	
	Model* myModel;
	Texture* myTexture;
};

