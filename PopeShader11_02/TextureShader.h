#pragma once

#include "IShader.h"

class TextureVertexShader;
class TexturePixelShader;

class TextureShader
{
public:
	TextureShader();
	~TextureShader();
	
	bool Init( ID3D11Device* pDevice );
	void CleanUp();

	ID3D11VertexShader* GetVS() const;
	ID3D11InputLayout* GetIL() const;
	ID3D11PixelShader* GetPS() const;

	ConstantBuffer* GetCBNeverChange();
	ConstantBuffer* GetCBChangeOnResize();
	ConstantBuffer* GetCBChangeEveryFrame();

private:
	TextureVertexShader* m_pVS = nullptr;
	TexturePixelShader* m_pPS = nullptr;

	ConstantBuffer* m_apCB[ 3 ];
};

class TextureVertexShader
	: public IVertexShader
{
public:
	TextureVertexShader( ShaderParam _param );
	virtual ~TextureVertexShader() {}
	virtual bool InitLayout( ID3D11Device* pDevice, ID3DBlob* pBlob ) override;
};

class TexturePixelShader
	: public IPixelShader
{
public:
	TexturePixelShader( ShaderParam _param );
	virtual ~TexturePixelShader() {}
};