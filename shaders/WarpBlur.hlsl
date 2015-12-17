cbuffer PerFrame : register(b0)
{
	float BlurWidth;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels		: register(t0);
SamplerState trilinear	: register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{

	float2 Center = { 0.5, 0.5 }; ///center of the screen
	float BlurStart = 1.0f; /// blur offset
	int nsamples = 5;

	input.uv -= Center;


	float4 color = float4(0,0,0,0);

	for (int i = 0; i < nsamples; i++)
	{
		float scale = BlurStart + BlurWidth * (i / (float)(nsamples - 1));
		color += pixels.Sample(trilinear, input.uv * scale + Center);
	}

	color /= nsamples;
	color = saturate(color);

	return color;
}