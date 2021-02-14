#pragma once

#include <memory>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#define ALIGN_16BIT						\
void* operator new( size_t size )		\
{										\
	return _mm_malloc( size, 16 );		\
}										\
\
void operator delete( void* memory )	\
{										\
	_mm_free( memory );					\
}										\

struct ColorVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;

	ALIGN_16BIT;
};

struct TextureVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;

	ALIGN_16BIT;
};

struct LightVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;

	ALIGN_16BIT;
};

struct BasicVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;

	BasicVertex() = default;

	BasicVertex( const BasicVertex& ) = default;
	BasicVertex& operator=( const BasicVertex& ) = default;

	BasicVertex( BasicVertex&& ) = default;
	BasicVertex& operator=( BasicVertex&& ) = default;

	BasicVertex( XMFLOAT3 const& _pos, XMFLOAT3 const& _normal, XMFLOAT2 const& _tex );
	BasicVertex( FXMVECTOR _pos, FXMVECTOR _normal, FXMVECTOR _tex );

	static const int InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[ InputElementCount ];
};