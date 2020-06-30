#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class LightShader
{
public:
	LightShader();
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection,
	            ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor,
	            DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT4 specularColor, float specularPower);
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	struct LightBufferType
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float specularPower;
		DirectX::XMFLOAT4 specularColor;
	};
	struct CameraBufferType
	{
		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParameteres(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view,
	                          DirectX::XMMATRIX projection, ID3D11ShaderResourceView* texture,
	                          DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor,
	                          DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT3 cameraPosition,
	                          DirectX::XMFLOAT4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
};

