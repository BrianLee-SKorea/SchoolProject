
cbuffer LightConstantBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;

	float4 vLightDir;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};

PS_INPUT vs_main( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;

	output.pos = mul( input.pos, world );
	output.pos = mul( output.pos, view );
	output.pos = mul( output.pos, projection );

	output.normal = mul( input.normal, world );

	return output;
}


float4 ps_main( PS_INPUT input ) : SV_Target
{
	/*float3 diffuse = saturate( dot( ( float3 )vLightDir, input.normal ) );
	float3 ambient = float3( 0.1f, 0.1f, 0.1f );
	float4 color = float4( diffuse + ambient, 1.0f );*/

	float4 color = saturate( dot( ( float3 )vLightDir, input.normal ) );

	return color;
}