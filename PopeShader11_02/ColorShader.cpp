#include "ColorShader.h"

ColorShader::ColorShader()
{
}

ColorShader::~ColorShader()
{
	CleanUp();
}

bool ColorShader::Init( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	ShaderParam vparam;
	vparam.filename = L"ColorShader.fx";
	vparam.entry = "vs_main";
	vparam.model = "vs_5_0";
	m_pVS = new ColorVertexShader( vparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pVS->Init( pDevice ) )
		return false;

	ShaderParam pparam;
	pparam.filename = L"ColorShader.fx";
	pparam.entry = "ps_main";
	pparam.model = "ps_5_0";
	m_pPS = new ColorPixelShader( pparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pPS->Init( pDevice ) )
		return false;

	m_pCB = new ConstantBuffer();
	if( nullptr == m_pCB )
		return false;

	if( !m_pCB->Init( pDevice, sizeof( ColorConstantBuffer ) ) )
		return false;

	return true;
}

void ColorShader::CleanUp()
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

ID3D11VertexShader * ColorShader::GetVS() const
{
	return m_pVS->GetShader();
}

ID3D11InputLayout * ColorShader::GetIL() const
{
	return m_pVS->GetLayout();
}

ID3D11PixelShader * ColorShader::GetPS() const
{
	return m_pPS->GetShader();
}

ConstantBuffer * ColorShader::GetCB()
{
	return m_pCB;
}

ColorVertexShader::ColorVertexShader( ShaderParam _param )
	: IVertexShader( _param )
{
}

bool ColorVertexShader::InitLayout( ID3D11Device * pDevice, ID3DBlob * pBlob )
{
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

ColorPixelShader::ColorPixelShader( ShaderParam _param )
	: IPixelShader( _param )
{
}
