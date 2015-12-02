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

float4 DirectionalLightCalculation(DirectionalLight light, float3 worldPos, float3 normal, float3 cameraPos, float specMapValue)
{
    float3 lightDir = normalize(-light.direction);
    float3 viewDir = normalize(cameraPos - worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return light.color * (diffuse + specular * specMapValue);
}

float4 PointLightCalculation(PointLight light, float3 worldPos, float3 normal, float3 cameraPos, float specMapValue)
{
    float3 lightPos = light.position;
    float3 lightDir = normalize(lightPos - worldPos);
	float3 viewDir = normalize(cameraPos - worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return light.color * (diffuse + specular * specMapValue);
}

float4 SpotLightCalculation(SpotLight light, float3 worldPos, float3 normal, float3 cameraPos, float specMapValue)
{
    float3 lightPos = light.position;
    float3 lightDir = normalize(lightPos - worldPos);
    float3 viewDir = normalize(cameraPos - worldPos);
    float3 reflection = reflect(-lightDir, normal);

    float angleFromCenter = max(dot(-lightDir, light.direction), 0);
    float spotFalloff = pow(angleFromCenter, light.power);

    float4 diffuse = saturate(dot(normal, lightDir));
    float specular = pow(max(dot(reflection, viewDir), 0), light.specularIntensity);

    return spotFalloff * (light.color * (diffuse + specular * specMapValue));
}
