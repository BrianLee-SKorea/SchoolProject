#include "LightShader.h"

LightShader::LightShader()
{
}

LightShader::~LightShader()
{
	CleanUp();
}

bool LightShader::Init( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	ShaderParam vparam;
	vparam.filename = L"LightShader.fx";
	vparam.entry = "vs_main";
	vparam.model = "vs_5_0";
	m_pVS = new LightVertexShader( vparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pVS->Init( pDevice ) )
		return false;

	ShaderParam pparam;
	pparam.filename = L"LightShader.fx";
	pparam.entry = "ps_main";
	pparam.model = "ps_5_0";
	m_pPS = new LightPixelShader( pparam );
	if( nullptr == m_pPS )
		return false;

	if( !m_pPS->Init( pDevice ) )
		return false;

	m_pCB = new ConstantBuffer();
	if( nullptr == m_pCB )
		return false;

	if( !m_pCB->Init( pDevice, sizeof( LightConstantBuffer ) ) )
		return false;

	return true;
}

void LightShader::CleanUp()
{
	if( m_pCB )
	{
		delete m_pCB;
		m_pCB = nullptr;
	}

	if( m_pPS )
	{
		delete m_pPS;
		m_pPS = nullptr;
	}

	if( m_pVS )
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
}

ID3D11VertexShader * LightShader::GetVS() const
{
	return m_pVS->GetShader();
}

ID3D11InputLayout * LightShader::GetIL() const
{
	return m_pVS->GetLayout();
}

ID3D11PixelShader * LightShader::GetPS() const
{
	return m_pPS->GetShader();
}

ConstantBuffer * LightShader::GetCB()
{
	return m_pCB;
}

LightVertexShader::LightVertexShader( ShaderParam _param )
	: IVertexShader( _param )
{
}

bool LightVertexShader::InitLayout( ID3D11Device * pDevice, ID3DBlob * pBlob )
{
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numLayout = ARRAYSIZE( layouts );

	HRESULT hr = pDevice->CreateInputLayout( layouts, numLayout, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pIL );
	if( FAILED( hr ) )
	{
		std::string error = "Create - InputLayout Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		return false;
	}

	return true;
}

LightPixelShader::LightPixelShader( ShaderParam _param )
	: IPixelShader( _param )
{
}
