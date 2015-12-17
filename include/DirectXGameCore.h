// -------------------------------------------------------------
//  Base class for a DirectX 11 Game, borrowing from
//  the structure set up by Frank D. Luna in "3D Game
//  Programming with DirectX 11" (d3dApp.h & d3dApp.cpp)
//
//  This version doesn't rely on D3DX or the Effect framework,
//  as these libraries are deprecated.
//
//  You should not need to edit this class at all.
// -------------------------------------------------------------

#pragma once

#include "Windows.h"
#include <string>
#include <d3d11.h>
#include "DXMacros.h"

// --------------------------------------------------------
// The core class for the DirectX Starter Code
// --------------------------------------------------------
class DirectXGameCore
{
public:
	DirectXGameCore(HINSTANCE hInstance);
	virtual ~DirectXGameCore(void);
	
	// The game loop
	int Run();
 
	// Methods called by the game loop - override these in
	// derived classes to implement custom functionality
	virtual bool Init();
	virtual void OnResize(); 
	virtual LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// Handles window and Direct3D initialization
	bool InitMainWindow();
	bool InitDirect3D();

	// Used to properly quit the game
	void Quit();

	// Window handles and such
	HINSTANCE hAppInst;
	HWND      hMainWnd;

	// Game and window state tracking
	bool      hasFocus;
	bool      minimized;
	bool      maximized;
	bool      resizing;
	
	// DirectX related buffers, views, etc.
	ID3D11Device*             device;
	ID3D11DeviceContext*      deviceContext;
	IDXGISwapChain*           swapChain;
	ID3D11Texture2D*          depthStencilBuffer;
	ID3D11RenderTargetView*   renderTargetView;
	ID3D11DepthStencilView*   depthStencilView;
	D3D11_VIEWPORT            viewport;
	D3D_DRIVER_TYPE           driverType;
	D3D_FEATURE_LEVEL         featureLevel;

	// The window's aspect ratio, used mostly for your projection matrix
	float aspectRatio;

	// Derived class can set these in derived constructor to customize starting values.
	std::wstring windowCaption;
	int windowWidth;
	int windowHeight;

private:
	// Timer related data
	double perfCounterSeconds;
	__int64 startTime;
	__int64 currentTime;
	__int64 previousTime;
	float totalTime;
	float deltaTime;

	// Updates the timer for this frame
	void UpdateTimer();

	// Calculates stats about the current frame and
	// updates the window's title bar
	void CalculateFrameStats();
};

