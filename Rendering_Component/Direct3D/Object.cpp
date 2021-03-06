#include "defines.h"
// Constructors
Object::Object()
{
}

// Destructors
Object::~Object()
{
		delete m_vertecies;
        delete m_indexList;
}

// Instantiates the object using his buffers
void Object::Instantiate(ID3D11Device* _device, std::string _filePath, XMFLOAT3 _position, float _shine)
{
	ReadObject(_filePath, _shine);
	CreateVertexBuffer(_device);
	CreateIndexBuffer(_device);
	m_worldToShader.worldMatrix = XMMatrixIdentity();
	m_worldToShader.worldMatrix.r[3].m128_f32[0] = _position.x;
	m_worldToShader.worldMatrix.r[3].m128_f32[1] = _position.y;
	m_worldToShader.worldMatrix.r[3].m128_f32[2] = _position.z;
}

// Draws the object to the screen
void Object::Render(ID3D11DeviceContext* _context, ID3D11Buffer* _constBuffer, CComPtr<ID3D11ShaderResourceView> _shader)
{
	// Set the index buffer
	unsigned int stride = sizeof(Vector);
	unsigned int offset = 0;
	_context->IASetVertexBuffers(0, 1, &m_vertexBuffer.p, &stride, &offset);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the shader resource - for the texture
	_context->PSSetShaderResources(0, 1,&m_shaderResourceView.p);

	if (_shader.p != nullptr)
		_context->PSSetShaderResources(1, 1, &_shader.p);

	// Constant buffer mapping
	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	_context->Map(_constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &m_worldToShader, sizeof(OBJECT_TO_VRAM));
	_context->Unmap(_constBuffer, NULL);
    // Setsampler

	// Set the index buffer
	_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_context->DrawIndexed(m_numIndicies, 0, 0);
}

// Creates the vertex buffer for that object
void Object::CreateVertexBuffer(ID3D11Device* _device)
{
	// Create the vertex buffer storing vertsPoints
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Vector) * m_numVerts;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.StructureByteStride = sizeof(Vector);

	// Setting the resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = m_vertecies;
	_device->CreateBuffer(&bufferDesc, &resourceData, &m_vertexBuffer.p);
}

// Creates the index buffer for that object
void Object::CreateIndexBuffer(ID3D11Device* _device)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(unsigned int) * m_numIndicies;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = m_indexList;

	// Create the buffer with the device.
	_device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer.p);
}

/*
Reads vertecies, uvs, normals and other object stuff from a .obj file
Followed http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ tutorial
*/
bool Object::ReadObject(std::string _filePath,float _shine)
{
	// Local variables
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<XMFLOAT3> temp_vertices;
	vector<XMFLOAT3> temp_normals;
	vector<XMFLOAT2> temp_uvs;
	FILE *file;

	// Opening file - "r" -> read in
	fopen_s(&file, _filePath.c_str(), "r");
	// Check if file opened
	if (file == NULL)
		return false;

	// Looping until the file finishes
	while (true)
	{
		// Read the first word of the line
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));

		// Check if the file has finished
		if (res == EOF)
			break;

		// Check if the line is a vertex
		if (strcmp(lineHeader, "v") == 0)
		{
			XMFLOAT3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		// Check if the line is a UV
		else if (strcmp(lineHeader, "vt") == 0)
		{
			XMFLOAT2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		// Check if the line is a normal
		else if (strcmp(lineHeader, "vn") == 0)
		{
			XMFLOAT3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		// Check if the line is a index
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
				return false;

			// Setting the indicies for the vertecies
			vertexIndices.push_back(vertexIndex[0] - 1);
			vertexIndices.push_back(vertexIndex[1] - 1);
			vertexIndices.push_back(vertexIndex[2] - 1);

			// Setting the indicies for the UVs
			uvIndices.push_back(uvIndex[0] - 1);
			uvIndices.push_back(uvIndex[1] - 1);
			uvIndices.push_back(uvIndex[2] - 1);

			// Setting the indicies for the normals
			normalIndices.push_back(normalIndex[0] - 1);
			normalIndices.push_back(normalIndex[1] - 1);
			normalIndices.push_back(normalIndex[2] - 1);
		}
	}

	// Setting vertecies member
	m_numVerts = vertexIndices.size();
	m_vertecies = new Vector[m_numVerts];
	for (unsigned int i = 0; i < m_numVerts; ++i)
	{
		// Setting vertecies
		m_vertecies[i].transform = temp_vertices[vertexIndices[i]];
		// Setting normals
		m_vertecies[i].normals = temp_normals[normalIndices[i]];
		// Setting UVs
		m_vertecies[i].uv.x = temp_uvs[uvIndices[i]].x;
		m_vertecies[i].uv.y = temp_uvs[uvIndices[i]].y;
		m_vertecies[i].uv.z = 0;
		m_vertecies[i].shine.x = _shine;
	}

	// Computing the tangents and bitangents
	ComputeTangents();
	// Setting indecies member
	m_numIndicies = vertexIndices.size();
	m_indexList = new unsigned int[m_numIndicies];
	for (unsigned int i = 0; i < m_numIndicies; ++i)
		m_indexList[i] = i;

	// Return true if everything went right
	return true;
}

// Textures the object 
void Object::TextureObject(ID3D11Device* _device, const wchar_t* _filePath)
{
	 CreateDDSTextureFromFile(_device, _filePath, NULL, &m_shaderResourceView.p);
}

// Returns the world matrix of the object
XMMATRIX Object::GetWorldMatrix()
{
	return m_worldToShader.worldMatrix;
}

