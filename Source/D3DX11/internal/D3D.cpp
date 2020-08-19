#include "D3D.h"
#include "Graphics.h"

D3D::D3D() :
	m_videoCardMemory(0),
	m_videoCardDescription{}, m_handleSwap(true),
	m_depthStencilBuffer(nullptr),
	m_depthStencilState(nullptr),
	m_depthDisabledStencilState(nullptr),
	m_depthStencilView(nullptr),
	m_rasterState(nullptr),
	m_projectionMatrix(),
	m_worldMatrix(),
	m_orthoMatrix(),
	m_alphaEnableBlendingState(nullptr),
	m_alphaDisableBlendingState(nullptr),
	m_createParams(nullptr)
{
}

D3D11_TEXTURE2D_DESC depthBufferDesc;
D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
D3D11_RASTERIZER_DESC rasterDesc;
D3D11_BLEND_DESC blendStateDescription;

bool D3D::Initialize(CreateParams* createParams, float screenDepth, float screenNear)
{
	if (!createParams)
		return false;
	m_handleSwap = true;
	m_createParams = createParams;
	if(createParams->myDevice && createParams->myDeviceContext && createParams->mySwapChain && createParams->myRTView)
	{
		m_handleSwap = false;
	}

	if (!CreateDescriptions())
		return false;
	if (!CreateResources(screenDepth, screenNear))
		return false;
	return true;
}

bool D3D::CreateDescriptions()
{
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	
	// Clear second depth stencil
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	// Create second depth stencil with zbuffer off for 2D-rendering
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	return true;
}

bool D3D::CreateResources(float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	if (m_handleSwap)
	{
		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)m_createParams->myWindowWidth)
			{
				if (displayModeList[i].Height == (unsigned int)m_createParams->myWindowHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = m_createParams->myWindowWidth;
		swapChainDesc.BufferDesc.Height = m_createParams->myWindowHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (m_createParams->myEnableVSync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = *m_createParams->myHwnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (m_createParams->myFullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_createParams->mySwapChain, &m_createParams->myDevice, NULL, &m_createParams->myDeviceContext);
		if (FAILED(result))
			return false;

		// Get the pointer to the back buffer.
		result = m_createParams->mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
			return false;

		// Create the render target view with the back buffer pointer.
		result = m_createParams->myDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_createParams->myRTView);
		if (FAILED(result))
			return false;

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;
	}

	depthBufferDesc.Width = m_createParams->myWindowWidth;
	depthBufferDesc.Height = m_createParams->myWindowHeight;

	// Create the texture for the depth buffer using the filled out description.
	result = m_createParams->myDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create the depth stencil state.
	result = m_createParams->myDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_createParams->myDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Create the depth stencil view.
	result = m_createParams->myDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_createParams->myDeviceContext->OMSetRenderTargets(1, &m_createParams->myRTView, m_depthStencilView);

	// Create the rasterizer state from the description we just filled out.
	result = m_createParams->myDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_createParams->myDeviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)m_createParams->myWindowWidth;
	viewport.Height = (float)m_createParams->myWindowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_createParams->myDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)m_createParams->myWindowWidth / (float)m_createParams->myWindowHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)m_createParams->myWindowWidth, (float)m_createParams->myWindowHeight, screenNear, screenDepth);

	// Create state using device
	result = m_createParams->myDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
		return false;

	// Create the blend state using the description.
	result = m_createParams->myDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
		return false;

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = m_createParams->myDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
		return false;

	return true;
}

void D3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_createParams->mySwapChain)
	{
		m_createParams->mySwapChain->SetFullscreenState(false, NULL);
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}
	
	if(m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_createParams->myRTView)
	{
		m_createParams->myRTView->Release();
		m_createParams->myRTView = 0;
	}

	if (m_createParams->myDeviceContext)
	{
		m_createParams->myDeviceContext->Release();
		m_createParams->myDeviceContext = 0;
	}

	if (m_createParams->myDevice)
	{
		m_createParams->myDevice->Release();
		m_createParams->myDevice = 0;
	}

	if (m_createParams->mySwapChain)
	{
		m_createParams->mySwapChain->Release();
		m_createParams->mySwapChain = 0;
	}

	return;
}

void D3D::BeginScene(float red, float green, float blue, float alpha) const
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_createParams->myDeviceContext->ClearRenderTargetView(m_createParams->myRTView, color);

	// Clear the depth buffer.
	m_createParams->myDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3D::EndScene() const
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_createParams->mySwapChain && m_handleSwap)
	{
		if (m_createParams->myEnableVSync)
		{
			// Lock to screen refresh rate.
			m_createParams->mySwapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			m_createParams->mySwapChain->Present(0, 0);
		}
	}
}

ID3D11Device* D3D::GetDevice() const
{
	return m_createParams->myDevice;
}

ID3D11DeviceContext* D3D::GetDeviceContext() const
{
	return m_createParams->myDeviceContext;
}

void D3D::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix) const
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void D3D::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) const
{
	worldMatrix = m_worldMatrix;
	return;
}

void D3D::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix) const
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3D::GetVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3D::TurnZBufferOn()
{
	m_createParams->myDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3D::TurnZBufferOff()
{
	m_createParams->myDeviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void D3D::TurnOnAlphaBlending()
{
	float blendFactor[4];
	
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_createParams->myDeviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3D::TurnOffAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_createParams->myDeviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

ID3D11DepthStencilView* D3D::GetDepthStencilView()
{
	return m_depthStencilView;
}

void D3D::SetBackBufferRenderTarget()
{
	m_createParams->myDeviceContext->OMSetRenderTargets(1, &m_createParams->myRTView, m_depthStencilView);
}

void D3D::SetFullscreen(const bool aFullscreen)
{
	if(m_createParams->mySwapChain)
		m_createParams->mySwapChain->SetFullscreenState(aFullscreen, nullptr);
}

void D3D::SetResolution(int aWidth, int aHeight, float aScreenDepth, float aScreenNear)
{
	if(m_handleSwap)
	{
		if(m_createParams->myDeviceContext && m_createParams->myRTView && m_depthStencilView)
		{
			ID3D11RenderTargetView* nullViews[] = { m_createParams->myRTView };
			m_createParams->myDeviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			m_createParams->myRTView->Release();
			m_createParams->myRTView = nullptr;
			m_depthStencilView->Release();
			m_depthStencilView = nullptr;
			m_createParams->myDeviceContext->Flush();
		}
	}

	if(m_createParams->mySwapChain)
	{
		// 2. Resize the existing swapchain
		HRESULT hr = m_createParams->mySwapChain->ResizeBuffers(2, aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// This means that all resources has to be re-created!
		}
	}
	
	// 3. Recreate resources
	CreateResources(aScreenDepth, aScreenNear);
}
