#include "Model.h"

#include <fstream>
#include <vector>

Model::Model(): m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexCount(0),
                m_indexCount(0), m_model(nullptr)
{
}

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* modelFilename)
{
	bool result;
	// Load model
	result = LoadModel(modelFilename);
	if (!result)
		return false;
	
	result = InitializeBuffers(device);
	if (!result)
		return false;
}

void Model::Shutdown()
{
	ShutdownBuffers();
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	for(int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = DirectX::XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		indices[i] = i;
	}

	// Set desc of vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give subresource ptr to vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	// Set desc of index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give subresource ptr to index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// Release arrays now that buffers are created
	delete[] vertices;
	vertices = nullptr;
	delete indices;
	indices = nullptr;
	return true;
}

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadModel(const char* filename)
{
	struct Vec3
	{
		float x;
		float y;
		float z;
	};
	struct Vec2
	{
		float x;
		float y;
	};
	struct Face
	{
		int vIndex1, vIndex2, vIndex3, vIndex4;
		int tIndex1, tIndex2, tIndex3, tIndex4;
		int nIndex1, nIndex2, nIndex3, nIndex4;
		bool useFourth = false;
	};
	
	// Open the file.
	std::ifstream fin;
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
		return false;

	
	std::vector<Vec3> vertices;
	std::vector<Vec2> texcoords;
	std::vector<Vec3> normals;
	std::vector<Face> faces;
	
	char input;
	char input2;
	int vertexCount = 0;
	fin.get(input);
	int lines = 0;
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				Vec3 vec;
				fin >> vec.x >> vec.y >> vec.z;
				vertices.push_back(vec);
			}
			if (input == 't')
			{
				Vec2 vec;
				fin >> vec.x >> vec.y;
				texcoords.push_back(vec);
			}
			if (input == 'n')
			{
				Vec3 vec;
				fin >> vec.x >> vec.y >> vec.z;
				normals.push_back(vec);
			}
		}
		
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				Face face;
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> face.vIndex4 >> input2 >> face.tIndex4 >> input2 >> face.nIndex4
					>> face.vIndex3 >> input2 >> face.tIndex3 >> input2 >> face.nIndex3
					>> face.vIndex2 >> input2 >> face.tIndex2 >> input2 >> face.nIndex2;
				vertexCount += 3;
				fin.get(input);
				if(input == ' ')
				{
					fin >> face.vIndex1 >> input2 >> face.tIndex1 >> input2 >> face.nIndex1;
					face.useFourth = true;
					vertexCount += 3;
				}
				faces.push_back(face);
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	m_vertexCount = vertexCount;
	m_indexCount = m_vertexCount;
	m_model = new ModelType[m_vertexCount];

	// Now loop through all the faces and output the three vertices for each face.
	int offset = 0;
	int vIndex;
	int tIndex;
	int nIndex;
	for(auto& face : faces)
	{
		vIndex = face.vIndex2 - 1;
		tIndex = face.tIndex2 - 1;
		nIndex = face.nIndex2 - 1;
		m_model[offset].x = vertices[vIndex].x;
		m_model[offset].y = vertices[vIndex].y;
		m_model[offset].z = vertices[vIndex].z * -1.0f;
		m_model[offset].tu = texcoords[tIndex].x;
		m_model[offset].tv = 1.0f - texcoords[tIndex].y;
		m_model[offset].nx = normals[nIndex].x;
		m_model[offset].ny = normals[nIndex].y;
		m_model[offset].nz = normals[nIndex].z * -1.0f;
		offset++;
		vIndex = face.vIndex3 - 1;
		tIndex = face.tIndex3 - 1;
		nIndex = face.nIndex3 - 1;
		m_model[offset].x = vertices[vIndex].x;
		m_model[offset].y = vertices[vIndex].y;
		m_model[offset].z = vertices[vIndex].z * -1.0f;
		m_model[offset].tu = texcoords[tIndex].x;
		m_model[offset].tv = 1.0f - texcoords[tIndex].y;
		m_model[offset].nx = normals[nIndex].x;
		m_model[offset].ny = normals[nIndex].y;
		m_model[offset].nz = normals[nIndex].z * -1.0f;
		offset++;
		vIndex = face.vIndex4 - 1;
		tIndex = face.tIndex4 - 1;
		nIndex = face.nIndex4 - 1;
		m_model[offset].x = vertices[vIndex].x;
		m_model[offset].y = vertices[vIndex].y;
		m_model[offset].z = vertices[vIndex].z * -1.0f;
		m_model[offset].tu = texcoords[tIndex].x;
		m_model[offset].tv = 1.0f - texcoords[tIndex].y;
		m_model[offset].nx = normals[nIndex].x;
		m_model[offset].ny = normals[nIndex].y;
		m_model[offset].nz = normals[nIndex].z * -1.0f;
		offset++;

		if (face.useFourth)
		{
			
			vIndex = face.vIndex2 - 1;
			tIndex = face.tIndex2 - 1;
			nIndex = face.nIndex2 - 1;
			m_model[offset].x = vertices[vIndex].x;
			m_model[offset].y = vertices[vIndex].y;
			m_model[offset].z = vertices[vIndex].z * -1.0f;
			m_model[offset].tu = texcoords[tIndex].x;
			m_model[offset].tv = 1.0f - texcoords[tIndex].y;
			m_model[offset].nx = normals[nIndex].x;
			m_model[offset].ny = normals[nIndex].y;
			m_model[offset].nz = normals[nIndex].z * -1.0f;
			offset++;
			vIndex = face.vIndex4 - 1;
			tIndex = face.tIndex4 - 1;
			nIndex = face.nIndex4 - 1;
			m_model[offset].x = vertices[vIndex].x;
			m_model[offset].y = vertices[vIndex].y;
			m_model[offset].z = vertices[vIndex].z * -1.0f;
			m_model[offset].tu = texcoords[tIndex].x;
			m_model[offset].tv = 1.0f - texcoords[tIndex].y;
			m_model[offset].nx = normals[nIndex].x;
			m_model[offset].ny = normals[nIndex].y;
			m_model[offset].nz = normals[nIndex].z * -1.0f;
			offset++;
			vIndex = face.vIndex1 - 1;
			tIndex = face.tIndex1 - 1;
			nIndex = face.nIndex1 - 1;
			m_model[offset].x = vertices[vIndex].x;
			m_model[offset].y = vertices[vIndex].y;
			m_model[offset].z = vertices[vIndex].z * -1.0f;
			m_model[offset].tu = texcoords[tIndex].x;
			m_model[offset].tv = 1.0f - texcoords[tIndex].y;
			m_model[offset].nx = normals[nIndex].x;
			m_model[offset].ny = normals[nIndex].y;
			m_model[offset].nz = normals[nIndex].z * -1.0f;
			offset++;
		}
	}

	normals.clear();
	vertices.clear();
	texcoords.clear();
	faces.clear();
	
	return true;
}


void Model::ReleaseModel()
{
	if(m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}
