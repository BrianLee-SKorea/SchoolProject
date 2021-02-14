Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer ToonConstantBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;

	float4 gLightPos;
	float4 gCameraPos;
	float4 surfaceColor;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL0;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

PS_INPUT vs_main( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;

	output.pos = mul( input.pos, world );

	float3 lightDir = normalize( output.pos - gLightPos );
	float viewDir = normalize( output.pos - gCameraPos );
	output.viewDir = viewDir;

	output.pos = mul( output.pos, view );
	output.pos = mul( output.pos, projection );

	float3 worldNormal = normalize( mul( input.normal, world ) );
	output.diffuse = dot( lightDir, worldNormal );
	output.reflection = reflect( lightDir, worldNormal );

	output.tex = input.tex;

	return output;
}

float4 ps_main( PS_INPUT input ) : SV_Target
{
	float3 diffuse = saturate( input.diffuse );

	float3 reflection = normalize( input.reflection );
	float3 viewDir = normalize( input.viewDir );
	float3 specular = 0.0f;

	if( diffuse.x > 0 )
	{
		specular = saturate( dot( reflection, -viewDir ) );
		specular = pow( specular, 30.0f );
	}
	
	float4 color = float4( diffuse, 1.0f );
	color.y += 0.4;

	color = ceil( color * 5 ) / 5.0f;

	return color;
}