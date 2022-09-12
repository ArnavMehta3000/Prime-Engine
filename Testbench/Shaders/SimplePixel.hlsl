cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return color;
}