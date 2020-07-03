#include "Graphics.h"

#include "Bitmap.h"
#include "Camera.h"
#include "Light.h"
#include "LightShader.h"
#include "Model.h"
#include "TextureShader.h"

#include "../D3DModel.h"
#include "../D3DSprite.h"
#include "../../CommonUtilities/Log.h"

Graphics::Graphics():
	m_hwnd(nullptr),
	m_screenWidth(0),
	m_screenHeight(0),
	m_fullscreen(false),
	m_Light(),
	m_Text(nullptr), m_frustum()
{
}

bool Graphics::Initialize(const CreateParams& createParams)
{
	bool result;
	DirectX::XMMATRIX baseViewMatrix;
	
	m_hwnd = *createParams.myHwnd;
	m_screenWidth = createParams.myWindowWidth;
	m_screenHeight = createParams.myWindowHeight;
	m_fullscreen = createParams.myStartInFullScreen;
	m_createParams = createParams;
	
	// Initialize the Direct3D object.
	result = m_Direct3D.Initialize(createParams.myDevice, createParams.myDeviceContext, createParams.mySwapChain, createParams.myRTView, m_screenWidth, m_screenHeight, createParams.myEnableVSync, m_hwnd, m_fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		Debug::Error << "Could not initialize Direct3D" << std::endl;
		return false;
	}

	// Set the initial position of the camera.
	m_Camera.SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera.Render();
	m_Camera.GetViewMatrix(baseViewMatrix);

	m_Text = new Text;
	if (!m_Text)
		return false;

	result = m_Text->Initialize(m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), m_hwnd, m_screenWidth, m_screenHeight, baseViewMatrix);
	if (!result)
	{
		Debug::Error << "Could not initialize the text object" << std::endl;
		return false;
	}
	
	// Initialize the light shader object.
	result = m_LightShader.Initialize(m_Direct3D.GetDevice(), m_hwnd);
	if (!result)
	{
		Debug::Error << "Could not initialize the light shader object." << std::endl;
		return false;
	}

	// Initialize the color shader object.
		result = m_ColorShader.Initialize(m_Direct3D.GetDevice(), m_hwnd);
	if (!result)
	{
		Debug::Error << "Could not initialize the color shader object." << std::endl;
		return false;
	}
	
	// Initialize the light object.
	m_Light.SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light.SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.SetDirection(0.0f, 0.0f, 1.0f);
	m_Light.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light.SetSpecularPower(8.0f);
	
	// Initialize the render to texture object.
	result = m_RenderTexture.Initialize(m_Direct3D.GetDevice(), m_screenWidth, m_screenHeight);
	if (!result)
		return false;

	// Initialize the debug window object.
	result = m_RenderTextureBitmap.Initialize(m_Direct3D.GetDevice(), m_screenWidth, m_screenHeight, 0, 0);
	if (!result)
	{
		Debug::Error << "Could not initialize the debug window object." << std::endl;
		return false;
	}
	
	// Initialize the texture shader object.
	result = m_TextureShader.Initialize(m_Direct3D.GetDevice(), m_hwnd);
	if (!result)
	{
		Debug::Error << "Could not initialize the texture shader object." << std::endl;
		return false;
	}
	
	return true;
}


void Graphics::Shutdown()
{
	m_ModelCalls.clear();
	m_BitmapCalls.clear();
	// Release the resource objects
	for(auto& it : m_Bitmaps)
	{
		it->Shutdown();
		delete(it);
	}
	m_Bitmaps.clear();
	for (auto& it : m_Models)
	{
		it.second->Shutdown();
		delete(it.second);
	}
	m_Models.clear();
	for (auto& it : m_Textures)
	{
		it.second->Shutdown();
		delete(it.second);
	}
	m_Textures.clear();
	
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	
	m_LightShader.Shutdown();
	m_ColorShader.Shutdown();
	m_TextureShader.Shutdown();
	
	m_RenderTextureBitmap.Shutdown();
	m_RenderTexture.Shutdown();
	
	m_Direct3D.Shutdown();
}

bool Graphics::Frame(int fps, int cpu, float frameTime)
{
	bool result;

	result = m_Text->SetFps(fps, m_Direct3D.GetDeviceContext());
	if (!result)
		return false;
	result = m_Text->SetCpu(cpu, m_Direct3D.GetDeviceContext());
	if (!result)
		return false;
		
	// Render the graphics scene.
	result = Render();
	if (!result)
		return false;
	return true;
}
void Graphics::SetFullscreen(const bool aFullscreen)
{
	m_fullscreen = aFullscreen;
	m_Direct3D.SetFullscreen(aFullscreen);
}

bool Graphics::GetFullscreen() const
{
	return m_fullscreen;
}

DirectX::XMINT2 Graphics::GetResolution() const
{
	return DirectX::XMINT2(m_screenWidth, m_screenHeight);
}

void Graphics::SetResolution(int width, int height)
{
	if(m_screenWidth != width || m_screenHeight != height)
	{
		m_screenWidth = width;
		m_screenHeight = height;
		m_Direct3D.SetResolution(width, height, m_fullscreen, m_hwnd, SCREEN_DEPTH, SCREEN_NEAR);
		m_RenderTexture.Shutdown();
		m_RenderTexture.Initialize(m_Direct3D.GetDevice(), width, height);
	}
}

