#pragma once

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>

#include "CustomVertexType.h"
#include "CustomConstantBufferType.h"
#include "ConstantBuffer.h"

namespace WindWolf
{
	HRESULT CompileShaderFromFile( const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlob );
}

struct ShaderParam
{
	std::wstring filename;
	std::string entry;
	std::string model;
};

struct IVertexShader
{
	IVertexShader( ShaderParam _param );
	virtual ~IVertexShader();

	bool Init( ID3D11Device* pDevice );
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) = 0;

	ID3D11VertexShader* GetShader() const;
	ID3D11InputLayout* GetLayout() const;

	// VARIABLE
	ID3D11VertexShader* m_pVS = nullptr;
	ID3D11InputLayout* m_pIL = nullptr;

	ShaderParam param;
};

struct IPixelShader
{
	IPixelShader( ShaderParam _param );
	virtual ~IPixelShader();

	bool Init( ID3D11Device* pDevice );

	ID3D11PixelShader* GetShader() const;

	// VARIABLE
	ID3D11PixelShader* m_pPS = nullptr;
	ShaderParam param;
};