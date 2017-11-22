struct INPUT_PIXEL
{
	float4 color : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	// Hacking the tag using normal.x
	float3 isSkyBox : NORMALS;
	float3 uvw : UV;
};
textureCUBE skyboxTexture : register (t0);

SamplerState filter : register(s0);

float4 main( INPUT_PIXEL _inputPixel ) : SV_TARGET
{
	float4 currColor = _inputPixel.color;
	if (_inputPixel.isSkyBox.x == 1)
		currColor = skyboxTexture.Sample(filter, _inputPixel.uvw);
	return currColor;
}