bool Graphics::Render()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	if (m_createParams.myPresentFunction)
	{
		// Render the entire scene to the texture first.
		result = RenderToTexture();
		if (!result)
			return false;
		
		m_createParams.myPresentFunction(m_RenderTexture.GetShaderResourceView());
	}
	else
	{
		// Clear the buffers to begin the scene.
		m_Direct3D.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		result = RenderScene();
		if (!result)
			return false;
		
		// Present the rendered scene to the screen.
		m_Direct3D.EndScene();
		
	}
	return true;
}

bool Graphics::RenderScene()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Generate the view matrix based on the camera's position.
	m_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D.GetWorldMatrix(worldMatrix);
	m_Camera.GetViewMatrix(viewMatrix);
	m_Direct3D.GetProjectionMatrix(projectionMatrix);
	m_Direct3D.GetOrthoMatrix(orthoMatrix);

	m_frustum.ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Render models using lightshader
	for(auto& it : m_ModelCalls)
	{
		auto model = it->myModel;
		auto texture = it->myTexture;
		if(model)
		{
			const auto pos = it->myPosition;
			const auto rot = it->myRotation;
			const auto scale = it->myScale;

			if(m_frustum.CheckPoint(pos.x, pos.y, pos.z))
			{
				// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
				model->Render(m_Direct3D.GetDeviceContext());

				const auto world = worldMatrix * 
					DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) * 
					DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * 
					DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

				if(texture)
				{
					// Render the model using the light shader.
					result = m_LightShader.Render(m_Direct3D.GetDeviceContext(), model->GetIndexCount(), world, viewMatrix, projectionMatrix,
						texture->GetTexture(), m_Light.GetDirection(), m_Light.GetAmbientColor(), m_Light.GetDiffuseColor(), m_Camera.GetPosition(), m_Light.GetSpecularColor(), m_Light.GetSpecularPower());
					if (!result)
						return false;
				}
				else
				{
					result = m_ColorShader.Render(m_Direct3D.GetDeviceContext(), model->GetIndexCount(), world, viewMatrix, projectionMatrix);
					if (!result)
						return false;
				}
			}
		}
	}
	const auto modelCalls = m_ModelCalls.size();
	m_ModelCalls.clear();
	m_ModelCalls.reserve(modelCalls);
	
	m_Direct3D.TurnZBufferOff();
	m_Direct3D.TurnOnAlphaBlending();

	// Render bitmaps using textureShader
	for(auto& it : m_BitmapCalls)
	{
		auto bitmap = it->myBitmap;
		auto texture = it->myTexture;
		if(bitmap)
		{
			const auto pos = it->myPosition;
			result = bitmap->Render(m_Direct3D.GetDeviceContext(), pos.x, pos.y);
			if (!result)
				return false;
			if(texture)
			{
				result = m_TextureShader.Render(m_Direct3D.GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, texture->GetTexture());
				if (!result)
					return false;
			}
			else
			{
				result = m_ColorShader.Render(m_Direct3D.GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix);
				if (!result)
					return false;
			}
		}
	}
	const auto bitmapCalls = m_BitmapCalls.size();
	m_BitmapCalls.clear();
	m_BitmapCalls.reserve(bitmapCalls);
	
	m_Text->Render(m_Direct3D.GetDeviceContext(), worldMatrix, orthoMatrix);
	
	m_Direct3D.TurnOffAlphaBlending();
	m_Direct3D.TurnZBufferOn();
	return true;
}

bool Graphics::RenderToTexture()
{
	// Set the render target to be the render to texture.
	m_RenderTexture.SetRenderTarget(m_Direct3D.GetDeviceContext(), m_Direct3D.GetDepthStencilView());
	
	// Clear the render to texture.
	m_RenderTexture.ClearRenderTarget(m_Direct3D.GetDeviceContext(), m_Direct3D.GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	bool result = RenderScene();
	if (!result)
		return false;
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D.SetBackBufferRenderTarget();
	return true;
}

Bitmap* Graphics::CreateBitmap()
{
	auto bitmap = new Bitmap;
	bool result = bitmap->Initialize(m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext());
	if (!result)
	{
		Debug::Error << "Could not initialize bitmap object" << std::endl;
		return nullptr;
	}
	m_Bitmaps.push_back(bitmap);
	return bitmap;
}

Model* Graphics::CreateModel(const char* filePath)
{
	auto itr = m_Models.find(filePath);
	if (itr != m_Models.end())
	{
		return itr->second;
	}
	auto model = new Model;
	bool result = model->Initialize(m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), filePath);
	if (!result)
	{
		Debug::Error << "Could not initialize model object" << std::endl;
		return nullptr;
	}
	m_Models[filePath] = model;
	return model;
}

Texture* Graphics::CreateTexture(const char* filePath)
{
	auto itr = m_Textures.find(filePath);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}
	auto texture = new Texture;
	bool result = texture->Initialize(m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), filePath);
	if (!result)
	{
		Debug::Error << "Could not initialize model object" << std::endl;
		return nullptr;
	}
	m_Textures[filePath] = texture;
	return texture;
}

void Graphics::RenderModel(D3DModel* model)
{
	if (model)
		m_ModelCalls.push_back(model);
}

void Graphics::RenderBitmap(D3DSprite* bitmap)
{
	if (bitmap)
		m_BitmapCalls.push_back(bitmap);
}
