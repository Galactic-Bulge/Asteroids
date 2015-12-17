cbuffer TransformationData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct GSInput
{
	float4 color	: COLOR;
	float3 position	: POSITION;
	float  size		: TEXCOORD0;
	int    type		: TEXCOORD1;
};

struct GStoPS
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

[maxvertexcount(4)]
void main(point GSInput input[1], 
		  inout TriangleStream<GStoPS> outStream)
{
	// Don't draw the ROOT particle
	if (input[0].type == 0) return;

	GStoPS output;

	float2 offsets[4];
	offsets[0] = float2(-0.1f, -0.1f);
	offsets[1] = float2(-0.1f,  0.1f);
	offsets[2] = float2( 0.1f, -0.1f);
	offsets[3] = float2( 0.1f,  0.1f);

	matrix mvp = mul(mul(world, view), projection);

	[unroll]
	for (int i = 0; i < 4; i++) {
		output.position = mul(float4(input[0].position, 1), mvp);

		float depthChange = output.position.z / output.position.w;

		output.position.xy += offsets[i] * depthChange * input[0].size;
		output.color = input[0].color;

		outStream.Append(output);
	}
}