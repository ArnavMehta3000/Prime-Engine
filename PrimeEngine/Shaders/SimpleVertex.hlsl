cbuffer WVPBuffer : register(b0)
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
}


struct VS_INPUT
{
    float3 inPos : POSITION;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
};



VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.inPos, 1.0f);
    pos = mul(pos, WorldMatrix);
    pos = mul(pos, ViewMatrix);
    pos = mul(pos, ProjectionMatrix);
    
    output.outPos = pos;
	
    return output;
}