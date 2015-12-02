cbuffer PerFrame : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer PerObject : register(b1)
{
	int id;
    matrix world;
};

// Describes individual vertex data
struct VertexShaderInput
{
    float3 position		: POSITION;
    float2 uv			: TEXCOORD;
    float3 normal		: NORMAL;
};

// Defines the output data of our vertex shader
struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float3 worldPos     : POSITION;
    float3 normal       : NORMAL;
    float2 uv           : TEXCOORD1;
	float2 noise        : TEXCOORD2;
};


float rand(float2 co) {
	return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453))*0.5;
}

Texture2D perlinTexture      : register(t4);

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
    // Set up output
    VertexToPixel output;

	//float3 tempWorldPos = mul(float4(1,1,1,1), world).xyz;

	uint2 samplePos;
	float randnum = rand(id);
	samplePos.x = (int)((input.uv.x / 2.0f + 1.0f + randnum) * 512) % 512;
	samplePos.y = (int)((input.uv.y / 2.0f + 1.0f + randnum) * 512) % 512;
	//float smallNoiseChange = perlinSmallTexture[input.uv + tempWorldPos.xy].r;
	float noiseChange = perlinTexture[samplePos].r;
	input.position += input.normal * (noiseChange * 0.5f);
	output.noise = mul(float2((float)samplePos.x/512, (float)samplePos.y/512), (float3x3)world);

    // Calculate output position
    matrix worldViewProj = mul(mul(world, view), projection);
    output.position = mul(float4(input.position, 1.0f), worldViewProj);

    // Take into account rotation (but not translation)
    // Most of the  world matrix
    output.normal = mul(input.normal, (float3x3)world);

    // The world space position of the vertex
    output.worldPos = mul(float4(input.position, 1), world).xyz;

	// Pass the UV's through
	output.uv = input.uv;// *(noise(input.uv*id*0.001f) * 10 - 5);

    return output;
}
