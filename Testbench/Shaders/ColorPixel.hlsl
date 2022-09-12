struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float4 inCol : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.inCol;
}