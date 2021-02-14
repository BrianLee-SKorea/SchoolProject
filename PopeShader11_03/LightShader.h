#pragma once

#include "IShader.h"

class LightVertexShader;
class LightPixelShader;

class LightShader
{
public:
	LightShader();
	~LightShader();

	bool Init( ID3D11Device* pDevice );
	void CleanUp();

	ID3D11VertexShader* GetVS() const;
	ID3D11InputLayout* GetIL() const;
	ID3D11PixelShader* GetPS() const;

	ConstantBuffer* GetCB();

private:
	LightVertexShader* m_pVS = nullptr;
	LightPixelShader* m_pPS = nullptr;

	ConstantBuffer* m_pCB = nullptr;
};

class LightVertexShader
	: public IVertexShader
{
public:
	LightVertexShader( ShaderParam _param );
	virtual ~LightVertexShader() {}
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) override;
};

class LightPixelShader
	: public IPixelShader
{
public:
	LightPixelShader( ShaderParam _param );
	virtual ~LightPixelShader() {}
};