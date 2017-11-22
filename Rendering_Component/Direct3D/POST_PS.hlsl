struct INPUT_PIXEL
{
	float4 color : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float3 normals : NORMALS;
	float3 uv : UV;
};

texture2D baseTexture : register (t0);

texture2D depthTexture : register (t1); 

SamplerState filter : register(s0);

cbuffer TIME : register(b0)
{
	float deltaTime;
	float effectType;
};
float4 main(INPUT_PIXEL _inputPixel) : SV_TARGET
{
	float newU = _inputPixel.uv.x;
	if (effectType == 5)
		newU += cos(_inputPixel.uv.y * 8 * 3.14159 + deltaTime) / 100;
	float4 color = baseTexture.Sample(filter, float2(newU, _inputPixel.uv.y));

	// True value of Z
	float zLinear = (2 * 0.1f) / (100.0f + 0.1f - depthTexture.Sample(filter, _inputPixel.uv.xy).r * (100.0f - 0.1f));

	// Fog
	if (effectType == 1)
		color.rgba = lerp(float4(0.7f, 0.7f, 0.7f, 1), color.rgba, 1 - zLinear);

	// Mute colors
	if (effectType == 2)
		color.xyz = dot(float3(0.15f, 0.4f, 0.4f), color.xyz);

	// Weird side effect
	if (effectType == 3)
		color.r = lerp(float4(1, 1, 1, 1), color.rgba, sin(deltaTime)).r;

	// Blinking
	if (effectType == 4)
		color *= saturate(lerp(float4(0, 0, 0, 1), float4(0.5f, 0.5f, 0.5f, 1), (cos(deltaTime * 2) + _inputPixel.uv.y)));

		return color;//float4(1 - (color.x ), 1 - (color.x ) , 1 - (color.x), 1);

}