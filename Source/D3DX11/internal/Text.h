#pragma once

#include "Font.h"
#include "FontShader.h"

class Text
{
public:
	Text();
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, DirectX::XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX);
	bool SetFps (int fps, ID3D11DeviceContext* deviceContext);
	bool SetCpu(int cpu, ID3D11DeviceContext* deviceContext);

private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, DirectX::XMMATRIX, DirectX::XMMATRIX);

	Font* m_Font;
	FontShader* m_FontShader;
	int m_screenWidth, m_screenHeight;
	DirectX::XMMATRIX m_baseViewMatrix;

	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};

