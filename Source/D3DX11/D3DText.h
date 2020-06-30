#pragma once
#include <DirectXMath.h>
#include <string>

class D3DText
{
public:
	D3DText(const char* font);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetRotation(float rot);
	void SetText(const char* text);
private:
	DirectX::XMFLOAT2 myPosition;
	DirectX::XMFLOAT2 myScale;
	float myRotation;
	std::string myText;
};


