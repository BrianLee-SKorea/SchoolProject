#pragma once

#include "IModel.h"

class ColorCube
	: public IModel
{
	virtual bool InitVertexBuffer( ID3D11Device* pDevice ) override;
	virtual bool InitIndexBuffer( ID3D11Device* pDevice ) override;

public:
	virtual UINT GetStride() const override;
};

class TextureCube
	: public IModel
{
	virtual bool InitVertexBuffer( ID3D11Device* pDevice ) override;
	virtual bool InitIndexBuffer( ID3D11Device* pDevice ) override;

public:
	virtual UINT GetStride() const override;
};

class LightCube
	: public IModel
{
	virtual bool InitVertexBuffer( ID3D11Device* pDevice ) override;
	virtual bool InitIndexBuffer( ID3D11Device* pDevice ) override;

public:
	virtual UINT GetStride() const override;
};