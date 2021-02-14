Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
	matrix view;
};

cbuffer cbChagneOnResize : register( b1 )
{
	matrix projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
	matrix world;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PS_INPUT vs_main( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	
	output.pos = mul( input.pos, world );
	output.pos = mul( output.pos, view );
	output.pos = mul( output.pos, projection );

	output.tex = input.tex;

	return output;
}

float4 ps_main( PS_INPUT input ) : SV_Target
{
	return txDiffuse.Sample( samLinear, input.tex );
}