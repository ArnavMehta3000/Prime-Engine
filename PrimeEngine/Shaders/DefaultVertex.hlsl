cbuffer WVPBuffer : register(b0)
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
}


struct VS_INPUT
{
	float3 inPos : POSITION;
    float4 inCol : COLOR;
};

struct VS_OUTPUT
{
	float4 outPos : SV_POSITION;
	float4 outCol : COLOR;
};



VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.inPos, 1.0f);
    pos = mul(pos, WorldMatrix);
    pos = mul(pos, ViewMatrix);
    pos = mul(pos, ProjectionMatrix);
    
    output.outPos = pos;
	output.outCol = input.inCol;
	
    return output;
}