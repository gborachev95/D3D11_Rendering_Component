struct INPUT_PIXEL
{
	float4 projectedCoordinate : SV_POSITION;
	float3 normals             : NORMALS;
	float2 uv                  : UV;
	// Not part of the structure
	float3 worldPosition       : WORLDPOS;
	float3 cameraPosition      : CAMERA_POS;
	//float4 projectedLight      : LIGHT_PROJ;
};

texture2D baseTexture : register(t0);
//texture2D depthTexture : register(t1);
SamplerState filterWrap  : register(s0);
//SamplerState filterClamp : register(s1);

struct LIGHT_DATA
{
	float3 transform;
	float3 direction;
	float  radius;
	float4 color;
	bool   status;
	bool3  padding;
};
cbuffer DIRECTION_LIGHT : register(b2)
{
	LIGHT_DATA dir_light;
};
cbuffer POINT_LIGHT : register(b3)
{
	LIGHT_DATA point_light;
};
cbuffer SPOT_LIGHT : register(b4)
{
	LIGHT_DATA spot_light;
};

float4 main(INPUT_PIXEL _inputPixel) : SV_TARGET 
{
	float4 currColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 dirLightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 pointLightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spotLightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
	// Getting the texture color
	currColor = baseTexture.Sample(filterWrap, _inputPixel.uv.xy);
	// Normalize normals
	_inputPixel.normals.xyz = normalize(_inputPixel.normals.xyz);
	// Directional light
	if (dir_light.status)
	{
		_inputPixel.normals = normalize(_inputPixel.normals);
		float lightRatio = saturate(dot(-dir_light.direction, _inputPixel.normals));
		dirLightColor = lightRatio * dir_light.color * currColor;
		dirLightColor += currColor * 0.5f;
	}
	// Point light
	if (point_light.status)
	{
		float3 lightDir = normalize(point_light.transform.xyz - _inputPixel.worldPosition.xyz);
		float lightRatio = saturate(dot(lightDir.xyz, _inputPixel.normals.xyz));
		lightRatio *= saturate(1 / dot(point_light.transform.xyz - _inputPixel.worldPosition.xyz, point_light.transform.xyz - _inputPixel.worldPosition.xyz));
		pointLightColor = lightRatio * point_light.color * currColor;
	}
	// Spot light
	if (spot_light.status)
	{
		float spotFactor = 0;
		float3 lightDir = spot_light.transform.xyz - _inputPixel.worldPosition.xyz;
		float len = length(lightDir);
		lightDir = normalize(lightDir);
		float surRatio = saturate(dot(-lightDir.xyz, spot_light.direction));
        // Calculating attenuation
		float aten = 1 - saturate((len / 10.0f));
		if (surRatio > spot_light.radius)
			spotFactor = 5.0f* aten;
		else
			spotFactor = aten * 0.7f;
		float lightRatio = saturate(dot(lightDir.xyz, _inputPixel.normals.xyz));
		spotLightColor = spotFactor * lightRatio * aten * spot_light.color * currColor;
	}
	
	// Apply shadows
	//float bias = 0.001f;
	//float2 uvDepth;
	//float4 finalColor = saturate(dirLightColor + pointLightColor + spotLightColor);
	//uvDepth.x = _inputPixel.projectedLight.x / _inputPixel.projectedLight.w / 2.0f + 0.5f;
	//uvDepth.y = -_inputPixel.projectedLight.y / _inputPixel.projectedLight.w / 2.0f + 0.5f;
	//if ((saturate(uvDepth.x) == uvDepth.x) && (saturate(uvDepth.y) == uvDepth.y))
	//{
	//	float depthValue = depthTexture.Sample(filterClamp, uvDepth.xy).r;
	//	float lightDepthValue = _inputPixel.projectedLight.z / _inputPixel.projectedLight.z;
	//	lightDepthValue = lightDepthValue - bias;
	//	if (lightDepthValue < depthValue)
	//	{
	//		float3 lightDir = normalize(point_light.transform.xyz - _inputPixel.worldPosition.xyz);
	//		float lightIntensity = saturate(dot(_inputPixel.normals, lightDir));
	//		if (lightIntensity > 0.0f)
	//			finalColor = saturate((dirLightColor + pointLightColor + spotLightColor) * lightIntensity);
	//	}
	//}
	// End of shadows

	return saturate(dirLightColor + pointLightColor + spotLightColor);
}
