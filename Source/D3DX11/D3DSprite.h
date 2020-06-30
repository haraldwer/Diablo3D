#pragma once
#include <DirectXMath.h>
#include <string>

class Bitmap;
class Texture;

class D3DSprite
{
	friend class Graphics;
public:
	D3DSprite(const char* aSpritePath);
	void SetPosition(float x, float y);
	void SetRotation(float rot);
	void SetScale(float x, float y);
	void SetUV(float x, float y);
	void SetColor(float r, float g, float b, float a);

	// Add to render queue in D3DX11
	void Render();
	
private:
	DirectX::XMFLOAT2 myPosition;
	float myRotation;
	DirectX::XMFLOAT2 myScale;
	DirectX::XMFLOAT2 myUV;
	DirectX::XMFLOAT4 myColor;
	const std::string mySpritePath;
	
	Bitmap* myBitmap;
	Texture* myTexture;
};

