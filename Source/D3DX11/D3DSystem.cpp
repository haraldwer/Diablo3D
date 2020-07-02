#include "D3DSystem.h"
#include "internal/Graphics.h"

D3DSystem* D3DSystem::instance = nullptr;

D3DSystem::D3DSystem(): m_applicationName(nullptr), m_hinstance(nullptr), m_hwnd(nullptr), m_frameCounter(),
                        m_cpuUsage(), m_timer()
{
}

bool D3DSystem::Initialize(CreateParams& params)
{
	bool result;

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	ApplicationHandle = this;
	instance = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = params.myApplicationName.c_str();

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (params.myStartInFullScreen)
	{
		params.myWindowWidth = GetSystemMetrics(SM_CXSCREEN);
		params.myWindowHeight = GetSystemMetrics(SM_CYSCREEN);
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)params.myWindowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)params.myWindowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
	}

	if(params.myHwnd == nullptr)
	{
		DWORD windowStyle = 0;
		switch(params.myWindowSetting)
		{
		case WindowSetting::OVERLAPPED:
			windowStyle = WS_OVERLAPPEDWINDOW;
			break;
		case WindowSetting::BORDERLESS:
			windowStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
			break;
		}
		
		// Create the window with the screen settings and get the handle to it.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
			m_applicationName,
			m_applicationName,
			windowStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			params.myWindowWidth,
			params.myWindowHeight,
			NULL,
			NULL,
			m_hinstance,
			NULL);
		params.myHwnd = &m_hwnd;
	}
	else
	{
		m_hwnd = *params.myHwnd;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	if(!params.myShowCursor)
		ShowCursor(false);

	// Initialize the graphics object.
	result = m_Graphics.Initialize(params);
	if (!result)
		return false;

	m_params = params;
	if (m_params.myInitFunction)
		m_params.myInitFunction();

	m_frameCounter.Initialize();
	m_cpuUsage.Initialize();
	m_timer.Initialize();
	return true;
}

void D3DSystem::Shutdown()
{
	m_cpuUsage.Shutdown();
	m_Graphics.Shutdown();
	ShutdownWindows();
}

void D3DSystem::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
				done = true;
		}
	}
}

void D3DSystem::SetInstance(D3DSystem* aInstance)
{
	instance = aInstance;
}

bool D3DSystem::Frame()
{
	bool result;

	m_frameCounter.Frame();
	m_cpuUsage.Frame();
	m_timer.Frame();
	
	if (m_params.myUpdateFunction)
		m_params.myUpdateFunction();
	
	// Do the frame processing for the graphics object.
	result = m_Graphics.Frame(m_frameCounter.GetFps(), m_cpuUsage.GetCpuPercentage(), m_timer.GetTime());
	if (!result)
		return false;

	return true;
}


LRESULT CALLBACK D3DSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if(m_params.myMessageHandlerFunction)
		return m_params.myMessageHandlerFunction(hwnd, umsg, wparam, lparam);

	static bool minimized = false;
	
	switch (umsg)
	{

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void D3DSystem::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_Graphics.GetFullscreen())
		ChangeDisplaySettings(NULL, 0);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	// All other messages pass to the message handler in the system class.
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}


DirectX::XMINT2 D3DSystem::GetResolution() const
{
	return m_Graphics.GetResolution();
}

void D3DSystem::SetResolution(const int width, const int height)
{
	m_Graphics.SetResolution(width, height);
}

void D3DSystem::SetFullscreen(const bool aFullscreen)
{
	m_Graphics.SetFullscreen(aFullscreen);
}

void D3DSystem::SetClearColor(const float r, const float g, const float b)
{
}


Bitmap* D3DSystem::CreateBitmap()
{
	return instance->m_Graphics.CreateBitmap();
}

Model* D3DSystem::CreateModel(const char* filePath)
{
	return instance->m_Graphics.CreateModel(filePath);
}

Texture* D3DSystem::CreateTexture(const char* filePath)
{
	return instance->m_Graphics.CreateTexture(filePath);
}

void D3DSystem::RenderModel(D3DModel* model)
{
	instance->m_Graphics.RenderModel(model);
}

void D3DSystem::RenderBitmap(D3DSprite* bitmap)
{
	instance->m_Graphics.RenderBitmap(bitmap);
}