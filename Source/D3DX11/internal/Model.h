#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Model
{
public:
	Model();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* modelFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);
	
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(const char*);
	void ReleaseModel();

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	ModelType* m_model;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};
};