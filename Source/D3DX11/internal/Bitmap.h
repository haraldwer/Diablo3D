#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Bitmap
{
public:
	Bitmap();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, float, float);
	
	int GetIndexCount() const;

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, float, float);
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	float m_bitmapWidth, m_bitmapHeight;
	float m_previousPosX, m_previousPosY;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
};