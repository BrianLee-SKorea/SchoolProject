#pragma once

#include <Windows.h>
#include "D3D.h"

class WfWindow
{
public:
	WfWindow();
	~WfWindow();

	HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow, LONG nWidth = 800, LONG nHeight = 600 );
	bool Init();
	int Run();
	void CleanUp();

private:
	void PlayDemo();
	void Update();

	void RenderFrame();
	void RenderScene();
	void RenderInfo();

private:
	HINSTANCE m_hInst = nullptr;
	HWND m_hWnd = nullptr;

	LONG m_nWidth;
	LONG m_nHeight;

	D3D* m_pD3D = nullptr;
};

static WfWindow* g_pApp = nullptr;

LRESULT CALLBACK WfProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void ProcessInput( HWND hWnd, WPARAM key );