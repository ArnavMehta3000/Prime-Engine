cbuffer cbPerFrame : register(b0)
{
    matrix ViewMatrix;
    matrix ProjectionMatrix;
}

cbuffer cbPerObject : register(b1)
{
    matrix WorldMatrix;
    float4 Color;
}

struct VS_INPUT
{
    float3 Pos : POSITION; /*float2 TexCoord : TEXCOORD0;*/
};
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION; /*float2 TexCoord : TEXCOORD0;*/
    float4 Color : COLOR;
};



VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.Pos, 1.0f);
    pos = mul(pos, WorldMatrix);
    pos = mul(pos, ViewMatrix);
    pos = mul(pos, ProjectionMatrix);
    
    output.Pos = pos;
   // output.TexCoord = input.TexCoord;
    output.Color = Color;
    
    return output;
}

