Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION; /*float2 TexCoord : TEXCOORD0;*/
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
};

float4 PSmain(PS_INPUT input) : SV_TARGET
{
    float4 texColor = tex.Sample(samplerState, input.UV);
    
    return input.Color * texColor;// * texColor;
}