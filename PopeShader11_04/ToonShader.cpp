#include "ToonShader.h"

ToonShader::ToonShader()
{
}

ToonShader::~ToonShader()
{
	CleanUp();
}

bool ToonShader::Init( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	ShaderParam vparam;
	vparam.filename = L"ToonShader.fx";
	vparam.entry = "vs_main";
	vparam.model = "vs_5_0";
	m_pVS = new ToonVertexShader( vparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pVS->Init( pDevice ) )
		return false;

	ShaderParam pparam;
	pparam.filename = L"ToonShader.fx";
	pparam.entry = "ps_main";
	pparam.model = "ps_5_0";
	m_pPS = new ToonPixelShader( pparam );
	if( nullptr == m_pPS )
		return false;

	if( !m_pPS->Init( pDevice ) )
		return false;

	m_pVCB = new ConstantBuffer();
	if( nullptr == m_pVCB )
		return false;

	if( !m_pVCB->Init( pDevice, sizeof( ToonConstantBuffer ) ) )
		return false;

	return true;
}

void ToonShader::CleanUp()
{
	if( m_pVCB )
	{
		delete m_pVCB;
		m_pVCB = nullptr;
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

ID3D11VertexShader * ToonShader::GetVS() const
{
	return m_pVS->GetShader();
}

ID3D11InputLayout * ToonShader::GetIL() const
{
	return m_pVS->GetLayout();
}

ID3D11PixelShader * ToonShader::GetPS() const
{
	return m_pPS->GetShader();
}

ToonVertexShader::ToonVertexShader( ShaderParam _param )
	: IVertexShader( _param )
{
}

bool ToonVertexShader::InitLayout( ID3D11Device * pDevice, ID3DBlob * pBlob )
{
	HRESULT hr = pDevice->CreateInputLayout( BasicVertex::InputElements, BasicVertex::InputElementCount, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pIL );
	if( FAILED( hr ) )
	{
		std::string error = "Create - InputLayout Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		return false;
	}

	return true;
}

ToonPixelShader::ToonPixelShader( ShaderParam _param )
	: IPixelShader( _param )
{
}
