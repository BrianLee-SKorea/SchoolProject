#pragma once

#include <memory>

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