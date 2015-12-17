struct VStoGS
{
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float  startSize	: TEXCOORD0;
	float  midSize		: TEXCOORD1;
	float  endSize		: TEXCOORD2;
	float  age			: TEXCOORD3;
	int    type			: TEXCOORD4;
	float3 position		: POSITION;
	float3 velocity		: TEXCOORD5;
	float3 acceleration	: TEXCOORD6;
};

VStoGS main(VStoGS input)
{
	return input;
}