#pragma once

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

struct PositionVertex
{
	XMFLOAT3 pos;
};

class D3D
{
public:
	D3D( HWND hWnd );
	~D3D();

	bool Init();
	void CleanUp();

	void BegenScene( float r, float g, float b, float a );
	void EndScene();
	void Present();

	void SetRedShader();
	void DrawTriangle();

private:
	bool InitDeviceAndSwapChain();
	bool InitRenderTargetView();
	bool InitRedShader();
	
	bool InitModel();
	bool InitTriangle();

	HRESULT CompileShaderFromFile( const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlob );

private:
	HWND m_hWnd;
	UINT m_width;
	UINT m_height;

private:

	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3D11Buffer* m_pTriangleBuffer = nullptr;
};