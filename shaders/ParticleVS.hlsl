cbuffer GeneratorData : register(b0)
{
	float lifeTime;
};

struct VSInput
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

struct VStoGS {
	float4 color	: COLOR;
	float3 position	: POSITION;
	float  size		: TEXCOORD0;
	int    type		: TEXCOORD1;
};

float BezierCurve(float p0, float p1, float p2, float t) {
	float oneMinusT = 1 - t;
	return oneMinusT * oneMinusT * p0 + 2 * oneMinusT * t * p1 + t * t * p2;
}

float2 BezierCurve(float2 p0, float2 p1, float2 p2, float t) {
	return float2(
		BezierCurve(p0.x, p1.x, p2.x, t),
		BezierCurve(p0.y, p1.y, p2.y, t));
}

float3 BezierCurve(float3 p0, float3 p1, float3 p2, float t) {
	return float3(
		BezierCurve(p0.x, p1.x, p2.x, t),
		BezierCurve(p0.y, p1.y, p2.y, t),
		BezierCurve(p0.z, p1.z, p2.z, t));
}

float4 BezierCurve(float4 p0, float4 p1, float4 p2, float t)
{
	return float4(
		BezierCurve(p0.x, p1.x, p2.x, t),
		BezierCurve(p0.y, p1.y, p2.y, t),
		BezierCurve(p0.z, p1.z, p2.z, t),
		BezierCurve(p0.w, p1.w, p2.w, t));
}


VStoGS main(VSInput input)
{
	VStoGS output;

	float t = input.age;
	float agePercent = t / lifeTime;
	output.position.xyz = input.position + input.velocity * t + input.acceleration * t * t * 0.5f;
	output.size  =  BezierCurve(input.startSize, input.midSize, input.endSize, agePercent);
	output.color =  BezierCurve(input.startColor, input.midColor, input.endColor, agePercent);
	output.type  =  input.type;
	return output;
}