#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "internal/Graphics.h"
#include "internal/FrameCounter.h"
#include "internal/Timer.h"
#include "internal/CPUUsage.h"

class D3DSystem
{
	friend class D3DModel;
	friend class D3DSprite;
public:
	D3DSystem();
	bool Initialize(CreateParams& params);
	void Shutdown();
	bool Frame();
	void Run();
	void GetView(float* matrix);
	void GetProjection(float* matrix);
	Camera& GetCamera();
	static void SetInstance(D3DSystem* aInstance);

	DirectX::XMINT2 GetResolution() const;
    void SetResolution(const int width, const int height);
    void SetFullscreen(const bool aFullscreen);
    void SetClearColor(const float r, const float g, const float b);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void ShutdownWindows();

	static Bitmap* CreateBitmap();
	static Model* CreateModel(const char* filePath);
	static Texture* CreateTexture(const char* filePath);
	static void RenderModel(D3DModel* model);
	static void RenderBitmap(D3DSprite* bitmap);

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Graphics m_Graphics;
	CreateParams m_params;

	FrameCounter m_frameCounter;
	CPUUsage m_cpuUsage;
	Timer m_timer;
	static D3DSystem* instance;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static D3DSystem* ApplicationHandle = nullptr;