#pragma pack_matrix( row_major )
struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	// Hacking the tag using normal.x
	float3 isSkyBox : NORMALS;
	float3 uvw :UV;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float3 isSkyBox : NORMALS;
	float3 uvw :UV;
};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
}
cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	float4 localCoordinate = float4(fromVertexBuffer.coordinate.xyz, 1);

    // Shading
	localCoordinate = mul(localCoordinate, worldMatrix);
	localCoordinate = mul(localCoordinate, viewMatrix);
	localCoordinate = mul(localCoordinate, projectionMatrix);

	// Sending data
	sendToRasterizer.projectedCoordinate = localCoordinate;
	sendToRasterizer.colorOut = fromVertexBuffer.color;
	sendToRasterizer.isSkyBox = fromVertexBuffer.isSkyBox;
	sendToRasterizer.uvw = fromVertexBuffer.coordinate.xyz;
	return sendToRasterizer;
}
