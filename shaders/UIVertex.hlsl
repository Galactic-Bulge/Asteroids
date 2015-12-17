struct VertexShaderInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer PerObject : register(b0)
{
    matrix translation;
    matrix scale;
};

cbuffer PerFrame : register(b1)
{
    matrix projection;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4 position = float4(input.position.x, input.position.y, 0.1f, 1.0f);
    matrix world = mul(scale, translation);
    output.position = mul(mul(position, world), projection);
    output.uv = input.uv;
    return output;
}
