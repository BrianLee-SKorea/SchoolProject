#pragma once

#include "IModel.h"
#include <vector>

struct BasicVertex;

void ComputeSphere( std::vector< BasicVertex >& vertices, std::vector< WORD >& indices, float diameter, float tessellation, bool rhcoords, bool invertn );

class BasicSphere
	: public IModel
{
	virtual bool InitVertexBuffer( ID3D11Device* pDevice ) override;
	virtual bool InitIndexBuffer( ID3D11Device* pDevice ) override;

public:
	bool InitSphere();
	virtual UINT GetStride() const override;
	WORD GetIndexCount() const;

private:
	std::vector< BasicVertex > vertices;
	std::vector< WORD > indices;
	float diameter = 1;
	float tessellation = 16;
	bool rhcoords = false;
	bool invertn = false;
};