cbuffer PerFrame : register (b0)
{
	float t;
}

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float3 direction : NORMAL;
};

TextureCube skybox : register(t0);
TextureCube fromSkybox : register(t1);
SamplerState trilinear : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = lerp(fromSkybox.Sample(trilinear, input.direction), skybox.Sample(trilinear, input.direction), t);
	return color;
    //return skybox.Sample(trilinear, input.direction);
}
