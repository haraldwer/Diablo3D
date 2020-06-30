#pragma once

class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11ShaderResourceView;
class ID3D11Texture2D;
class Texture
{
public:
	Texture();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() const;

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	bool LoadTarga(const char*, int&, int&);
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

