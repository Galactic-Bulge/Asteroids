// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
    float4 position		: SV_POSITION;
	float3 worldPos     : TEXCOORD0;
    float3 normal       : NORMAL;
    float2 uv           : TEXCOORD1;
};

float isColliding;

SamplerState trilinear      : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
    // Re-normalize interpolated normal
    input.normal = normalize(input.normal);

	float4 diffuseColor;

    // Sample the diffuse texture
	if (isColliding == 1) {
		diffuseColor = float4(1, 0, 0, 1);
	}
	else {
		diffuseColor = float4(0, 0, 1, 1);
	}

    // Combine lights
	return diffuseColor;
}
