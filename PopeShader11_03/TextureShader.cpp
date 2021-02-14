#include "TextureShader.h"

TextureShader::TextureShader()
{
	for( size_t i = 0; i < 3; ++i )
		m_apCB[ i ] = 0;
}

TextureShader::~TextureShader()
{
	CleanUp();
}

bool TextureShader::Init( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	ShaderParam vparam;
	vparam.filename = L"TextureShader.fx";
	vparam.entry = "vs_main";
	vparam.model = "vs_4_0";
	m_pVS = new TextureVertexShader( vparam );
	if( nullptr == m_pVS )
		return false;

	if( !m_pVS->Init( pDevice ) )
		return false;

	ShaderParam pparam;
	pparam.filename = L"TextureShader.fx";
	pparam.entry = "ps_main";
	pparam.model = "ps_4_0";
	m_pPS = new TexturePixelShader( pparam );
	if( nullptr == m_pPS )
		return false;

	if( !m_pPS->Init( pDevice ) )
		return false;

	UINT ConstantByteWidth[] = {
		sizeof( CBNeverChanges ),
		sizeof( CBChangeOnResize ),
		sizeof( CBChangeEveryFrame ),
	};

	for( size_t i = 0; i < 3; ++i )
	{
		m_apCB[ i ] = new ConstantBuffer( i );
		if( nullptr == m_apCB[ i ] )
			return false;

		if( !m_apCB[ i ]->Init( pDevice, ConstantByteWidth[ i ] ) )
			return false;
	}

	return true;
}

void TextureShader::CleanUp()
{
	for( size_t i = 0; i < 3; ++i )
	{
		if( m_apCB[ i ] )
		{
			delete m_apCB[ i ];
			m_apCB[ i ] = nullptr;
		}
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

ID3D11VertexShader * TextureShader::GetVS() const
{
	return m_pVS->GetShader();
}

ID3D11InputLayout * TextureShader::GetIL() const
{
	return m_pVS->GetLayout();
}

ID3D11PixelShader * TextureShader::GetPS() const
{
	return m_pPS->GetShader();
}

ConstantBuffer * TextureShader::GetCBNeverChange()
{
	return m_apCB[ 0 ];
}

ConstantBuffer * TextureShader::GetCBChangeOnResize()
{
	return m_apCB[ 1 ];
}

ConstantBuffer * TextureShader::GetCBChangeEveryFrame()
{
	return m_apCB[ 2 ];
}

TextureVertexShader::TextureVertexShader( ShaderParam _param )
	: IVertexShader( _param )
{
}

bool TextureVertexShader::InitLayout( ID3D11Device * pDevice, ID3DBlob * pBlob )
{
	D3D11_INPUT_ELEMENT_DESC layouts[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

TexturePixelShader::TexturePixelShader( ShaderParam _param )
	: IPixelShader( _param )
{
}
