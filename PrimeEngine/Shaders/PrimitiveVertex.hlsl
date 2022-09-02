cbuffer PrimitivesBuffer : register(b0)
{
    //matrix WVP; // 16 bytes
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    //uint InstanceID : SV_InstanceID;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
};

VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.inPos, 1.0f);
    //pos = mul(pos, WVP);
    pos = mul(pos, WorldMatrix);
    pos = mul(pos, ViewMatrix);
    pos = mul(pos, ProjectionMatrix);
    
    output.outPos = pos;
    
    return output;
}