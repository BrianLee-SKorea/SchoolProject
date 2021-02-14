#include "WfWindow.h"

WfWindow::WfWindow()
{
	g_pApp = this;
}

WfWindow::~WfWindow()
{
	g_pApp = nullptr;
}

HRESULT WfWindow::InitWindow( HINSTANCE hInstance, int nCmdShow, LONG nWidth, LONG nHeight )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WfProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, (LPCTSTR)IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WfShader";
	wcex.hIconSm = LoadIcon( wcex.hInstance, (LPCTSTR)IDI_APPLICATION );
	if( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	m_hInst = hInstance;
	m_nWidth = nWidth;
	m_nHeight = nHeight;


	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rcClient = { 0, 0, m_nWidth, m_nHeight };
	AdjustWindowRect( &rcClient, dwStyle, FALSE );

	m_hWnd = CreateWindow(
		L"WfShader",
		L"Base",
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top,
		nullptr,
		nullptr,
		m_hInst,
		nullptr
	);

	if( !m_hWnd )
		return E_FAIL;

	ShowWindow( m_hWnd, nCmdShow );

	return S_OK;
}

bool WfWindow::Init()
{
	m_pD3D = new D3D( m_hWnd );
	if( nullptr == m_pD3D )
		return false;

	if( !m_pD3D->Init() )
		return false;

	return true;
}

int WfWindow::Run()
{
	MSG msg = { 0 };
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			PlayDemo();
		}
	}

	return static_cast<int>( msg.wParam );
}

void WfWindow::CleanUp()
{
	if( m_pD3D )
	{
		m_pD3D->CleanUp();
		delete m_pD3D;
		m_pD3D = nullptr;
	}
}

//------------------------------------------------------------
// 게임 루프
//------------------------------------------------------------
void WfWindow::PlayDemo()
{
	Update();
	RenderFrame();
}
// 게임로직 업데이트
void WfWindow::Update()
{
}

//------------------------------------------------------------
// 렌더링
//------------------------------------------------------------
void WfWindow::RenderFrame()
{
	m_pD3D->SetCurrent( 0 );
	m_pD3D->SetIAStage();

	m_pD3D->BegenScene( 0.0f, 0.125f, 0.3f, 1.0f );
	{
		RenderScene();
		RenderInfo();
	}
	m_pD3D->EndScene();
	m_pD3D->Present();
}

// 3D 물체등을 그린다.
void WfWindow::RenderScene()
{
	
	m_pD3D->SetModel();

	/*m_pD3D->SetRedShader();
	m_pD3D->DrawTriangle();*/

	// m_pD3D->SetColorModel();

	// m_pD3D->SetTextureModel();
}

// 디버그 정보 등을 출력
void WfWindow::RenderInfo()
{
}

LRESULT CALLBACK WfProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_KEYDOWN:
			ProcessInput( hWnd, wParam );
			break;

		case WM_DESTROY:
			if( g_pApp )
				g_pApp->CleanUp();

			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}

void ProcessInput( HWND hWnd, WPARAM key )
{
	switch( key )
	{
		case VK_ESCAPE:
			PostMessage( hWnd, WM_DESTROY, 0L, 0L );
			break;
	}
}
