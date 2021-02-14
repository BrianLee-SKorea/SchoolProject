#include "D3D.h"

#define SAFE_RELEASE( x ) if( x ) { x->Release(); x = nullptr; }

D3D::D3D( HWND hWnd )
	: m_hWnd( hWnd )
{
}

D3D::~D3D()
{
	CleanUp();
}

bool D3D::Init()
{
	if( !InitDeviceAndSwapChain() )
		return false;

	if( !InitRenderTargetView() )
		return false;

	if( !InitRedShader() )
		return false;

	if( !InitModel() )
		return false;

	return true;
}

void D3D::CleanUp()
{
	if( m_pDeviceContext )
		m_pDeviceContext->ClearState();

	SAFE_RELEASE( m_pTriangleBuffer );
	SAFE_RELEASE( m_pInputLayout );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexShader );

	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
}

void D3D::BegenScene( float r, float g, float b, float a )
{
	float clearColor[] = { r, g, b, a };
	m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, clearColor );
}

void D3D::EndScene()
{
}

void D3D::Present()
{
	m_pSwapChain->Present( 0, 0 );
}

void D3D::SetRedShader()
{
	m_pDeviceContext->VSSetShader( m_pVertexShader, nullptr, 0 );
	m_pDeviceContext->PSSetShader( m_pPixelShader, nullptr, 0 );
}

void D3D::DrawTriangle()
{
	m_pDeviceContext->Draw( 3, 0 );
}

bool D3D::InitDeviceAndSwapChain()
{
	HRESULT hr = S_OK;

	RECT rc = { 0 };
	GetClientRect( m_hWnd, &rc );
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	::ZeroMemory( &sd, sizeof( sd ) );

	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		m_driverType = driverTypes[ driverTypeIndex ];
		hr = D3D11CreateDeviceAndSwapChain( nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &m_featureLevel, &m_pDeviceContext );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED( hr ) )
		return false;

	return true;
}

bool D3D::InitRenderTargetView()
{
	HRESULT hr = S_OK;

	// CREATE RENDER TARGET VIEW
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
	if( FAILED( hr ) )
		return false;

	hr = m_pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
	if( FAILED( hr ) )
		return false;

	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, nullptr );

	// SETUP VIEWPORT
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = static_cast<FLOAT>( m_width );
	vp.Height = static_cast<FLOAT>( m_height );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pDeviceContext->RSSetViewports( 1, &vp );

	return true;
}

bool D3D::InitRedShader()
{
	HRESULT hr = S_OK;

#pragma region VERTEX_SHADER
	{
		ID3DBlob* pBlob = nullptr;
		hr = CompileShaderFromFile( L"RedShader.fx", "vs_main", "vs_4_0", &pBlob );
		if( FAILED( hr ) )
		{
			OutputDebugStringA( "Compile VertexShader Fail\n" );
			return false;
		}

		hr = m_pDevice->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader );
		if( FAILED( hr ) )
		{
			OutputDebugStringA( "Create VertexShader Fail\n" );
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC layouts[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numLayout = ARRAYSIZE( layouts );
		hr = m_pDevice->CreateInputLayout( layouts, numLayout, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout );
		
		pBlob->Release();

		if( FAILED( hr ) )
		{
			OutputDebugStringA( "Create InputLayout Fail\n" );
			return false;
		}

		m_pDeviceContext->IASetInputLayout( m_pInputLayout );
	}
#pragma endregion

#pragma region PIXEL_SHADER
	{
		ID3DBlob* pBlob = nullptr;
		hr = CompileShaderFromFile( L"RedShader.fx", "ps_main", "ps_4_0", &pBlob );
		if( FAILED( hr ) )
		{
			OutputDebugStringA( "Compile PixelShader Fail\n" );
			return false;
		}

		hr = m_pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader );
		pBlob->Release();

		if( FAILED( hr ) )
		{
			OutputDebugStringA( "Create PixelShader Fail\n" );
			return false;
		}
	}
#pragma endregion

	return true;
}

bool D3D::InitModel()
{
	if( !InitTriangle() )
		return false;

	return true;
}

bool D3D::InitTriangle()
{
	PositionVertex vertices[] = {
		XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( PositionVertex ) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA id;
	ZeroMemory( &id, sizeof( id ) );
	id.pSysMem = vertices;

	HRESULT hr = m_pDevice->CreateBuffer( &bd, &id, &m_pTriangleBuffer );
	if( FAILED( hr ) )
	{
		OutputDebugStringA( "Triangle Init Fail\n" );
		return false;
	}

	UINT stride = sizeof( PositionVertex );
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pTriangleBuffer, &stride, &offset );
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return true;
}

HRESULT D3D::CompileShaderFromFile( const wchar_t * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlob )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 0, 0, ppBlob, &pErrorBlob );
	if( FAILED( hr ) )
	{
		if( pErrorBlob )
		{
			OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			pErrorBlob->Release();
			pErrorBlob = nullptr;
		}

		return hr;
	}

	if( pErrorBlob )
		pErrorBlob->Release();

	return hr;
}