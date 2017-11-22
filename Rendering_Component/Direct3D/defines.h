#pragma once
// Defines
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h> 
// For smart pointers
#include <atlbase.h>
#include "XTime.h"
#include "DDSTextureLoader.h"
// Shaders
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include  "OBJECT_VS.csh"
#include  "OBJECT_PS.csh"
#include  "NORMALMAPPED_OBJ_VS.csh"
#include  "NORMALMAPPED_OBJ_PS.csh"
#include  "INSTANCING_VS.csh"
#include  "POST_VS.csh"
#include  "POST_PS.csh"
// Namespace defines
using namespace DirectX;
using namespace std;
// Loading libraries
#pragma comment (lib, "d3d11.lib") 

// Defines
#define BACKBUFFER_WIDTH	1000//1920
#define BACKBUFFER_HEIGHT   900//1080
#define WINDOW_POSX         0
#define WINDOW_POSY         0
const float SCREEN_ZFAR =   100.0f;
const float SCREEN_ZNEAR =  0.01f;
#define STAR_VERTS          12
#define STAR_INDICIES       60
#define INSTANCES           30

// Structures
struct Color
{
	union{
		struct
		{
		float r;
		float g;
		float b;
		float a;
		};

		float color[4];
	};
	
	float* GetColor()
	{
		return color;
	}
};
struct Vector
{
	XMFLOAT3 transform;
	XMFLOAT3 normals;
	XMFLOAT3 uv;
	XMFLOAT3 tangents;
	XMFLOAT3 bitangents;
	XMFLOAT3 shine;
};
struct SVector
{
	XMFLOAT3 transform;
	Color color;
	XMFLOAT3 normals;
	XMFLOAT3 uv;
};
struct OBJECT_TO_VRAM
{
	XMMATRIX worldMatrix;
};
struct SCENE_TO_VRAM
{
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMFLOAT3 cameraPosition;
};
struct LIGHT_TO_VRAM
{
	XMVECTOR transform;
	XMFLOAT3 direction;
	float    radius;
	Color    color;
	bool     status;
	bool     padding[3];
};

// Casses
class Object
{
	// Graphics variables
	CComPtr<ID3D11Buffer>              m_indexBuffer;
	CComPtr<ID3D11Buffer>              m_vertexBuffer;
	CComPtr<ID3D11Buffer>              m_instanceBuffer;
	CComPtr<ID3D11Texture2D>           m_texture;
	CComPtr<ID3D11ShaderResourceView>  m_shaderResourceView;
	OBJECT_TO_VRAM                     m_worldToShader;
	// Object variables
	Vector               *m_vertecies;
	unsigned int         *m_indexList;
	unsigned int          m_numVerts;
	unsigned int          m_numIndicies;
protected:
	void InitializeVector(vector<Vector>*_vert);
	void CreateVertexBuffer(ID3D11Device* _device);
	void CreateIndexBuffer(ID3D11Device* _device);
	void ComputeTangents();
public:
	Object();
	~Object();
	void Instantiate(ID3D11Device* _device, std::string _filePath, XMFLOAT3 _position, float _shine);
	void Render(ID3D11DeviceContext* _context, ID3D11Buffer* _constBuffer, CComPtr<ID3D11ShaderResourceView> _shader = nullptr);
	bool ReadObject(std::string _filePath, float _shine);
	void TextureObject(ID3D11Device* _device, const wchar_t*  _filePath);
	XMMATRIX GetWorldMatrix();
	void SetWorldMatrix(XMMATRIX& _matrix);
	void SetPosition(float _x, float _y, float _z);
	void CreateInstanceBuffer(ID3D11Device* _device, unsigned int _numberObjs, XMMATRIX* _positions);
	void RenderInstances(ID3D11DeviceContext* _context, CComPtr<ID3D11ShaderResourceView> _shader, unsigned int _numInstances, XMMATRIX* _matrixData);
	void SetShaderResourceView(CComPtr<ID3D11ShaderResourceView> _shader);
};