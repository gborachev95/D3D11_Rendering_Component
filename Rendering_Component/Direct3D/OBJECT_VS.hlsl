#pragma pack_matrix( row_major )
struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float3 normals    : NORMALS;
	float3 uv         : UV;
};
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float3 normals             : NORMALS;
	float2 uv                  : UV;
	// Not part of the structure
	float3 worldPosition       : WORLDPOS;
	float3 cameraPosition      : CAMERA_POS;
	// Lights
	//float4 projectedLight      : LIGHT_PROJ;
};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
}
cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4 cameraPosition;
}
//cbuffer LIGHT : register (b2)
//{
//	// Lights
//	float4x4 lightViewMatrix;
//	float4x4 lightProjectionMatrix;
//}
OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	float4 localCoordinate = float4(fromVertexBuffer.coordinate.xyz, 1);

	// Shading
	localCoordinate = mul(localCoordinate, worldMatrix);
	sendToRasterizer.worldPosition.xyz = localCoordinate.xyz;
	float3 worldNormals = mul(float4(fromVertexBuffer.normals.xyz,0), worldMatrix).xyz;
	localCoordinate = mul(localCoordinate, viewMatrix);
	localCoordinate = mul(localCoordinate, projectionMatrix);

	// Lights shading
	//float4 localLight = float4(fromVertexBuffer.coordinate.xyz, 1);
	//localLight = mul(localLight, worldMatrix);
	//localLight = mul(localLight, lightViewMatrix);
	//localLight = mul(localLight, lightProjectionMatrix);

	// Sending data
	sendToRasterizer.projectedCoordinate = localCoordinate;
	sendToRasterizer.normals = worldNormals;
	sendToRasterizer.uv = fromVertexBuffer.uv.xy;
	sendToRasterizer.cameraPosition = viewMatrix[3].xyz;
	// Lights
	//sendToRasterizer.projectedLight = localLight;
	return sendToRasterizer;
}
