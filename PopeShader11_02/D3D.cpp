#include "D3D.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "ModelCube.h"

#define SAFE_RELEASE( x ) if( x ) { x->Release(); x = nullptr; }
#define SAFE_CLEANUP( x ) if( x ) { x->CleanUp(); delete x; x = nullptr; }
#define SAFE_DELETE( x ) if( x ) { delete x; x = nullptr; }

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

	if( !InitDepthStencilView() )
		return false;

	if( !InitRenderTargetView() )
		return false;

	if( !InitSamplerState() )
		return false;

	if( !LoadTextureFromFile() )
		return false;

	if( !InitModel() )
		return false;

	InitMatrix();
	
	if( !InitShaders() )
		return false;
	InitTextureConstantBuffer();

	return true;
}

void D3D::CleanUp()
{
	SAFE_CLEANUP( m_pColorShader );
	SAFE_CLEANUP( m_pTextureShader );

	SAFE_DELETE( m_pColorCube );
	SAFE_DELETE( m_pTextureCube );

	if( m_pDeviceContext )
		m_pDeviceContext->ClearState();

	SAFE_RELEASE( m_pTextureResourceView );
	SAFE_RELEASE( m_pSamplerLinear );

	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pDepthStencilView );
	SAFE_RELEASE( m_pDepthStencil );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
}

void D3D::BegenScene( float r, float g, float b, float a )
{
	float clearColor[] = { r, g, b, a };
	m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, clearColor );
	m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void D3D::EndScene()
{
}

void D3D::Present()
{
	m_pSwapChain->Present( 0, 0 );
}

void D3D::DrawTriangle()
{
	m_pDeviceContext->Draw( 3, 0 );
}

void D3D::SetCurrent( size_t current )
{
	m_current = current;
}

void D3D::SetIAStage()
{
	UINT stride = m_pColorCube->GetStride();
	UINT offset = 0;

	switch( m_current )
	{
		case 0:
			stride = m_pColorCube->GetStride();
			offset = 0;

			m_pDeviceContext->IASetInputLayout( m_pColorShader->GetIL() );

			m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pColorCube->m_pVB, &stride, &offset );
			m_pDeviceContext->IASetIndexBuffer( m_pColorCube->m_pIB, DXGI_FORMAT_R16_UINT, 0 );
			m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			break;

		case 1:
			stride = m_pTextureCube->GetStride();
			offset = 0;

			m_pDeviceContext->IASetInputLayout( m_pTextureShader->GetIL() );

			m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pTextureCube->m_pVB, &stride, &offset );
			m_pDeviceContext->IASetIndexBuffer( m_pTextureCube->m_pIB, DXGI_FORMAT_R16_UINT, 0 );
			m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			break;
	}
}

void D3D::SetModel()
{
	switch( m_current )
	{
		case 0:
			SetColorModel();
			break;

		case 1:
			SetTextureModel();
			break;
	}
}

void D3D::SetColorModel()
{
	static float rotateY = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if( dwTimeStart == 0 )
		dwTimeStart = dwTimeCur;
	rotateY = ( dwTimeCur - dwTimeStart ) / 1000.0f;

	// 1st Cube
	m_world = XMMatrixRotationY( rotateY );

	// 2nd Cube
	XMMATRIX spin = XMMatrixRotationZ( -rotateY );
	XMMATRIX orbit = XMMatrixRotationY( -rotateY * 2.0f );
	XMMATRIX translate = XMMatrixTranslation( -4.0f, 0.0f, 0.0f );
	XMMATRIX scale = XMMatrixScaling( 0.3f, 0.3f, 0.3f );

	m_world2 = scale * spin * translate * orbit;

	// Set Shader
	m_pDeviceContext->VSSetShader( m_pColorShader->GetVS(), nullptr, 0 );
	m_pDeviceContext->VSSetConstantBuffers( 0, 1, &m_pColorShader->GetCB()->pBuffer );

	m_pDeviceContext->PSSetShader( m_pColorShader->GetPS(), nullptr, 0 );

	// 1st Cube Render
	ColorConstantBuffer ccb;
	ccb.world = XMMatrixTranspose( m_world );
	ccb.view = XMMatrixTranspose( m_view );
	ccb.projection = XMMatrixTranspose( m_projection );

	m_pDeviceContext->UpdateSubresource( m_pColorShader->GetCB()->pBuffer, 0, nullptr, &ccb, 0, 0 );
	m_pDeviceContext->DrawIndexed( 36, 0, 0 );

	// 2nd Cube Render
	ColorConstantBuffer ccb2;
	ccb2.world = XMMatrixTranspose( m_world2 );
	ccb2.view = XMMatrixTranspose( m_view );
	ccb2.projection = XMMatrixTranspose( m_projection );

	m_pDeviceContext->UpdateSubresource( m_pColorShader->GetCB()->pBuffer, 0, nullptr, &ccb2, 0, 0 );
	 m_pDeviceContext->DrawIndexed( 36, 0, 0 );
}

