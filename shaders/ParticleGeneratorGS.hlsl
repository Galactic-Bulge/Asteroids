#define ROOT 0
#define STANDARD 1

cbuffer GlobalData : register(b0)
{
	float dt;
	float tt;
};

cbuffer GeneratorData : register(b1)
{
	float3 generatorPos;
	float  spawnRate;
};

struct GSInput
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

texture1D randomTexture : register(t0);
sampler   randomSampler : register(s0);

[maxvertexcount(2)]
void main(point GSInput input[1],
	inout PointStream<GSInput> outStream)
{
	// Increment particle age
	input[0].age += dt;

	// Can this particle generate new particles?
	if (input[0].type == ROOT) {
		if (input[0].age >= spawnRate) {
			input[0].age = 0;

			GSInput newParticle;
			newParticle.startColor = input[0].startColor;
			newParticle.midColor = input[0].midColor;
			newParticle.endColor = input[0].endColor;
			newParticle.position = generatorPos;
			newParticle.velocity = input[0].velocity;
			newParticle.acceleration = input[0].acceleration;
			newParticle.size = input[0].size;
			newParticle.age = 0;
			newParticle.lifeTime = input[0].lifeTime;
			newParticle.startTime = tt;
			newParticle.type = STANDARD;

			// Random offsets
			float4 random = randomTexture.SampleLevel(randomSampler, tt * 10, 0);
			newParticle.position.xy += random.xy * 0.05f;

			outStream.Append(newParticle);
		}

		outStream.Append(input[0]);
	}
	else if (input[0].age < input[0].lifeTime) {
		outStream.Append(input[0]);
	}
}