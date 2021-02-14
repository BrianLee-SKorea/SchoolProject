#include "IShader.h"

namespace WindWolf
{
	HRESULT CompileShaderFromFile( const wchar_t * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlob )
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
}

IVertexShader::IVertexShader( ShaderParam _param )
	: param( _param )
{
}

IVertexShader::~IVertexShader()
{
	if( m_pIL )
	{
		m_pIL->Release();
		m_pIL = nullptr;
	}

	if( m_pVS )
	{
		m_pVS->Release();
		m_pVS = nullptr;
	}
}

bool IVertexShader::Init( ID3D11Device * pDevice )
{
	HRESULT hr = S_OK;

	ID3DBlob* pBlob = nullptr;
	hr = WindWolf::CompileShaderFromFile( param.filename.c_str(), param.entry.c_str(), param.model.c_str(), &pBlob );
	if( FAILED( hr ) )
	{
		std::string error = "Compile - VertexShader Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		if( pBlob ) pBlob->Release();
		return false;
	}

	hr = pDevice->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVS );
	if( FAILED( hr ) )
	{
		std::string error = "Create - VertexShader Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		if( pBlob ) pBlob->Release();
		return false;
	}

	if( !InitLayout( pDevice, pBlob ) )
	{
		if( pBlob )
			pBlob->Release();

		return false;
	}

	if( pBlob )
	{
		pBlob->Release();
		pBlob = nullptr;
	}

	return true;
}

ID3D11VertexShader * IVertexShader::GetShader() const
{
	return m_pVS;
}

ID3D11InputLayout * IVertexShader::GetLayout() const
{
	return m_pIL;
}

IPixelShader::IPixelShader( ShaderParam _param )
	: param( _param )
{
}

IPixelShader::~IPixelShader()
{
	if( m_pPS )
	{
		m_pPS->Release();
		m_pPS = nullptr;
	}
}

bool IPixelShader::Init( ID3D11Device * pDevice )
{
	HRESULT hr = S_OK;

	ID3DBlob* pBlob = nullptr;
	hr = WindWolf::CompileShaderFromFile( param.filename.c_str(), param.entry.c_str(), param.model.c_str(), &pBlob );
	if( FAILED( hr ) )
	{
		std::string error = "Compile - PixelShader Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		return false;
	}

	hr = pDevice->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPS );
	if( FAILED( hr ) )
	{
		std::string error = "Create - PixelShader Fail\t";
		error += param.entry + "\n";

		OutputDebugStringA( error.c_str() );
		return false;
	}

	if( pBlob )
	{
		pBlob->Release();
		pBlob = nullptr;
	}

	return true;
}

ID3D11PixelShader * IPixelShader::GetShader() const
{
	return m_pPS;
}
