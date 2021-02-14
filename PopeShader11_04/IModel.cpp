#include "IModel.h"

IModel::IModel()
{
}

IModel::~IModel()
{
	if( m_pIB )
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}

	if( m_pVB )
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}
}

bool IModel::InitModel( ID3D11Device * pDevice )
{
	if( nullptr == pDevice )
		return false;

	if( !InitVertexBuffer( pDevice ) )
		return false;

	if( !InitIndexBuffer( pDevice ) )
		return false;

	return true;
}
