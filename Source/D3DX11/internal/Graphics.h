#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "Camera.h"
#include "ColorShader.h"
#include "D3D.h"
#include "Light.h"
#include "Text.h"
#include "Frustum.h"
#include "RenderTexture.h"
#include "RenderTextureBitmap.h"

#include "LightShader.h"
#include "TextureShader.h"

class Model;
class D3DModel;
class Bitmap;
class D3DSprite;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

enum class WindowSetting
{
    OVERLAPPED,
    BORDERLESS
};

struct CreateParams
{
    CreateParams()
    {
        myHwnd = nullptr;
        myDevice = nullptr;
        myDeviceContext = nullptr;
        mySwapChain = nullptr;
        myRTView = nullptr;
        myWindowWidth = 1280;
        myWindowHeight = 720;
        myEnableVSync = false;
        myFullscreen = false;
        myShowCursor = true;
        myClearColor = { 0, 0, 0, 1 };
        myWindowSetting = WindowSetting::OVERLAPPED;
        myApplicationName = L"Application";
    }

    std::function<void()> myInitFunction;
    std::function<void()> myUpdateFunction;
    std::function<void(ID3D11ShaderResourceView*)> myPresentFunction;
    std::function<void(HWND, UINT, WPARAM, LPARAM)> myMessageHandlerFunction;
	
    /* How big should the window be? */
    unsigned short myWindowWidth;
    unsigned short myWindowHeight;

    /* Will show the FPS and memory text*/
    DirectX::XMFLOAT4 myClearColor;
    HWND* myHwnd;
    std::wstring myApplicationName;
    bool myEnableVSync;
    bool myFullscreen;
    bool myShowCursor;
    WindowSetting myWindowSetting;
    void* myTargetTexture;
    ID3D11Device* myDevice;
    ID3D11DeviceContext* myDeviceContext;
    IDXGISwapChain* mySwapChain;
    ID3D11RenderTargetView* myRTView;
};

class Graphics
{
public:
	Graphics();

	bool Initialize(CreateParams* createParams);
	void Shutdown();
	bool Frame(int fps, int cpu, float frameTime);
    bool Render();
	
	void SetFullscreen(const bool aFullscreen);
	bool GetFullscreen() const;
	void SetResolution(int width, int height);
    DirectX::XMINT2 GetResolution() const;

	Bitmap* CreateBitmap();
    Model* CreateModel(const char* filePath);
    Texture* CreateTexture(const char* filePath);

    void RenderModel(D3DModel* model);
    void RenderBitmap(D3DSprite* bitmap);
	void GetView(float* matrix) const;
	void GetProjection(float* matrix) const;
	Camera& GetCamera();

private:
	
    bool RenderToTexture();
	bool RenderScene();
	D3D m_Direct3D;
	Camera m_Camera;
	Light m_Light;

	// These work like resources, one instance per resource
	std::vector<Bitmap*> m_Bitmaps;
    std::unordered_map<std::string, Model*> m_Models;
    std::unordered_map<std::string, Texture*> m_Textures;

    std::vector<D3DModel*> m_ModelCalls;
    std::vector<D3DSprite*> m_BitmapCalls;
	
	Text* m_Text;
	
	LightShader m_LightShader;
	TextureShader m_TextureShader;
    ColorShader m_ColorShader;
    CreateParams* m_createParams;

    Frustum m_frustum;
    RenderTexture m_RenderTexture;
    RenderTextureBitmap m_RenderTextureBitmap;
};

