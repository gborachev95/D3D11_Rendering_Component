#pragma pack_matrix( row_major )
struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float3 normals    : NORMALS;
	float3 uv         : UV;
	float3 tangents   : TANGENTS;
	float3 bitangents : BITANGENTS;
	float3 shine      : SHINE;
};
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float3 normals             : NORMALS;
	float2 uv                  : UV;
	float3 tangents            : TANGENTS;
	float3 bitangents          : BITANGENTS;
	float3 shine               : SHINE;
	// Not part of the structure
	float3 worldPosition       : WORLDPOS;
	float3 cameraPosition      : CAMERA_POS;
};

cbuffer OBJECT : register(b0)
{
	float4x4 instancesMatricies[30];

}
cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer, unsigned int _id : SV_InstanceID)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	float4 localCoordinate = float4(fromVertexBuffer.coordinate.xyz, 1);

		// Shading
	localCoordinate = mul(localCoordinate, instancesMatricies[_id]);
	sendToRasterizer.worldPosition = localCoordinate.xyz;
	localCoordinate = mul(localCoordinate, viewMatrix);
	localCoordinate = mul(localCoordinate, projectionMatrix);
	float3 worldNormals = mul(float4(fromVertexBuffer.normals.xyz, 0), instancesMatricies[_id]).xyz;

	// Tangents and Bitangents
	sendToRasterizer.tangents = mul(fromVertexBuffer.tangents, (float3x3)instancesMatricies[_id]);
	sendToRasterizer.tangents = normalize(sendToRasterizer.tangents);
	sendToRasterizer.bitangents = mul(fromVertexBuffer.bitangents, (float3x3)instancesMatricies[_id]);
	sendToRasterizer.bitangents = normalize(sendToRasterizer.bitangents);

	// Sending data
	sendToRasterizer.projectedCoordinate = localCoordinate;
	sendToRasterizer.normals = worldNormals;
	sendToRasterizer.uv.xy = fromVertexBuffer.uv.xy;
	sendToRasterizer.cameraPosition = viewMatrix[3].xyz;

	sendToRasterizer.shine = fromVertexBuffer.shine;
	return sendToRasterizer;
}