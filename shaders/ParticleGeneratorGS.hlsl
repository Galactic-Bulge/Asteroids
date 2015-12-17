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
	float  lifeTime;
};

struct GSInput
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
			newParticle.startColor		= input[0].startColor;
			newParticle.midColor		= input[0].midColor;
			newParticle.endColor		= input[0].endColor;
			newParticle.position		= generatorPos;
			newParticle.velocity		= input[0].velocity;
			newParticle.acceleration	= input[0].acceleration;
			newParticle.startSize		= input[0].startSize;
			newParticle.midSize			= input[0].midSize;
			newParticle.endSize			= input[0].endSize;
			newParticle.age				= 0.0f;
			newParticle.type			= STANDARD;

			// Random offsets
			float4 random = randomTexture.SampleLevel(randomSampler, tt * 10, 0);
			newParticle.position.xy += random.xy * 0.05f;
			newParticle.velocity.x = random.x;
			newParticle.velocity.y = random.y;
			newParticle.velocity.z = input[0].velocity.z;

			outStream.Append(newParticle);
		}

		outStream.Append(input[0]);
	}
	else if (input[0].age < lifeTime) {
		outStream.Append(input[0]);
	}
}