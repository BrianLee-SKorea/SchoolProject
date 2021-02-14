#include "BasicShader.h"
#include "CustomVertexType.h"
#include "CustomConstantBufferType.h"

BasicShader::BasicShader()
{
}

BasicShader::~BasicShader()
{
}

bool BasicShader::Init( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	ShaderParam vparam;
	vparam.filename = L"BasicShader.fx";
	vparam.entry = "vs_main";
	vparam.model = "vs_5_0";
	m_pVS = new BasicVertexShader( vparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pVS->Init( pDevice ) )
		return false;

	ShaderParam pparam;
	pparam.filename = L"BasicShader.fx";
	pparam.entry = "ps_main";
	pparam.model = "ps_5_0";
	m_pPS = new BasicPixelShader( pparam );
	if( nullptr == m_pPS )
		return false;

	if( !m_pPS->Init( pDevice ) )
		return false;

	m_pCB = new ConstantBuffer();
	if( nullptr == m_pCB )
		return false;

	if( !m_pCB->Init( pDevice, sizeof( BasicConstantBuffer ) ) )
		return false;

	return true;
}

void BasicShader::CleanUp()
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

ID3D11VertexShader * BasicShader::GetVS() const
{
	return m_pVS->GetShader();
}

ID3D11InputLayout * BasicShader::GetIL() const
{
	return m_pVS->GetLayout();
}

ID3D11PixelShader * BasicShader::GetPS() const
{
	return m_pPS->GetShader();
}

ConstantBuffer * BasicShader::GetCB()
{
	return m_pCB;
}

BasicVertexShader::BasicVertexShader( ShaderParam _param )
	: IVertexShader( _param )
{
}

bool BasicVertexShader::InitLayout( ID3D11Device * pDevice, ID3DBlob * pBlob )
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

BasicPixelShader::BasicPixelShader( ShaderParam _param )
	: IPixelShader( _param )
{
}
