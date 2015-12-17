
// Defines the output data of our vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels		: register(t0);
SamplerState trilinear	: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return pixels.Sample(trilinear, input.uv);
}