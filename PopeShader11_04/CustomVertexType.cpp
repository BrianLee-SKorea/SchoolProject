#include "CustomVertexType.h"


const D3D11_INPUT_ELEMENT_DESC BasicVertex::InputElements[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

BasicVertex::BasicVertex( XMFLOAT3 const & _pos, XMFLOAT3 const & _normal, XMFLOAT2 const & _tex )
	: pos( _pos ), normal( _normal ), tex( _tex )
{
}

BasicVertex::BasicVertex( FXMVECTOR _pos, FXMVECTOR _normal, FXMVECTOR _tex )
{
	XMStoreFloat3( &pos, _pos );
	XMStoreFloat3( &normal, _normal );
	XMStoreFloat2( &tex, _tex );
}
