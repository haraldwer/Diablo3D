#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

struct CreateParams;

class D3D
{
public:
	D3D();

	bool Initialize(CreateParams*, float, float);
	bool CreateDescriptions();
	bool CreateResources(float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float, float, float, float) const;
	void EndScene() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void GetProjectionMatrix(DirectX::XMMATRIX&) const;
	void GetWorldMatrix(DirectX::XMMATRIX&) const;
	void GetOrthoMatrix(DirectX::XMMATRIX&) const;

	void GetVideoCardInfo(char*, int&) const;

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();
	
	void SetFullscreen(const bool aFullscreen);
	void SetResolution(int aWidth, int aHeight, float aScreenDepth, float aScreenNear);


private:
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	bool m_handleSwap;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	CreateParams* m_createParams;
};
