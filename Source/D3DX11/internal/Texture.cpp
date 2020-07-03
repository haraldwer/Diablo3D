#include "Texture.h"
#include <d3d11.h>
#include <stdio.h>
#include "DDSTextureLoader.h"

Texture::Texture(): m_targaData(nullptr), m_texture(nullptr), m_textureView(nullptr)
{
}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
{
	//const size_t cSize = strlen(filename) + 1;
	//wchar_t* wc = new wchar_t[cSize];
	//mbstowcs(wc, filename, cSize);
	//
	//DirectX::CreateDDSTextureFromFile(device, wc, (ID3D11Resource**)(&m_texture), &m_textureView);
	
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load targa image
	result = LoadTarga(filename, height, width);
	if (!result)
	{
		
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
		return false;

	// Set row pitch
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy targa image data into texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup shader resource view desc
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
		return false;

	// Generate mipmaps for texture
	deviceContext->GenerateMips(m_textureView);

	// Release targa image data now that image has been loaded into texture
	delete[] m_targaData;
	m_targaData = nullptr;
	return true;
}

void Texture::Shutdown()
{
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return m_textureView;
}

bool Texture::LoadTarga(const char* filename, int& height, int& width)
{
	FILE* filePtr;
	TargaHeader targaFileHeader;

	// Open targa file for reading in binary
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	// Read in file header
	unsigned int count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr));
	if (count != 1)
		return false;

	// Get information from header
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bpp;

	// Check if 32bit and not 24bit
	if (bpp != 32)
		return false;

	int imageSize = width * height * 4;

	// Allocate memory for the targa image data
	unsigned char* targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	// Close file
	error = fclose(filePtr);
	if (error != 0)
		return false;

	// Allocate memory for targa destination data
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
		return false;

	// Initialize index into targa destination data array
	int index = 0;

	// Initialize index into targa image data
	int k = (width * height * 4) - (width * 4);

	// Copy targa data into dest array (which is stored upside down)
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2]; // R
			m_targaData[index + 1] = targaImage[k + 1];	// G
			m_targaData[index + 2] = targaImage[k + 0];	// B
			m_targaData[index + 3] = targaImage[k + 3];	// A
			
			k += 4;
			index += 4;
		}

		// Set targa image data index back to preceding row at the beginning of the column since its reading it in upside down
		k -= (width * 8);
	}

	// Release targa image data
	delete[] targaImage;
	targaImage = nullptr;
	return true;
}
