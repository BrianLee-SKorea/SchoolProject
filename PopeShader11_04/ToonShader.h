#pragma once

#include "IShader.h"

class ToonVertexShader;
class ToonPixelShader;

class ToonShader
{
public:
	ToonShader();
	~ToonShader();

	bool Init( ID3D11Device* pDevice );
	void CleanUp();

	ID3D11VertexShader* GetVS() const;
	ID3D11InputLayout* GetIL() const;
	ID3D11PixelShader* GetPS() const;

	ConstantBuffer* m_pVCB = nullptr;
	ConstantBuffer* m_pPCB = nullptr;

private:
	ToonVertexShader* m_pVS = nullptr;
	ToonPixelShader* m_pPS = nullptr;
};

class ToonVertexShader
	: public IVertexShader
{
public:
	ToonVertexShader( ShaderParam _param );
	virtual ~ToonVertexShader() {}
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) override;
};

class ToonPixelShader
	: public IPixelShader
{
public:
	ToonPixelShader( ShaderParam _param );
	virtual ~ToonPixelShader() {}
};