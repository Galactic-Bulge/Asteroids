#define MAX_LIGHTS 10

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float specularIntensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float specularIntensity;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float specularIntensity;
    float3 direction;
    float power;
};

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

float4 DirectionalLightCalculation(DirectionalLight light, VertexToPixel input, float specMapValue)
{
    float3 normal = input.normal;
    float3 lightDir = normalize(-light.direction);
    float3 viewDir = normalize(cameraPosition - input.worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return light.color * (diffuse + specular * specMapValue);
}

float4 PointLightCalculation(PointLight light, VertexToPixel input, float specMapValue)
{
    float3 normal = input.normal;
    float3 lightPos = light.position;
    float3 lightDir = normalize(lightPos - input.worldPos);
    float3 viewDir = normalize(cameraPosition - input.worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return light.color * (diffuse + specular * specMapValue);
}

float4 SpotLightCalculation(SpotLight light, VertexToPixel input, float specMapValue)
{
    float3 normal = input.normal;
    float3 lightPos = light.position;
    float3 lightDir = normalize(lightPos - input.worldPos);
    float3 viewDir = normalize(cameraPosition - input.worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float angleFromCenter = max(dot(-lightDir, light.direction), 0);
    float spotFalloff = pow(angleFromCenter, light.power);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return spotFalloff * (light.color * (diffuse + specular * specMapValue));
}

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
        dirColor += DirectionalLightCalculation(directionalLights[i], input, specMapValue);
    }

    // Calculate Point Lighting.
    [unroll] for(int j = 0; j < MAX_LIGHTS; ++j)
    {
        pointColor += PointLightCalculation(pointLights[j], input, specMapValue);
    }

    // Calculate Spot Lighting.
    [unroll] for(int k = 0; k < MAX_LIGHTS; ++k)
    {
        spotColor += SpotLightCalculation(spotLights[k], input, specMapValue);
    }

    // Sample the diffuse texture.
    float4 diffuseColor = rustTexture.Sample(trilinear, input.uv);

    // Calculate the final color.
    return diffuseColor * (dirColor + pointColor + spotColor);
}
