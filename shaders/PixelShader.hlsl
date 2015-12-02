#include "Light.hlsl"

struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float3 worldPos     : POSITION;
    float3 normal       : NORMAL;
    float2 uv           : TEXCOORD1;
};

cbuffer PerFrame : register(b0)
{
    float3 cameraPosition;
};

cbuffer Lights : register(b1)
{
    DirectionalLight directionalLights[MAX_LIGHTS];
    PointLight pointLights[MAX_LIGHTS];
    SpotLight spotLights[MAX_LIGHTS];
};

Texture2D diffuseTexture    : register(t0);
Texture2D rustTexture       : register(t1);
Texture2D specMapTexture    : register(t2);
SamplerState trilinear      : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    // Re-normalize interpolated normal
    input.normal = normalize(input.normal);

    // Sample the Specular Map.
    float specMapValue = specMapTexture.Sample(trilinear, input.uv).r;

    // Initialize Lighting.
    float4 dirColor = float4(0, 0, 0, 1);
    float4 pointColor = float4(0, 0, 0, 1);
    float4 spotColor = float4(0, 0, 0, 1);

    // Calculate Directional Lighting.
    [unroll] for(int i = 0; i < MAX_LIGHTS; ++i)
    {
        dirColor += DirectionalLightCalculation(directionalLights[i], input.worldPos, input.normal, cameraPosition, specMapValue);
    }

    // Calculate Point Lighting.
    [unroll] for(int j = 0; j < MAX_LIGHTS; ++j)
    {
        pointColor += PointLightCalculation(pointLights[j], input.worldPos, input.normal, cameraPosition, specMapValue);
    }

    // Calculate Spot Lighting.
    [unroll] for(int k = 0; k < MAX_LIGHTS; ++k)
    {
        spotColor += SpotLightCalculation(spotLights[k], input.worldPos, input.normal, cameraPosition, specMapValue);
    }

    // Sample the diffuse texture.
    float4 diffuseColor = rustTexture.Sample(trilinear, input.uv);

    // Calculate the final color.
	return diffuseColor * (dirColor + pointColor + spotColor);
}
