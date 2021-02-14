#pragma once

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "DirectXTK" )

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Include/DDSTextureLoader.h"
#include "Include/WICTextureLoader.h"

#include "CustomConstantBufferType.h"
#include "CustomVertexType.h"

// Shader Forward Declaration
class ColorShader;
class TextureShader;

class ColorCube;
class TextureCube;

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

	void DrawTriangle();

	void SetCurrent( size_t current );
	void SetIAStage();
	void SetModel();
	void SetColorModel();
	void SetTextureModel();

	ALIGN_16BIT;

private:
	bool InitDeviceAndSwapChain();
	bool InitDepthStencilView();
	bool InitRenderTargetView();

	bool InitSamplerState();
	void InitMatrix();

	bool InitShaders();
	void InitTextureConstantBuffer();

	bool InitModel();
	bool InitColorCube();
	bool InitTextureCube();

	bool LoadTextureFromFile();

	HRESULT CompileShaderFromFile( const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlob );

private:
	HWND m_hWnd;
	UINT m_width;
	UINT m_height;

private:
	size_t m_current = 0;

	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D11_PRIMITIVE_TOPOLOGY m_topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11Texture2D* m_pDepthStencil = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	// TEXTURE
	ID3D11ShaderResourceView* m_pTextureResourceView = nullptr;
	ID3D11SamplerState* m_pSamplerLinear = nullptr;

	XMMATRIX m_world, m_world2;
	XMMATRIX m_view;
	XMMATRIX m_projection;

	// Custom Shader Class
	ColorShader* m_pColorShader = nullptr;
	TextureShader* m_pTextureShader = nullptr;

	ColorCube* m_pColorCube = nullptr;
	TextureCube* m_pTextureCube = nullptr;
};