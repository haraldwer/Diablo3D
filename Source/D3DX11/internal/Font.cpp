#include "Font.h"
#include <fstream>

Font::Font(): m_font(nullptr), m_texture(nullptr)
{
}

bool Font::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fontFilename, const char* textureFilename)
{
	bool result;
	result = LoadFontData(fontFilename);
	if (!result)
		return false;
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
		return false;
	return true;
}

void Font::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

ID3D11ShaderResourceView* Font::GetTexture() const
{
	return m_texture->GetTexture();
}

void Font::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY) const
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	vertexPtr = (VertexType*)vertices;
	numLetters = (int)strlen(sentence);
	index = 0;
	for(int i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = DirectX::XMFLOAT3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = DirectX::XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
}

bool Font::LoadFontData(const char* filename)
{
	std::ifstream fin;
	char temp;

	// Create font spacing buffer
	m_font = new FontType[95];
	if (!m_font)
		return false;
	fin.open(filename);
	if (fin.fail())
		return false;
	for(int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
			fin.get(temp);
		fin.get(temp);
		while (temp != ' ')
			fin.get(temp);
		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}
	fin.close();
	return true;
}

void Font::ReleaseFontData()
{
	if(m_font)
	{
		delete[] m_font;
		m_font = nullptr;
	}
}

bool Font::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* context, const char* filename)
{
	bool result;
	m_texture = new Texture;
	if (!m_texture)
		return false;
	result = m_texture->Initialize(device, context, filename);
	if (!result)
		return false;
	return true;
}

void Font::ReleaseTexture()
{
	if(m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}
}
