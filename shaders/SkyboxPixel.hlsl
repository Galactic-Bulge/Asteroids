struct VertexToPixel
{
    float4 position : SV_POSITION;
    float3 direction : NORMAL;
};

TextureCube skybox : register(t0);
SamplerState trilinear : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    return skybox.Sample(trilinear, input.direction);
}
