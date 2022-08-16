cbuffer ConstantBuffer : register(b0)
{
    float xOffset;
    float yOffset;
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
    
    input.inPos.x += xOffset;
    input.inPos.y += yOffset;
    
	output.outPos = float4(input.inPos, 1.0);
	output.outCol = input.inCol;
	
    return output;
}