void D3D::SetTextureModel()
{
	static float rotateY = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if( dwTimeStart == 0 )
		dwTimeStart = dwTimeCur;
	rotateY = ( dwTimeCur - dwTimeStart ) / 1000.0f;

	m_world = XMMatrixRotationY( rotateY );

	CBChangeEveryFrame cb;
	cb.world = XMMatrixTranspose( m_world );
	m_pDeviceContext->UpdateSubresource( m_pTextureShader->GetCBChangeEveryFrame()->pBuffer, 0, nullptr, &cb, 0, 0 );

	m_pDeviceContext->VSSetShader( m_pTextureShader->GetVS(), nullptr, 0 );
	m_pDeviceContext->VSSetConstantBuffers( 0, 1, &m_pTextureShader->GetCBNeverChange()->pBuffer );
	m_pDeviceContext->VSSetConstantBuffers( 1, 1, &m_pTextureShader->GetCBChangeOnResize()->pBuffer );
	m_pDeviceContext->VSSetConstantBuffers( 2, 1, &m_pTextureShader->GetCBChangeEveryFrame()->pBuffer );

	m_pDeviceContext->PSSetShader( m_pTextureShader->GetPS(), nullptr, 0 );
	m_pDeviceContext->PSSetConstantBuffers( 2, 1, &m_pTextureShader->GetCBChangeEveryFrame()->pBuffer );
	m_pDeviceContext->PSSetShaderResources( 0, 1, &m_pTextureResourceView );
	m_pDeviceContext->PSSetSamplers( 0, 1, &m_pSamplerLinear );

	m_pDeviceContext->DrawIndexed( 36, 0, 0 );
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

bool D3D::InitDepthStencilView()
{
	HRESULT hr = S_OK;

	// CREATE DEPTH STENCIL TEXTURE
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D( &descDepth, nullptr, &m_pDepthStencil );
	if( FAILED( hr ) )
		return false;

	// CREATE DEPTH STENCIL VIEW
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
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

	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

	// SETUP VIEWPORT
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = static_cast<FLOAT>( m_width );
	vp.Height = static_cast<FLOAT>( m_height );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pDeviceContext->RSSetViewports( 1, &vp );

	if( pBackBuffer )
		pBackBuffer->Release();

	return true;
}

bool D3D::InitSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	HRESULT hr = m_pDevice->CreateSamplerState( &samplerDesc, &m_pSamplerLinear );
	if( FAILED( hr ) )
		return false;

	return true;
}

void D3D::InitMatrix()
{
	m_world = XMMatrixIdentity();
	
	XMVECTOR Eye = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_view = XMMatrixLookAtLH( Eye, At, Up );

	m_projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, m_width / (float)m_height, 0.01f, 100.0f );
}

bool D3D::InitShaders()
{
	m_pColorShader = new ColorShader();
	if( !m_pColorShader )
		return false;
	if( !m_pColorShader->Init( m_pDevice ) )
		return false;

	m_pTextureShader = new TextureShader();
	if( !m_pTextureShader )
		return false;
	if( !m_pTextureShader->Init( m_pDevice ) )
		return false;

	return true;
}

void D3D::InitTextureConstantBuffer()
{
	CBNeverChanges cb1;
	cb1.view = XMMatrixTranspose( m_view );
	m_pDeviceContext->UpdateSubresource( m_pTextureShader->GetCBNeverChange()->pBuffer, 0, nullptr, &cb1, 0, 0 );

	CBChangeOnResize cb2;
	cb2.projection = XMMatrixTranspose( m_projection );
	m_pDeviceContext->UpdateSubresource( m_pTextureShader->GetCBChangeOnResize()->pBuffer, 0, nullptr, &cb2, 0, 0 );
}

bool D3D::InitModel()
{
	if( !InitColorCube() )
		return false;

	if( !InitTextureCube() )
		return false;

	return true;
}

bool D3D::InitColorCube()
{
	m_pColorCube = new ColorCube;
	if( !m_pColorCube )
		return false;
	if( !m_pColorCube->InitModel( m_pDevice ) )
		return false;

	return true;
}

bool D3D::InitTextureCube()
{
	m_pTextureCube = new TextureCube;
	if( !m_pTextureCube )
		return false;
	if( !m_pTextureCube->InitModel( m_pDevice ) )
		return false;

	return true;
}

bool D3D::LoadTextureFromFile()
{
	HRESULT hr = CreateDDSTextureFromFile( m_pDevice, L"sword.dds", nullptr, &m_pTextureResourceView );
	if( FAILED( hr ) )
		return false;

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