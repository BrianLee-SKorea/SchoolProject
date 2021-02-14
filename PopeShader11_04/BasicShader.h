#pragma once

#include "IShader.h"

class BasicVertexShader;
class BasicPixelShader;

class BasicShader
{
public:
	BasicShader();
	~BasicShader();

	bool Init( ID3D11Device* pDevice );
	void CleanUp();

	ID3D11VertexShader* GetVS() const;
	ID3D11InputLayout* GetIL() const;
	ID3D11PixelShader* GetPS() const;

	ConstantBuffer* GetCB();

private:
	BasicVertexShader* m_pVS = nullptr;
	BasicPixelShader* m_pPS = nullptr;

	ConstantBuffer* m_pCB = nullptr;
};


class BasicVertexShader
	: public IVertexShader
{
public:
	BasicVertexShader( ShaderParam _param );
	virtual ~BasicVertexShader() {}
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) override;
};

class BasicPixelShader
	: public IPixelShader
{
public:
	BasicPixelShader( ShaderParam _param );
	virtual ~BasicPixelShader() {}
};