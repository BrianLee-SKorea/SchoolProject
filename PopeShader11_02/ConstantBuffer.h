#pragma once

#pragma comment( lib, "d3d11.lib" )

#include <d3d11.h>

#include <malloc.h>

struct ConstantBuffer
{
	ConstantBuffer( UINT _slot = 0);
	~ConstantBuffer();

	bool Init( ID3D11Device* pDevice, 
		UINT ByteWidth, 
		D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, 
		UINT CPUAccessFlags = 0, 
		UINT MiscFlags = 0, 
		UINT StructureByteStride = 0
	);

	ID3D11Buffer* pBuffer = nullptr;
	UINT slot;
};