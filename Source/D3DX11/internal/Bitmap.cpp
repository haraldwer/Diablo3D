#include "Bitmap.h"

Bitmap::Bitmap(): m_vertexBuffer(nullptr),
m_indexBuffer(nullptr),
m_vertexCount(0),
m_indexCount(0),
m_bitmapWidth(0),
m_bitmapHeight(0),
m_previousPosX(0),
m_previousPosY(0)
{
}

bool Bitmap::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_bitmapWidth = 1.0f;
	m_bitmapHeight = 1.0f;

	m_previousPosX = -1;
	m_previousPosY = -1;

	bool result = InitializeBuffers(device);
	if (!result)
		return false;
	return true;
}

void Bitmap::Shutdown()
{
	ShutdownBuffers();
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, float posX, float posY)
{
	bool result = UpdateBuffers(deviceContext, posX, posY);
	if (!result)
		return false;
	RenderBuffers(deviceContext);
	return true;
}

int Bitmap::GetIndexCount() const
{
	return m_indexCount;
}

bool Bitmap::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create square with 6 vertices
	m_vertexCount = 6;
	m_indexCount = m_vertexCount;
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));
	for (int i = 0; i < m_indexCount; i++)
		indices[i] = i;

	// Create dynamic buffer instead of static, because we wanna change values each frame
	// Vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give ptrs to vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	// Index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = nullptr;
	delete[] indices;
	indices = nullptr;
	
	return true;
}

void Bitmap::ShutdownBuffers()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, float posX, float posY)
{
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// If position since last frame hasn't changed
	if (posX == m_previousPosX && posY == m_previousPosY)
		return true;

	m_previousPosX = posX;
	m_previousPosY = posY;

	// 01 to screen space here
	const float left = posX - m_bitmapWidth * 0.5f;
	const float right = left + m_bitmapWidth;
	const float top = posY - m_bitmapWidth * 0.5f;
	const float bottom = top + m_bitmapHeight;
	
	// Create vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// First triangle
	vertices[0].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = DirectX::XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)) * m_vertexCount);
	deviceContext->Unmap(m_vertexBuffer, 0);
	delete[] vertices;
	vertices = nullptr;
	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexType);
	offset = 0;

	// Set buffers to active in input assembler
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}