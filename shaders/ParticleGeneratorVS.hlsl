struct VStoGS
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

VStoGS main(VStoGS input)
{
	return input;
}