#include "ModelSphere.h"
#include "CustomVertexType.h"

void ComputeSphere( std::vector<BasicVertex>& vertices, std::vector<WORD>& indices, float diameter, float tessellation, bool rhcoords, bool invertn )
{
	vertices.clear();
	indices.clear();

	if( tessellation < 3 )
		throw std::printf( "tesselation parameter out of range" );

	size_t verticalSegments = static_cast<size_t>( tessellation );
	size_t horizontalSegments = static_cast<size_t>( tessellation * 2 );

	float radius = diameter / 2.0f;

	for( size_t i = 0; i <= verticalSegments; ++i )
	{
		float v = 1 - float( i ) / verticalSegments;

		float latitude = ( i * XM_PI / verticalSegments ) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos( &dy, &dxz, latitude );

		for( size_t j = 0; j <= horizontalSegments; ++j )
		{
			float u = float( j ) / horizontalSegments;

			float longitude = j * XM_2PI / horizontalSegments;
			float dx, dz;

			XMScalarSinCos( &dx, &dz, longitude );

			dx *= dxz;
			dz *= dxz;

			XMVECTOR normal = XMVectorSet( dx, dy, dz, 0.0f );
			XMVECTOR textureCoordinate = XMVectorSet( u, v, 0.0f, 0.0f );

			BasicVertex vertex( XMVectorScale( normal, radius ), normal, textureCoordinate );
			vertices.emplace_back( vertex );
		}
	}

	size_t stride = horizontalSegments + 1;

	for( size_t i = 0; i < verticalSegments; ++i )
	{
		for( size_t j = 0; j <= horizontalSegments; ++j )
		{
			size_t nextI = i + 1;
			size_t nextJ = ( j + 1 ) % stride;

			WORD p1 = static_cast<WORD>( i * stride + j );
			WORD p2 = static_cast<WORD>( nextI * stride + j );
			WORD p3 = static_cast<WORD>( i * stride + nextJ );
			WORD p4 = static_cast<WORD>( nextI * stride + nextJ );

			indices.push_back( p1 );
			indices.push_back( p2 );
			indices.push_back( p3 );

			indices.push_back( p3 );
			indices.push_back( p2 );
			indices.push_back( p4 );
		}
	}

	if( !rhcoords )
	{
		for( auto it = indices.begin(); it != indices.end(); it += 3 )
		{
			std::swap( *it, *( it + 2 ) );
		}

		for( auto it = vertices.begin(); it != vertices.end(); ++it )
		{
			it->tex.x = ( 1.f - it->tex.x );
		}
	}

	if( !invertn )
	{
		for( auto it = vertices.begin(); it != vertices.end(); ++it )
		{
			it->normal.x = -it->normal.x;
			it->normal.y = -it->normal.y;
			it->normal.z = -it->normal.z;
		}
	}
}

bool BasicSphere::InitSphere()
{
	ComputeSphere( vertices, indices, diameter, tessellation, rhcoords, invertn );
	return true;
}

bool BasicSphere::InitVertexBuffer( ID3D11Device * pDevice )
{
	D3D11_BUFFER_DESC vbd;
	ZeroMemory( &vbd, sizeof( vbd ) );
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof( BasicVertex ) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vid;
	ZeroMemory( &vid, sizeof( vid ) );
	vid.pSysMem = vertices.data();

	HRESULT hr = pDevice->CreateBuffer( &vbd, &vid, &m_pVB );
	if( FAILED( hr ) )
		return false;

	return true;
}

bool BasicSphere::InitIndexBuffer( ID3D11Device * pDevice )
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory( &ibd, sizeof( ibd ) );
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof( WORD ) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iid;
	ZeroMemory( &iid, sizeof( iid ) );
	iid.pSysMem = indices.data();

	HRESULT hr = pDevice->CreateBuffer( &ibd, &iid, &m_pIB );
	if( FAILED( hr ) )
		return false;

	return true;
}

UINT BasicSphere::GetStride() const
{
	return sizeof( BasicVertex );
}

WORD BasicSphere::GetIndexCount() const
{
	return static_cast<WORD>( indices.size() );
}
