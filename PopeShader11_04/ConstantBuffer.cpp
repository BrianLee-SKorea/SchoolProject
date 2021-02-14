#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer( UINT _slot )
	: slot( _slot )
{
}

ConstantBuffer::~ConstantBuffer()
{
	if( pBuffer )
	{
		pBuffer->Release();
		pBuffer = nullptr;
	}
}

bool ConstantBuffer::Init( ID3D11Device * pDevice, UINT ByteWidth, D3D11_USAGE Usage, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride )
{
	if( !pDevice )
		return false;

	UINT value = ByteWidth / 16;
	UINT remain = ByteWidth % 16;
	if( remain > 0 )
		ByteWidth = ( value + 1 ) * 16;

	D3D11_BUFFER_DESC bd;
	::ZeroMemory( &bd, sizeof( bd ) );

	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = Usage;
	bd.ByteWidth = ByteWidth;
	bd.CPUAccessFlags = CPUAccessFlags;
	bd.MiscFlags = MiscFlags;
	bd.StructureByteStride = StructureByteStride;
	
	HRESULT hr = pDevice->CreateBuffer( &bd, nullptr, &pBuffer );
	if( FAILED( hr ) )
		return false;

	return true;
}
