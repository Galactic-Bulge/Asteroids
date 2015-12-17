struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D buttonTexture : register(t0);
SamplerState trilinear : register(s0);

float4 main(VertexShaderOutput output) : SV_TARGET
{
    return buttonTexture.Sample(trilinear, output.uv);
}