// Sets the world matrix of the object
void Object::SetWorldMatrix(XMMATRIX& _matrix)
{
	m_worldToShader.worldMatrix = _matrix;
}

// Moves the object according to the passed in values
void Object::SetPosition(float _x, float _y, float _z)
{
	m_worldToShader.worldMatrix.r[3].m128_f32[0] = _x;
	m_worldToShader.worldMatrix.r[3].m128_f32[1] = _y;
	m_worldToShader.worldMatrix.r[3].m128_f32[2] = _z;

}

/* 
Used for normal mapping.
Followed http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/ tutorial
Parameters: 
[in] _verts       - vertecies of object
[in] _uvs         - uv coordinates of object
[in] _normals     - normals of object
*/
void Object::ComputeTangents()
{
	vector<XMFLOAT3> temp_tangents;
	vector<XMFLOAT3> temp_bitangents;

	for (unsigned int i = 0; i < m_numVerts; i += 3)
	{
		// Getting the triangles 
		XMFLOAT3 tempV0 = m_vertecies[i].transform;
		XMFLOAT3 tempV1 = m_vertecies[i + 1].transform;
		XMFLOAT3 tempV2 = m_vertecies[i + 2].transform;
	
		XMFLOAT3 tempUV0 = m_vertecies[i].uv;
		XMFLOAT3 tempUV1 = m_vertecies[i + 1].uv;
		XMFLOAT3 tempUV2 = m_vertecies[i + 2].uv;
	
		// Edges of the triangle 
		XMFLOAT3 deltaPos1{ tempV1.x - tempV0.x, tempV1.y - tempV0.y, tempV1.z - tempV0.z };
		XMFLOAT3 deltaPos2{ tempV2.x - tempV0.x, tempV2.y - tempV0.y, tempV2.z - tempV0.z };

		XMFLOAT3 deltaUV1 = { tempUV1.x - tempUV0.x, tempUV1.y - tempUV0.y, 0 };
		XMFLOAT3 deltaUV2 = { tempUV2.x - tempUV0.x, tempUV2.y - tempUV0.y, 0 };
	
		float ratio = (1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x));

		// Calculating tangent
		XMFLOAT3 tempTangent1 { deltaPos1.x * deltaUV2.y, deltaPos1.y * deltaUV2.y, deltaPos1.z * deltaUV2.y };
		XMFLOAT3 tempTangent2{ deltaPos2.x * deltaUV1.y, deltaPos2.y * deltaUV1.y, deltaPos2.z * deltaUV1.y };
		XMFLOAT3 tempTangent3{ tempTangent1.x - tempTangent2.x, tempTangent1.y - tempTangent2.y, tempTangent1.z - tempTangent2.z };
		XMFLOAT3 tangent{ tempTangent3.x*ratio, tempTangent3.y*ratio, tempTangent3.z*ratio };

		// Calculating bitangent
		XMFLOAT3 tempBitangent{ tempTangent2.x - tempTangent1.x, tempTangent2.y - tempTangent1.y, tempTangent2.z - tempTangent1.z };
		XMFLOAT3 bitangent{ tempBitangent.x*ratio, tempBitangent.y*ratio, tempBitangent.z*ratio };
	
		// Setting them 
		m_vertecies[i].tangents = tangent;
		m_vertecies[i + 1].tangents = tangent;
		m_vertecies[i + 2].tangents = tangent;
	
		m_vertecies[i].bitangents = bitangent;
		m_vertecies[i + 1].bitangents = bitangent;
		m_vertecies[i + 2].bitangents = bitangent;
	}
}

// Instancing multiple objects at different positions. Creates an instance buffer.
void Object::CreateInstanceBuffer(ID3D11Device* _device,unsigned int _numberObjs, XMMATRIX* _positions)
{	
	D3D11_BUFFER_DESC bufferDesc;
	// Creating constant buffer for instansing
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = sizeof(XMMATRIX) *_numberObjs;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.StructureByteStride = sizeof(XMMATRIX);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = _positions;
	
	_device->CreateBuffer(&bufferDesc, &initData, &m_instanceBuffer.p);

}

// Renders multiple objecs of the same instance. Objects instance buffer need to be created before that - CreateInstanceBuffer()
void Object::RenderInstances(ID3D11DeviceContext* _context, CComPtr<ID3D11ShaderResourceView> _shader,unsigned int _numInstances, XMMATRIX* _matrixData)
{
	// Set the index buffer
	unsigned int stride = sizeof(Vector);
	unsigned int offset = 0;
	_context->IASetVertexBuffers(0, 1, &m_vertexBuffer.p, &stride, &offset);
	_context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	_context->VSSetConstantBuffers(0, 1, &m_instanceBuffer.p);

	// Set the shader resource - for the texture
	_context->PSSetShaderResources(0, 1, &m_shaderResourceView.p);
	
	if (_shader.p != nullptr)
		_context->PSSetShaderResources(1, 1, &_shader.p);
	
	// Constant buffer mapping
	D3D11_MAPPED_SUBRESOURCE mapSubresource;
    ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	_context->Map(m_instanceBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &(*_matrixData), sizeof(XMMATRIX)*_numInstances);
	_context->Unmap(m_instanceBuffer, NULL);
	
	_context->DrawIndexedInstanced(m_numIndicies,_numInstances, 0, 0, 0);

}

void Object::SetShaderResourceView(CComPtr<ID3D11ShaderResourceView> _shader)
{
	m_shaderResourceView = _shader;
}