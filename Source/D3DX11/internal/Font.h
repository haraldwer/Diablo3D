#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Font
{
public:
	Font();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fontFilename, const char* textureFilename);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() const;
	void BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY) const;

private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, const char* filename);
	void ReleaseTexture();
	FontType* m_font;
	Texture* m_texture;
};

