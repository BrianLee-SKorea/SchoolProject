#pragma once

#pragma comment( lib, "d3d11.lib" )

#include <d3d11.h>
#include <DirectXMath.h>

#include "CustomVertexType.h"

struct IModel
{
	IModel();
	virtual ~IModel();

	bool InitModel( ID3D11Device* pDevice );
	
protected:
	virtual bool InitVertexBuffer( ID3D11Device* pDevice ) = 0;
	virtual bool InitIndexBuffer( ID3D11Device* pDevice ) = 0;

public:
	virtual UINT GetStride() const = 0;

	ID3D11Buffer* m_pVB = nullptr;
	ID3D11Buffer* m_pIB = nullptr;
};