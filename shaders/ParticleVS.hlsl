struct VSInput
{
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float  size : TEXCOORD2;
	float  age : TEXCOORD3;
	float  lifeTime : TEXCOORD4;
	float  startTime : TEXCOORD5;
	int    type : TEXCOORD6;
	float3 position		: POSITION;
	float3 velocity		: TEXCOORD0;
	float3 acceleration	: TEXCOORD1;
};

struct VStoGS {
	float4 color	: COLOR;
	float3 position	: POSITION;
	float size : TEXCOORD0;
	int type : TEXCOORD1;
};

VStoGS main(VSInput input)
{
	VStoGS output;

	output.color = input.startColor;
	output.color.w = input.age / input.lifeTime;
	float t = input.age;
	output.position.xyz = input.position + input.velocity; // +input.acceleration * t * t * 0.5;
	output.size = input.size;
	output.type = input.type;
	return output;
}