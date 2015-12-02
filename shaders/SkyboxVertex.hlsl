cbuffer PerFrame : register(b0)
{
    matrix view;
    matrix projection;
};

struct VertexShaderInput
{
    float3 position : POSITION;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
};

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float3 direction : NORMAL;
};

VertexToPixel main(VertexShaderInput input)
{
    // Remove translation component from View matrix.
    matrix viewNoTranslation = view;
    viewNoTranslation._41 = 0;
    viewNoTranslation._42 = 0;
    viewNoTranslation._43 = 0;

    matrix viewProjection = mul(viewNoTranslation, projection);

    VertexToPixel output;

    // Calculate output position.
    output.position = mul(float4(input.position, 1), viewProjection);

    // Set depth to max value.
    output.position.z = output.position.w;

    // Raw position will be sampling direction.
    output.direction = input.position;

    return output;
}
