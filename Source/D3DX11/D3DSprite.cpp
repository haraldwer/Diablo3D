#include "D3DSprite.h"
#include "D3DSystem.h"

D3DSprite::D3DSprite(const char* aSpritePath) : myPosition(), myRotation(0), myScale(), myUV(), myColor(),
                                                myBitmap(nullptr), myTexture(nullptr), mySpritePath(aSpritePath)
{
	myBitmap = D3DSystem::CreateBitmap();
	myTexture = D3DSystem::CreateTexture(aSpritePath);
}

void D3DSprite::SetPosition(const float x, const float y)
{
	myPosition = DirectX::XMFLOAT2(x, y);
}

void D3DSprite::SetRotation(const float rot)
{
	myRotation = rot;
}

void D3DSprite::SetScale(const float x, const float y)
{
	myScale = DirectX::XMFLOAT2(x, y);
}

void D3DSprite::SetUV(const float x, const float y)
{
	myUV = DirectX::XMFLOAT2(x, y);
}

void D3DSprite::SetColor(const float r, const float g, const float b, const float a)
{
	myColor = DirectX::XMFLOAT4(r, g, b, a);
}

void D3DSprite::Render()
{
	D3DSystem::RenderBitmap(this);
}
