#pragma once

#include "IShader.h"

class ColorVertexShader;
class ColorPixelShader;

class ColorShader
{
public:
	ColorShader();
	~ColorShader();

	bool Init( ID3D11Device* pDevice );
	void CleanUp();

	ID3D11VertexShader* GetVS() const;
	ID3D11InputLayout* GetIL() const;
	ID3D11PixelShader* GetPS() const;

	ConstantBuffer* GetCB();

private:
	ColorVertexShader* m_pVS = nullptr;
	ColorPixelShader* m_pPS = nullptr;

	ConstantBuffer* m_pCB = nullptr;
};


class ColorVertexShader
	: public IVertexShader
{
public:
	ColorVertexShader( ShaderParam _param );
	virtual ~ColorVertexShader() {}
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) override;
};

class ColorPixelShader
	: public IPixelShader
{
public:
	ColorPixelShader( ShaderParam _param );
	virtual ~ColorPixelShader() {}
};