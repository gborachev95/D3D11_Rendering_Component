// Includes
#include "defines.h"
#include "math.h"

// Testing

// Globals
CComPtr<IDXGISwapChain>           Swapchain                  = NULL;
CComPtr<ID3D11Device>             Device                     = NULL;
CComPtr<ID3D11DeviceContext>      DeviceContext              = NULL;

CComPtr<ID3D11DepthStencilView>   depthView                  = NULL;
CComPtr<ID3D11Texture2D>          zBuffer                    = NULL;
CComPtr<ID3D11ShaderResourceView> zShaderToResourceView      = NULL;
CComPtr<ID3D11RenderTargetView>   RenderTargetViewHolder     = NULL;

CComPtr<ID3D11RenderTargetView>   RenderTargetViewToTexture  = NULL;
CComPtr<ID3D11ShaderResourceView> renderToShaderResourceView = NULL;
CComPtr<ID3D11DepthStencilView>   depthRenderToTextureView   = NULL;
CComPtr<ID3D11Texture2D>          zRendertoTextureBuffer     = NULL;
CComPtr<ID3D11Texture2D>          renderToTexture            = NULL;

CComPtr<ID3D11RenderTargetView>   postRenderTargetView       = NULL;
CComPtr<ID3D11ShaderResourceView> postShaderResourceView     = NULL;
CComPtr<ID3D11DepthStencilView>   postdepthView              = NULL;
CComPtr<ID3D11Texture2D>          postZTexture               = NULL;
CComPtr<ID3D11Texture2D>          postTexture                = NULL;

HWND							  window;
D3D11_VIEWPORT                    viewPort;
SCENE_TO_VRAM                     viewToShader;
SCENE_TO_VRAM                     renderToTargetView;
// Prototypes
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );

// Demo app class
class DEMO_APP
{
	XTime                             Timer;
	// Window handles				  
	HINSTANCE						  application;
	WNDPROC							  appWndProc;
	// D3D11 Graphics handles		  
	CComPtr<ID3D11Buffer>             vertexStarBuffer;
	CComPtr<ID3D11Buffer>             vertexSkyBuffer;
	CComPtr<ID3D11Buffer>             constBuffer;
	CComPtr<ID3D11Buffer>             constBufferScene;
	CComPtr<ID3D11Buffer>             constBufferDirLight;
	CComPtr<ID3D11Buffer>             constBufferPointLight;
	CComPtr<ID3D11Buffer>             constBufferSpotLight;
	CComPtr<ID3D11Buffer>             postConstBuffer;
	CComPtr<ID3D11Buffer>             indexStarBuffer;
	CComPtr<ID3D11Buffer>             indexSkyBuffer;
	CComPtr<ID3D11Buffer>             quadPostBuffer;
	CComPtr<ID3D11Buffer>             quadPostIndexBuffer;
	CComPtr<ID3D11VertexShader>       VS_Shader;
	CComPtr<ID3D11PixelShader>        PS_Shader;
	CComPtr<ID3D11VertexShader>       VS_ShaderObject;
	CComPtr<ID3D11PixelShader>        PS_ShaderObject;
	CComPtr<ID3D11VertexShader>       VS_ShaderBumpObject;
	CComPtr<ID3D11PixelShader>        PS_ShaderBumpObject;
	CComPtr<ID3D11VertexShader>       VS_ShaderInstancing;
	CComPtr<ID3D11PixelShader>        PS_PostProcessing;
	CComPtr<ID3D11VertexShader>       VS_PostProcessing;
	CComPtr<ID3D11InputLayout>        inputLayout;
	CComPtr<ID3D11InputLayout>        inputLayoutObject;
	CComPtr<ID3D11InputLayout>        inputLayoutNorObject;
	CComPtr<ID3D11BlendState>         blendClassicState;
	CComPtr<ID3D11BlendState>         blendTransparentState;
	CComPtr<ID3D11DepthStencilState>  depthState;
	CComPtr<ID3D11SamplerState>       samplerState;
	CComPtr<ID3D11ShaderResourceView> skyShaderResourceView;
	CComPtr<ID3D11ShaderResourceView> mainBumpShaderResourceView;
	CComPtr<ID3D11ShaderResourceView> stormBumpShaderResourceView;
	CComPtr<ID3D11ShaderResourceView> pyrBumpShaderResourceView;
	CComPtr<ID3D11ShaderResourceView> kataShaderResourceView;

	// Geometry 
	SVector                          vertStarPoints[STAR_VERTS];
	SVector                          vertSkyPoints[8];

	// Variables send using Constant buffers
	OBJECT_TO_VRAM                  starToShader;
	OBJECT_TO_VRAM                  skyToShader;

	LIGHT_TO_VRAM                   dirLightToShader;
	LIGHT_TO_VRAM                   pointLightToShader;
	LIGHT_TO_VRAM                   spotLightToShader;

	// Quick fixes
	float xForce = 0.005f;
	float yForce = 0.0005f;
	float zForce = 0.005f;
	POINT oldMousePos;
	bool keyPressed = false;
	bool FULL_SCREEN = false;
	XMMATRIX dirLightMatrix;
	XMMATRIX instancedObjects[INSTANCES];
	float xSpot = 0.0005f;
	float zSpotdir = 1;
	bool changeDir = false;
	float dataToShader[2];

	// Loaded Objets
	Object pyrObj;
	Object pointLightObj;
	Object eyeCube;
	Object eyeCube1;
	Object eyeCube2;
	Object groundObj;
	Object mirrorObj;
	Object katarinaObj;
	Object pirateObj;
	Object appleObj;
	Object stormtrooperObj;

public:
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
	// Methods
	void Render();
	void RenderStar();
	void RenderSky();
	void RenderObjects();
	void ClearScreen(Color _color);
	void InitializeStarVerts();
	void InitializeToShader();
	void InitilizeLights();
	void Update();
	void Input();
	void Bounce();
	void CreateStarIndexBuffer();
	void CreateConstBuffers();
	void CreateDepthBuffer();
	void CreateLayouts();
	void CreateShaders();
	void CreateStarVertexBuffer();
	void CreateTheWindow(HINSTANCE _hinst, WNDPROC _proc);
	void CreateViewPorts();
	void LoadObjects();
	void FPCameraMovement(float _speed);
	void CreateSwapChainNBackBuffer();
	void CreateSamplerState();
	void CreateSkyIndexBuffer();
	void InitializeSkyVerts();
	void CreateSkyVertexBuffer();
	void InstancingObjects();
	void LightUpdate();
	void InitilizeBlendStates();
	void RenderToTexture();
	void InitilizeRenderToTexture();
	void RenderWeirdObjects();
	void PostProcessingCreation();
	void PostRender();
	void CreatePostQuad();
};

// Resize Window
void ResizeWindow(unsigned int _width, unsigned int _height)
{
	// Resizing the depth buffer
	zBuffer.Release();
	depthView.Release();
	RenderTargetViewHolder.Release();

	Swapchain->ResizeBuffers(1, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	Device->CreateTexture2D(&textureDesc, nullptr, &zBuffer.p);

	// Creation of the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;
	Device->CreateDepthStencilView(zBuffer.p, nullptr, &depthView.p);

	// Resizing the viewport
	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.Width = float(_width);
	viewPort.Height = float(_height);
	viewPort.MaxDepth = 1;

	// Setting the backbuffer
	ID3D11Texture2D *backBuffer;
	Swapchain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));
	// Creating the render view
	Device->CreateRenderTargetView(backBuffer, NULL, &RenderTargetViewHolder.p);

	// It releases the back buffer from data so it can take new data
	backBuffer->Release();

	// Setting the projection matrix
	viewToShader.projectionMatrix = XMMatrixPerspectiveFovLH(45, float(_width) / float(_height), SCREEN_ZNEAR, SCREEN_ZFAR);
}

void ResizeTextures(unsigned int _width, unsigned int _height)
{
	// Releasing
	renderToTexture.Release();
	RenderTargetViewToTexture.Release();
	renderToShaderResourceView.Release();
	depthRenderToTextureView.Release();
	zRendertoTextureBuffer.Release();

	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	Device->CreateTexture2D(&textureDesc, NULL, &renderToTexture.p);
	
	// Render target 
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Device->CreateRenderTargetView(renderToTexture, NULL, &RenderTargetViewToTexture.p);
	
	Device->CreateShaderResourceView(renderToTexture, NULL, &renderToShaderResourceView);


	// Creation of the texture for depth buffer
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// Creation of the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	// Creation of the texture for render to texture
	Device->CreateTexture2D(&textureDesc, nullptr, &zRendertoTextureBuffer.p);
	// Creation of the depth stencil view
	Device->CreateDepthStencilView(zRendertoTextureBuffer.p, &depthDesc, &depthRenderToTextureView.p);
}

void ResizePPTexture(unsigned int _width, unsigned int _height)
{
	// Releasing
	postTexture.Release();
	postRenderTargetView.Release();
	postShaderResourceView.Release();
	postdepthView.Release();
	postZTexture.Release();
	zShaderToResourceView.Release();

	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	Device->CreateTexture2D(&textureDesc, NULL, &postTexture.p);

	// Render target 
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Device->CreateRenderTargetView(postTexture, NULL, &postRenderTargetView.p);

	Device->CreateShaderResourceView(postTexture, NULL, &postShaderResourceView);


	// Creation of the texture for depth buffer
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;//DXGI_FORMAT_R32_TYPELESS;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// Creation of the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// Creation of the texture for render to texture
	Device->CreateTexture2D(&textureDesc, nullptr, &postZTexture.p);
	// Creation of the depth stencil view
	Device->CreateDepthStencilView(postZTexture.p, &depthDesc, &postdepthView.p);

	Device->CreateShaderResourceView(postZTexture, &srvDesc, &zShaderToResourceView.p);
}
// Main loop 
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run() )
    {	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }
	myApp.ShutDown(); 
	return 0; 
}

// Callback function
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
    switch ( message )
    {
        case ( WM_DESTROY ):{ PostQuitMessage( 0 ); }
        break;
		case WM_SIZE:
		{
			if (Swapchain)
			{
				// Getting the new height and width
				float newWidth = LOWORD(lParam);
				float newHeight = HIWORD(lParam);

				ResizeWindow(unsigned int(newWidth), unsigned int(newHeight));

				ResizeTextures(unsigned int(newWidth), unsigned int(newHeight));

				ResizePPTexture(unsigned int(newWidth), unsigned int(newHeight));

			}

			break;
		}
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}

// Constructor
DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	// Creating the window
	CreateTheWindow(hinst, proc);

	// Creates the swapchain and back buffer
	CreateSwapChainNBackBuffer();

	// Creates the view ports
	CreateViewPorts();

    // Loads objects from obj
	LoadObjects();

	// Initilizing verts
	InitializeStarVerts();
	InitializeSkyVerts();

	// Creating the vertex buffer for the star
	CreateStarVertexBuffer();
	CreateSkyVertexBuffer();

	// Creating the shaders for the objects and the star
	CreateShaders();

	// Creating the layouts
	CreateLayouts();

	// Creating const buffers
	CreateConstBuffers();

	// Creating the sampler state
	CreateSamplerState();

	// Initilizing index buffer
	CreateStarIndexBuffer();
	CreateSkyIndexBuffer();

	// Initilizing data that is being send to the  VS_Shader
	InitializeToShader();

	// Initilizing the depth stencil view
	CreateDepthBuffer();

	// Initializing the start mouse position
	GetCursorPos(&oldMousePos);

	// Sets the directional light matrix to identity
	XMMatrixIsIdentity(dirLightMatrix);

	// Initializes matricies of objects
	InstancingObjects();

	// Initilizes blend states
	InitilizeBlendStates();

	// Initilizes render to texture texture
	InitilizeRenderToTexture();

	CreatePostQuad();

	PostProcessingCreation();
}

// Creates the window
void DEMO_APP::CreateTheWindow(HINSTANCE _hinst, WNDPROC _proc)
{
	// Window setting up
	application = _hinst;
	appWndProc = _proc;
	// Setting up the window
	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = appWndProc;
	wndClass.hInstance = application;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	RegisterClassEx(&wndClass);

	// Window size rectangle
	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);
	// Creating the window
	window = CreateWindow(L"DirectXApplication", L"Graphics Project", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,
		window_size.right - window_size.left, window_size.bottom - window_size.top,NULL, NULL, application, this);
	ShowWindow(window, SW_SHOW);
}

// Creates the swap chain and back buffer
void DEMO_APP::CreateSwapChainNBackBuffer()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// Creating the swap chain and the device
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set the buffer parameters
	DXGI_MODE_DESC buffer;
	ZeroMemory(&buffer, sizeof(DXGI_MODE_DESC));
	buffer.Height = BACKBUFFER_HEIGHT;
	buffer.Width = BACKBUFFER_WIDTH;
	buffer.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the swap chain desc parameters
	swapChainDesc.BufferDesc = buffer;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create the device and swap chain
	HRESULT hresultHandle = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &Swapchain, &Device, NULL, &DeviceContext);

	// Setting the backbuffer
	ID3D11Texture2D *backBuffer;
	Swapchain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));

	// Creating the render view
	Device->CreateRenderTargetView(backBuffer, NULL, &RenderTargetViewHolder.p);

	// It releases the back buffer from data so it can take new data
	backBuffer->Release();
}

// Destruction - has to be called when the program ends
bool DEMO_APP::ShutDown()
{
	// Leak detection here if no smart pointers
	//#if defined(DEBUG) || defined (_DEBUG)
	//	ID3D11Debug* d3dDebug = nullptr;
	//	HRESULT hr = Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(d3dDebug));
	//	if (SUCCEEDED(hr))
	//		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//#endif
	UnregisterClass(L"DirectXApplication", application);
	return true;
}

/* STAR */
// Initializes vertecies for the star
void DEMO_APP::InitializeStarVerts()
{
	ZeroMemory(&vertStarPoints, sizeof(SVector) * STAR_VERTS);
	// Setting all other points
	for (int i = 1, degree = 0; i < STAR_VERTS - 1; ++i, degree -= 36)
	{
		if (i % 2 == 0)
		{
			vertStarPoints[i].transform.x = cosf(Deg2Rad(float(degree))) * 0.5f;
			vertStarPoints[i].transform.y = sinf(Deg2Rad(float(degree))) * 0.5f;
		}
		else
		{
			vertStarPoints[i].transform.x = cosf(Deg2Rad(float(degree)));
			vertStarPoints[i].transform.y = sinf(Deg2Rad(float(degree)));
		}
		vertStarPoints[i].color.g = 0.3f;
		vertStarPoints[i].color.r = 0.3f;
		vertStarPoints[i].color.a = 1.0f;

		vertStarPoints[i].normals = XMFLOAT3(0, 0, 0);
	}
	// Setting the center points
	vertStarPoints[0].transform.z = -0.3f;
	vertStarPoints[11].transform.z = 0.3f;
	vertStarPoints[11].color.r = 1.0f;
	vertStarPoints[11].color.g = 1.0f;
	vertStarPoints[11].color.a = 1.0f;
	vertStarPoints[0].color.r = 1.0f;
	vertStarPoints[0].color.g = 1.0f;
	vertStarPoints[0].color.a = 1.0f;
}

// Creates the vertex buffer for the indexed object
void DEMO_APP::CreateStarVertexBuffer()
{
	// Create the vertex buffer storing vertsPoints
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(SVector) * STAR_VERTS;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.StructureByteStride = sizeof(SVector);

	// Setting the resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = &vertStarPoints;
	Device->CreateBuffer(&bufferDesc, &resourceData, &vertexStarBuffer.p);
}

// Creates index buffer
void DEMO_APP::CreateStarIndexBuffer()
{
	// Create indices.
	unsigned int indices[STAR_INDICIES] = { 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 1, 0,
		1, 10, 11, 10, 9, 11, 9, 8, 11, 8, 7, 11, 7, 6, 11, 6, 5, 11, 5, 4, 11, 4, 3, 11, 3, 2, 11, 2, 1, 11 };

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(unsigned int) * STAR_INDICIES;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = indices;

	// Create the buffer with the device.
	Device->CreateBuffer(&bufferDesc, &initData, &indexStarBuffer.p);
}

/* SKYBOX */
// Initializes vertecies for the skybox
void DEMO_APP::InitializeSkyVerts()
{
	ZeroMemory(&vertSkyPoints, sizeof(SVector) * 8);
	// Setting all other points
	vertSkyPoints[0].transform = XMFLOAT3(-0.5, -0.5, 0.5);
	vertSkyPoints[1].transform = XMFLOAT3(0.5, -0.5, 0.5);
	vertSkyPoints[2].transform = XMFLOAT3(0.5, 0.5, 0.5);
	vertSkyPoints[3].transform = XMFLOAT3(-0.5, 0.5, 0.5);
	vertSkyPoints[4].transform = XMFLOAT3(0.5, 0.5, -0.5);
	vertSkyPoints[5].transform = XMFLOAT3(-0.5, 0.5, -0.5);
	vertSkyPoints[6].transform = XMFLOAT3(0.5, -0.5, -0.5);
	vertSkyPoints[7].transform = XMFLOAT3(-0.5, -0.5, -0.5);
	// Hacking the tag
	vertSkyPoints[0].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[1].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[2].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[3].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[4].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[5].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[6].normals = XMFLOAT3(1, 1, 1);
	vertSkyPoints[7].normals = XMFLOAT3(1, 1, 1);
}

// Creates the vertex buffer for the indexed object
void DEMO_APP::CreateSkyVertexBuffer()
{
	// Create the vertex buffer storing vertsPoints
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(SVector) * 8;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.StructureByteStride = sizeof(SVector);

	// Setting the resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = &vertSkyPoints;
	Device->CreateBuffer(&bufferDesc, &resourceData, &vertexSkyBuffer.p);
}

// Creates index buffer
void DEMO_APP::CreateSkyIndexBuffer()
{
	// Create indices.
	unsigned int indices[36] = { 5, 3, 7, 3, 0, 7, 2, 4, 1, 4, 6, 1, 0, 1, 7, 1, 6, 7, 7, 6, 5, 6, 4, 5, 5, 4, 3, 4, 2, 3, 3, 2, 0, 2, 1, 0 };
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 36;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = indices;

	// Create the buffer with the device.
	Device->CreateBuffer(&bufferDesc, &initData, &indexSkyBuffer.p);
}

// Creates index and vertex buffer for Post quad
void DEMO_APP::CreatePostQuad()
{
	SVector quadPoints[4];
	ZeroMemory(&quadPoints, sizeof(SVector)* 4);
	quadPoints[3].transform = XMFLOAT3(-1, -1, 0.1f);
	quadPoints[2].transform = XMFLOAT3(1, -1, 0.1f);
	quadPoints[1].transform = XMFLOAT3(1, 1, 0.1f);
	quadPoints[0].transform = XMFLOAT3(-1, 1, 0.1f);
		
	quadPoints[0].uv = XMFLOAT3(1.0f, 0.0f, 0);
	quadPoints[1].uv = XMFLOAT3(0.0f, 0, 0);
	quadPoints[2].uv = XMFLOAT3(0.0f, 1.0f, 0);
	quadPoints[3].uv = XMFLOAT3(1.0f, 1.0f, 0);

	// Create the vertex buffer storing vertsPoints
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(SVector) * 4;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.StructureByteStride = sizeof(SVector);

	// Setting the resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = &quadPoints;
	Device->CreateBuffer(&bufferDesc, &resourceData, &quadPostBuffer.p);

	// Create indices.
	unsigned int indices[6] = { 0, 2, 3, 0, 1, 2 };
	//320210

	// Fill in a buffer description.
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 6;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = indices;

	// Create the buffer with the device.
	Device->CreateBuffer(&bufferDesc, &initData, &quadPostIndexBuffer.p);
}

// Clears the screen
void DEMO_APP::ClearScreen(Color _color)
{
	DeviceContext->ClearRenderTargetView(postRenderTargetView.p, _color.GetColor());//DeviceContext->ClearRenderTargetView(RenderTargetViewHolder.p, _color.GetColor());
	DeviceContext->ClearDepthStencilView(postdepthView.p, D3D11_CLEAR_DEPTH, 1, 0);//DeviceContext->ClearDepthStencilView(depthView.p, D3D11_CLEAR_DEPTH, 1, 0);
}

// Initializes data that will be going to the shader
void DEMO_APP::InitializeToShader()
{
	// Setting the shaders
	// Setting Star matrix of start
	starToShader.worldMatrix = XMMatrixIdentity();
	starToShader.worldMatrix.r[3].m128_f32[1] = 4.0f;
	starToShader.worldMatrix.r[3].m128_f32[2] = 3.0f;

	// Setting the projection matrix
	viewToShader.projectionMatrix = XMMatrixPerspectiveFovLH(45, BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT, SCREEN_ZNEAR, SCREEN_ZFAR);

	// Setting the View matrix
	viewToShader.viewMatrix = XMMatrixIdentity();
	viewToShader.viewMatrix.r[3].m128_f32[1] = 2.0f;
	viewToShader.viewMatrix.r[3].m128_f32[2] = -7.0f;

	// Setting Sky matrix of start
	skyToShader.worldMatrix = XMMatrixIdentity();
	skyToShader.worldMatrix.r[3] = viewToShader.viewMatrix.r[3];
	viewToShader.viewMatrix = XMMatrixInverse(nullptr, viewToShader.viewMatrix);

	// Initilizes lights data
	InitilizeLights();

	// Initilizeing render to target camera
	renderToTargetView.viewMatrix = XMMatrixIdentity();
	renderToTargetView.projectionMatrix = XMMatrixPerspectiveFovLH(120, BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT, SCREEN_ZNEAR, SCREEN_ZFAR);
    renderToTargetView.viewMatrix.r[3].m128_f32[1] = 25.0f;
	renderToTargetView.viewMatrix.r[3].m128_f32[2] = 20.0f;
	renderToTargetView.viewMatrix = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)), renderToTargetView.viewMatrix);
	renderToTargetView.viewMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(90)), renderToTargetView.viewMatrix);
	renderToTargetView.viewMatrix = XMMatrixInverse(nullptr, renderToTargetView.viewMatrix);
}

// Initilizes the light for the shader
void DEMO_APP::InitilizeLights()
{
	// Direction light
	ZeroMemory(&dirLightToShader, sizeof(LIGHT_TO_VRAM));
	// Setting the color 
	dirLightToShader.color.r = 1.0f;
	dirLightToShader.color.g = 1.0f;
	dirLightToShader.color.b = 1.0f;
	dirLightToShader.color.a = 1.0f;
	// Setting the direction
	dirLightToShader.direction.x = -1.0f;
	dirLightToShader.direction.y = -0.5f;
	dirLightToShader.status = true;

	// Point light
	ZeroMemory(&pointLightToShader, sizeof(LIGHT_TO_VRAM));
	// Setting the color 
	pointLightToShader.color.r = 1.0f;
	pointLightToShader.color.g = 1.0f;
	pointLightToShader.color.b = 1.0f;
	pointLightToShader.color.a = 1.0f;
	pointLightToShader.transform.m128_f32[1] =  2.0f;

	// Spot light
	ZeroMemory(&spotLightToShader, sizeof(LIGHT_TO_VRAM));
	spotLightToShader.direction.z = 1;
	spotLightToShader.radius = 0.9238f;
	spotLightToShader.transform.m128_f32[1] = 4;
	spotLightToShader.color.r = 1.0f;
	spotLightToShader.color.g = 1.0f;
	spotLightToShader.color.b = 1.0f;
	spotLightToShader.color.a = 1.0f;
}

// Initilizes blend states
void DEMO_APP::InitilizeBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	Device->CreateBlendState(&blendDesc, &blendTransparentState.p);

	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Device->CreateBlendState(&blendDesc, &blendClassicState.p);
}

// Initilizes matricies of objects that are going to be instanciated
void DEMO_APP::InstancingObjects()
{
	float distanceX = -8.0f;
	float distanceZ = 3.0f;
	for (int i = 0; i < INSTANCES; ++i, distanceX += 2.0f)
	{
		
		// No one is in the middle
		if (distanceX == 0.0f)// || distanceX == 2.0f || distanceX == -2.0f)
		{
			--i;
			continue;
		}

		// Setting matrix proper position
		instancedObjects[i] = XMMatrixIdentity();
		instancedObjects[i].r[3].m128_f32[0] = distanceX;
		instancedObjects[i].r[3].m128_f32[1] = -0.5f;
		instancedObjects[i].r[3].m128_f32[2] = distanceZ;
	
		// Rotating only the closest colums
		if (distanceX == 2.0f)
			instancedObjects[i] = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(90)), instancedObjects[i]);
		else if (distanceX == -2.0f)
			instancedObjects[i] = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(-90)), instancedObjects[i]);

		// Going to next row
		if (distanceX >= 8.0f)
		{
			distanceX = -10.0f;
			distanceZ += 2.0f;
		}
	}
	stormtrooperObj.CreateInstanceBuffer(Device, INSTANCES, instancedObjects);
}

// Creates constant buffer
void DEMO_APP::CreateConstBuffers()
{
	// Creating the constant buffer for the world
	D3D11_BUFFER_DESC constBufferDesc;
	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = sizeof(OBJECT_TO_VRAM);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &constBuffer.p);

	// Creating const buffer for the scene
	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = sizeof(SCENE_TO_VRAM);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &constBufferScene.p);

	// Creating const buffer for the lights
	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = sizeof(LIGHT_TO_VRAM);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &constBufferDirLight.p);

	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = sizeof(LIGHT_TO_VRAM);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &constBufferPointLight.p);

	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = sizeof(LIGHT_TO_VRAM);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &constBufferSpotLight.p);
	
	ZeroMemory(&constBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constBufferDesc.ByteWidth = 16;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.StructureByteStride = sizeof(float);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&constBufferDesc, NULL, &postConstBuffer.p);

}

// Creates depth buffer
void DEMO_APP::CreateDepthBuffer()
{
	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = BACKBUFFER_WIDTH;
	textureDesc.Height = BACKBUFFER_HEIGHT;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;//DXGI_FORMAT_R32_TYPELESS;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	Device->CreateTexture2D(&textureDesc, nullptr, &zBuffer.p);

	// Creation of the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	Device->CreateDepthStencilView(zBuffer.p, &depthDesc, &depthView.p);

	// RENDER TO TEXTURE
	// Creation of the texture for render to texture
	Device->CreateTexture2D(&textureDesc, nullptr, &zRendertoTextureBuffer.p);
	// Creation of the depth stencil view
	Device->CreateDepthStencilView(zRendertoTextureBuffer.p, &depthDesc, &depthRenderToTextureView.p);

	// POST
	// Creation of the texture for render to texture
	Device->CreateTexture2D(&textureDesc, nullptr, &postZTexture.p);
	// Creation of the depth stencil view
	Device->CreateDepthStencilView(postZTexture.p, &depthDesc, &postdepthView.p);

	Device->CreateShaderResourceView(postZTexture, &srvDesc, &zShaderToResourceView.p);


	// USED FOR TRANSPARENCY
	// Creating the depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	ZeroMemory(&depthStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStateDesc.StencilEnable = TRUE;
	depthStateDesc.DepthEnable = TRUE;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; 
	depthStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Device->CreateDepthStencilState(&depthStateDesc, &depthState.p);
}

// Creates the sampler state that goes to the pixel shader texture
void DEMO_APP::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	Device->CreateSamplerState(&samplerDesc, &samplerState.p);
}

// Creates the layouts
void DEMO_APP::CreateLayouts()
{
	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3D11_INPUT_ELEMENT_DESC vLayoutObject[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SHINE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLDPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "CAMERA_POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "LIGHT_PROJ", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	// Creating layout for trival shader
	Device->CreateInputLayout(vLayout, ARRAYSIZE(vLayout), Trivial_VS, sizeof(Trivial_VS), &inputLayout.p);
	// Creating layout for object shader
	Device->CreateInputLayout(vLayoutObject, ARRAYSIZE(vLayoutObject), OBJECT_VS, sizeof(OBJECT_VS), &inputLayoutObject.p);
	// Creating layout for normal mapped objects
	Device->CreateInputLayout(vLayoutObject, ARRAYSIZE(vLayoutObject), NORMALMAPPED_OBJ_VS, sizeof(NORMALMAPPED_OBJ_VS), &inputLayoutNorObject.p);
}

// Creates shaders
void DEMO_APP::CreateShaders()
{
	// Creates the shader for the star - simple color
	Device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &VS_Shader.p);
	Device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &PS_Shader.p);

	// Creates the shaders for the Object
	Device->CreateVertexShader(OBJECT_VS, sizeof(OBJECT_VS), nullptr, &VS_ShaderObject.p);
	Device->CreatePixelShader(OBJECT_PS, sizeof(OBJECT_PS), nullptr, &PS_ShaderObject.p);

	// Creates the shaders for the Bumped objects
	Device->CreateVertexShader(NORMALMAPPED_OBJ_VS, sizeof(NORMALMAPPED_OBJ_VS), nullptr, &VS_ShaderBumpObject.p);
    Device->CreatePixelShader(NORMALMAPPED_OBJ_PS, sizeof(NORMALMAPPED_OBJ_PS), nullptr, &PS_ShaderBumpObject.p);

	// Creates the shader for the instanced objects
	Device->CreateVertexShader(INSTANCING_VS, sizeof(INSTANCING_VS), nullptr, &VS_ShaderInstancing.p);

	// Creates the shaders for the post-processing
	Device->CreateVertexShader(POST_VS, sizeof(POST_VS), nullptr, &VS_PostProcessing.p);
	Device->CreatePixelShader(POST_PS, sizeof(POST_PS), nullptr, &PS_PostProcessing.p);
}

// Create view ports
void DEMO_APP::CreateViewPorts()
{
	// Creating the view port
	ZeroMemory(&viewPort, sizeof(viewPort));
	//viewPort.TopLeftX = BACKBUFFER_WIDTH / 2;
	//viewPort.TopLeftY = 10;
	viewPort.Width = BACKBUFFER_WIDTH;
	viewPort.Height = BACKBUFFER_HEIGHT;
	viewPort.MaxDepth = 1;
}

// Loads objects from files
void DEMO_APP::LoadObjects()
{
	// Texture skybox
	/*
	Skybox texture by Jockum Skoglund a.k.a hipshot
	hipshot@zfight.com
	www.zfight.com
	Stockholm, 2005 08 25
	*/
	CreateDDSTextureFromFile(Device, L"..\\Direct3D\\Textures\\Jockum Skoglund.dds", NULL, &skyShaderResourceView.p);

	// Load 1
	XMFLOAT3 pyrPos{ 0, -0.45f, 20.0f };
	pyrObj.Instantiate(Device, "..\\Direct3D\\Meshes\\test pyramid2.obj", pyrPos,0.2f);
	pyrObj.TextureObject(Device, L"..\\Direct3D\\Textures\\BlueGrungeTexture.dds");

	// Load 2
	XMFLOAT3 lightPos{ -10.5f, 0, 0.0f };
	pointLightObj.Instantiate(Device, "..\\Direct3D\\Meshes\\point_light.obj", lightPos,0);
	pointLightObj.TextureObject(Device, L"..\\Direct3D\\Textures\\eyeTexture.dds");
	XMMATRIX rotate = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180)), pointLightObj.GetWorldMatrix());
	pointLightObj.SetWorldMatrix(rotate);

	// Load 3
	XMFLOAT3 eyeBox{ 0, 4, 10 };
	eyeCube.Instantiate(Device, "..\\Direct3D\\Meshes\\Cube.obj", eyeBox, 1);
	eyeCube.TextureObject(Device, L"..\\Direct3D\\Textures\\glassTexture.dds");
	// Load 4
	XMFLOAT3 eyeBox2{ -3, 4, 10 };
	eyeCube1.Instantiate(Device, "..\\Direct3D\\Meshes\\Cube.obj", eyeBox2, 1);
	eyeCube1.TextureObject(Device, L"..\\Direct3D\\Textures\\glassTexture.dds");
	// Load 5
	XMFLOAT3 eyeBox3{ 3, 4, 10 };
	eyeCube2.Instantiate(Device, "..\\Direct3D\\Meshes\\Cube.obj", eyeBox3, 1);
	eyeCube2.TextureObject(Device, L"..\\Direct3D\\Textures\\glassTexture.dds");

	// Load 6
	XMFLOAT3 ground{ 0, -0.5f, 0 };
	groundObj.Instantiate(Device, "..\\Direct3D\\Meshes\\ground.obj", ground,1);
	groundObj.TextureObject(Device, L"..\\Direct3D\\Textures\\BlueGrungeTexture.dds");

	// Load 7
	XMFLOAT3 katarinaPos{ -2.0f, -0.5f, -1.0f };
	katarinaObj.Instantiate(Device, "..\\Direct3D\\Meshes\\katarina.obj", katarinaPos,0.2f);
	katarinaObj.TextureObject(Device, L"..\\Direct3D\\Textures\\kataTexture.dds");

	// Load 8
	XMFLOAT3 katarinaPos2{ 2.0f, -0.5f, -1.0f };
	pirateObj.Instantiate(Device, "..\\Direct3D\\Meshes\\katarina.obj", katarinaPos2, 0.2f);
	pirateObj.TextureObject(Device, L"..\\Direct3D\\Textures\\katapirateTexture.dds");

	// Load 9
	XMFLOAT3 applePos{ -0.2f, 4.5f, 20.0f };
	appleObj.Instantiate(Device, "..\\Direct3D\\Meshes\\apple.obj", applePos,1);
	appleObj.TextureObject(Device, L"..\\Direct3D\\Textures\\appleTexture.dds");

	// Load 10
	XMFLOAT3 stormPos{ -2.5f, -0.5f, 2.0f };
	stormtrooperObj.Instantiate(Device, "..\\Direct3D\\Meshes\\Stormtrooper.obj", stormPos, 1);
	stormtrooperObj.TextureObject(Device, L"..\\Direct3D\\Textures\\StormtrooperTexture.dds");
	
	// Load 11
	XMFLOAT3 mirror{ 0, 0, 0 };
	mirrorObj.Instantiate(Device, "..\\Direct3D\\Meshes\\smallPlain.obj", mirror, 0);
	mirrorObj.TextureObject(Device, L"..\\Direct3D\\Textures\\BlueGrungeTexture.dds");

	// Bump resource
	CreateDDSTextureFromFile(Device, L"..\\Direct3D\\Textures\\sss2NormalMap.dds", NULL, &mainBumpShaderResourceView.p);
	CreateDDSTextureFromFile(Device, L"..\\Direct3D\\Textures\\StormtrooperNormalMap.dds", NULL, &stormBumpShaderResourceView.p);
	CreateDDSTextureFromFile(Device, L"..\\Direct3D\\Textures\\kataNormalMap .dds", NULL, &kataShaderResourceView.p);
	CreateDDSTextureFromFile(Device, L"..\\Direct3D\\Textures\\oldBrickWallNormalMap.dds", NULL, &pyrBumpShaderResourceView.p);
}

// Bounces an object using X and Y coordinates
void DEMO_APP::Bounce()
{
	// X movement
	starToShader.worldMatrix.r[3].m128_f32[0] += xForce;
	if ((starToShader.worldMatrix.r[3].m128_f32[0] > 8) || (starToShader.worldMatrix.r[3].m128_f32[0]  < -8))
		xForce = -xForce;

	// Y movement
    starToShader.worldMatrix.r[3].m128_f32[1] += yForce;
	if ((starToShader.worldMatrix.r[3].m128_f32[1] > 5) || (starToShader.worldMatrix.r[3].m128_f32[1]  < 4))
		yForce = -yForce;

	// Z movement
	starToShader.worldMatrix.r[3].m128_f32[2] += zForce;
	if ((starToShader.worldMatrix.r[3].m128_f32[2] > 12) || starToShader.worldMatrix.r[3].m128_f32[2]  < 3)
		zForce = -zForce;
	
}

// Execution of the program
bool DEMO_APP::Run()
{
	Input();
	Update();
	Render();
	return true;
}

// Gets the input from the user
void DEMO_APP::Input()
{
	// Move point light
	if (pointLightToShader.status)
	{
		if (GetAsyncKeyState(VK_DOWN))
			pointLightToShader.transform.m128_f32[2] -= float(Timer.Delta())* 2.0f;
		else if (GetAsyncKeyState(VK_UP))			
			pointLightToShader.transform.m128_f32[2] += float(Timer.Delta())* 2.0f;
		if (GetAsyncKeyState(VK_LEFT))				
			pointLightToShader.transform.m128_f32[0] += float(Timer.Delta())* 2.0f;
		else if (GetAsyncKeyState(VK_RIGHT))		
			pointLightToShader.transform.m128_f32[0] -= float(Timer.Delta())* 2.0f;
		if (GetAsyncKeyState('Z'))						
			pointLightToShader.transform.m128_f32[1] += float(Timer.Delta())* 2.0f;
		else if (GetAsyncKeyState('X'))					
			pointLightToShader.transform.m128_f32[1] -= float(Timer.Delta())* 2.0f;
	}

	// Move camera
	FPCameraMovement(float(Timer.Delta()) * 2.0f);

	// Turn on lights
	if (GetAsyncKeyState('P') && !keyPressed)
	{
		pointLightToShader.status = !pointLightToShader.status;
		keyPressed = true;
	}
	if (GetAsyncKeyState('O') && !keyPressed)
	{
		dirLightToShader.status = !dirLightToShader.status;
		keyPressed = true;
	}
	if (GetAsyncKeyState('I') && !keyPressed)
	{
		spotLightToShader.status = !spotLightToShader.status;
		keyPressed = true;
	}
	if (!GetAsyncKeyState('I') && !GetAsyncKeyState('O') && !GetAsyncKeyState('P') && keyPressed)
		keyPressed = false;

	// Effect data for post processing
	if (GetAsyncKeyState('0'))
		dataToShader[1] = 0;
	if (GetAsyncKeyState('1'))
		dataToShader[1] = 1;
	if (GetAsyncKeyState('2'))
		dataToShader[1] = 2;
	if (GetAsyncKeyState('3'))
		dataToShader[1] = 3;
	if (GetAsyncKeyState('4'))
		dataToShader[1] = 4;
	if (GetAsyncKeyState('5'))
		dataToShader[1] = 5;

}

// Updates every frame
void DEMO_APP::Update()
{
	// Signal gives pulse to the timer
	Timer.Signal();

	// Rotate the star
	starToShader.worldMatrix = XMMatrixMultiply(XMMatrixRotationY(float(Timer.Delta())), starToShader.worldMatrix);

	// Rotate apple
	XMMATRIX rotation = XMMatrixMultiply(XMMatrixRotationY(float(Timer.Delta())), appleObj.GetWorldMatrix());
	appleObj.SetWorldMatrix(rotation);

	rotation = XMMatrixMultiply(XMMatrixRotationY(float(Timer.Delta())), eyeCube.GetWorldMatrix());
	eyeCube.SetWorldMatrix(rotation);

    // Moves the star around
	Bounce();

	// Update lights
	LightUpdate();

}

// Updates the lights positions
void DEMO_APP::LightUpdate()
{
	// Updating the position of the point light
	if (pointLightToShader.status)
		pointLightObj.SetPosition(pointLightToShader.transform.m128_f32[0], pointLightToShader.transform.m128_f32[1], pointLightToShader.transform.m128_f32[2]);

	if (dirLightToShader.status)
	{
		// Rotating the directional light
		XMMATRIX rotation = XMMatrixRotationZ((float)Timer.Delta()*1.0f);//, XMMatrixRotationX((float)Timer.Delta()*-0.05f);
		dirLightMatrix.r[3].m128_f32[0] = dirLightToShader.direction.x;
		dirLightMatrix.r[3].m128_f32[1] = dirLightToShader.direction.y;
		dirLightMatrix.r[3].m128_f32[2] = dirLightToShader.direction.z;
		// Globaly rotating the direction
		dirLightMatrix = XMMatrixMultiply(dirLightMatrix, rotation);
		// Sending the values back
		dirLightToShader.direction.x = dirLightMatrix.r[3].m128_f32[0];
		dirLightToShader.direction.y = dirLightMatrix.r[3].m128_f32[1];
		dirLightToShader.direction.z = dirLightMatrix.r[3].m128_f32[2];
	}
}

// Renders objects to the scene
void DEMO_APP::Render()
{
	// Renders to textures
	RenderToTexture();

	// Setting the render target with the depth buffer
	float blendFactor[] = { 0, 0, 0, 0 };
	unsigned int sampleMask = 0xffffffff;
	//DeviceContext->OMSetBlendState(blendTransparentState, blendFactor, sampleMask);
	DeviceContext->RSSetViewports(1, &viewPort);
	DeviceContext->OMSetRenderTargets(1, &postRenderTargetView.p, postdepthView.p);// DeviceContext->OMSetRenderTargets(1, &RenderTargetViewHolder.p, depthView.p);
	//DeviceContext->OMSetDepthStencilState(depthState, 1);

	// Clearing the screen
	Color color{ 0.39f, 0.58f, 0.92f, 1 };
	ClearScreen(color);

	// Setting everything together 
	DeviceContext->VSSetShader(VS_Shader.p, NULL, NULL);
	DeviceContext->PSSetShader(PS_Shader.p, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->PSSetSamplers(0, 1, &samplerState.p);
	
	
	// Setting constant buffers - first parameter is the regester (b0) in shader
	DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer.p);
	DeviceContext->PSSetConstantBuffers(0, 1, &constBuffer.p);
	// Scene const buffer
	DeviceContext->VSSetConstantBuffers(1, 1, &constBufferScene.p);
	DeviceContext->PSSetConstantBuffers(1, 1, &constBufferScene.p);
	// Lights const buffer
	DeviceContext->VSSetConstantBuffers(2, 1, &constBufferDirLight.p);
	DeviceContext->PSSetConstantBuffers(2, 1, &constBufferDirLight.p);
	DeviceContext->VSSetConstantBuffers(3, 1, &constBufferPointLight.p);
	DeviceContext->PSSetConstantBuffers(3, 1, &constBufferPointLight.p);
	DeviceContext->VSSetConstantBuffers(4, 1, &constBufferSpotLight.p);
	DeviceContext->PSSetConstantBuffers(4, 1, &constBufferSpotLight.p);
	
	// Constant buffer mapping
	// Scene mapping
	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferScene, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &viewToShader, sizeof(SCENE_TO_VRAM));
	DeviceContext->Unmap(constBufferScene, NULL);
	// Lights mapping
	// Directional light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferDirLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &dirLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferDirLight, NULL);
	// Point light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferPointLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &pointLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferPointLight, NULL);
	// Spot light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferSpotLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &spotLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferSpotLight, NULL);

	// Rendering skybox first
	RenderSky();
	// Draw objects
	RenderStar();
	RenderObjects();
	RenderWeirdObjects();
	PostRender();
	Swapchain->Present(0, 0);
}

// Draws the star
void DEMO_APP::RenderStar()
{
	// Setting the vertex buffer to be the current
	// Stride is the movement through the vertecies
	unsigned int stride = sizeof(SVector);
	unsigned int offset = 0;
	
	// Set the buffers
	DeviceContext->IASetVertexBuffers(0, 1, &vertexStarBuffer.p, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexStarBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &starToShader, sizeof(OBJECT_TO_VRAM));
	DeviceContext->Unmap(constBuffer, NULL);
	// Drawing the star
	DeviceContext->DrawIndexed(STAR_INDICIES, 0, 0);
}

// Draws the skybox
void DEMO_APP::RenderSky()
{
	// Setting the vertex buffer to be the current
	// Stride is the movement through the vertecies
	unsigned int stride = sizeof(SVector);
	unsigned int offset = 0;

	// Set the buffers
	DeviceContext->IASetVertexBuffers(0, 1, &vertexSkyBuffer.p, &stride, &offset);
	DeviceContext->IASetIndexBuffer(indexSkyBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the shader resource - for the texture
	DeviceContext->PSSetShaderResources(0, 1, &skyShaderResourceView.p);

	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &skyToShader, sizeof(OBJECT_TO_VRAM));
	DeviceContext->Unmap(constBuffer, NULL);

	// Drawing the sky
	DeviceContext->DrawIndexed(36, 0,0);
	// Cleaning the depth buffer so it gives the infinite look
	DeviceContext->ClearDepthStencilView(postdepthView.p, D3D11_CLEAR_DEPTH, 1, 0);
}

// Calls draw on each object that is in the scene
void DEMO_APP::RenderObjects()
{
	// Setting the shaders for the object class
	DeviceContext->VSSetShader(VS_ShaderObject, NULL, NULL);
	DeviceContext->PSSetShader(PS_ShaderObject, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayoutObject);

	// Drawing objects
	if (pointLightToShader.status)
		pointLightObj.Render(DeviceContext, constBuffer);

	// Setting the shaders for the bumped objects
	DeviceContext->VSSetShader(VS_ShaderBumpObject, NULL, NULL);
	DeviceContext->PSSetShader(PS_ShaderBumpObject, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayoutNorObject);

	// Drawing normalmapped objects
	pyrObj.Render(DeviceContext, constBuffer, pyrBumpShaderResourceView);
	groundObj.Render(DeviceContext, constBuffer, mainBumpShaderResourceView);
	katarinaObj.Render(DeviceContext, constBuffer, kataShaderResourceView);
	pirateObj.Render(DeviceContext, constBuffer, kataShaderResourceView);
	appleObj.Render(DeviceContext, constBuffer);

	// Draw instanced objects
	DeviceContext->VSSetShader(VS_ShaderInstancing, NULL, NULL);
	stormtrooperObj.RenderInstances(DeviceContext, stormBumpShaderResourceView, INSTANCES,instancedObjects);

	// Draw transparent objects 
	DeviceContext->VSSetShader(VS_ShaderObject, NULL, NULL);
	DeviceContext->PSSetShader(PS_ShaderObject, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayoutObject);
	DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer.p);
	DeviceContext->PSSetConstantBuffers(0, 1, &constBuffer.p);
	eyeCube.Render(DeviceContext, constBuffer);
	eyeCube1.Render(DeviceContext, constBuffer);
	eyeCube2.Render(DeviceContext, constBuffer);
}

void DEMO_APP::RenderWeirdObjects()
{
	mirrorObj.SetShaderResourceView(renderToShaderResourceView);
	mirrorObj.Render(DeviceContext, constBuffer, mainBumpShaderResourceView);
}

// First person camera movement
void DEMO_APP::FPCameraMovement(float _speed)
{
	// Get the cursor position
	POINT mousePos;
	ZeroMemory(&mousePos, sizeof(POINT));
	GetCursorPos(&mousePos);

	// Calculating the difference of the mouse position
	float deltaX = float(oldMousePos.x - mousePos.x);
	float deltaY = float(oldMousePos.y - mousePos.y);

	// Rotating when holding left key
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		viewToShader.viewMatrix = XMMatrixInverse(0, viewToShader.viewMatrix);
		XMVECTOR storePosition;
		storePosition.m128_f32[0] = viewToShader.viewMatrix.r[3].m128_f32[0];
		storePosition.m128_f32[1] = viewToShader.viewMatrix.r[3].m128_f32[1];
		storePosition.m128_f32[2] = viewToShader.viewMatrix.r[3].m128_f32[2];

		viewToShader.viewMatrix.r[3].m128_f32[0] = 0;
		viewToShader.viewMatrix.r[3].m128_f32[1] = 0;
		viewToShader.viewMatrix.r[3].m128_f32[2] = 0;


		// Spot light
		//if (!changeDir)
		//{
		//	spotLightToShader.direction.x += deltaX*0.0005f;
		//	spotLightToShader.direction.y += deltaY*0.0005f;
		//}
		//else
		//{
		//	spotLightToShader.direction.x -= deltaX*0.0005f;
		//	spotLightToShader.direction.y -= deltaY*0.0005f;
		//}
		//if (spotLightToShader.direction.x < -3.08f || spotLightToShader.direction.x > 3.08f)
		//{
		//	spotLightToShader.direction.x = 0.0f;
		//	zSpotdir = -zSpotdir;
		//	spotLightToShader.direction.z = zSpotdir;
		//	changeDir = !changeDir;
		//}

		
		viewToShader.viewMatrix = XMMatrixMultiply(XMMatrixRotationX(-deltaY*0.0005f), viewToShader.viewMatrix);
		viewToShader.viewMatrix = XMMatrixMultiply(viewToShader.viewMatrix, XMMatrixRotationY(deltaX*0.0005f));

		viewToShader.viewMatrix.r[3].m128_f32[0] = storePosition.m128_f32[0];
		viewToShader.viewMatrix.r[3].m128_f32[1] = storePosition.m128_f32[1];
		viewToShader.viewMatrix.r[3].m128_f32[2] = storePosition.m128_f32[2];

		viewToShader.viewMatrix = XMMatrixInverse(0, viewToShader.viewMatrix);
	}

	oldMousePos = mousePos;

	if (GetAsyncKeyState('W'))
		viewToShader.viewMatrix.r[3].m128_f32[2] -= _speed;
	else if (GetAsyncKeyState('S'))
		viewToShader.viewMatrix.r[3].m128_f32[2] += _speed;
	if (GetAsyncKeyState('D'))
		viewToShader.viewMatrix.r[3].m128_f32[0] += _speed;
	else if (GetAsyncKeyState('A'))
		viewToShader.viewMatrix.r[3].m128_f32[0] -= _speed;

	// Spot light 
	viewToShader.viewMatrix = XMMatrixInverse(0, viewToShader.viewMatrix);
	spotLightToShader.transform.m128_f32[0] = viewToShader.viewMatrix.r[3].m128_f32[0];
	spotLightToShader.transform.m128_f32[1] = viewToShader.viewMatrix.r[3].m128_f32[1];
	spotLightToShader.transform.m128_f32[2] = viewToShader.viewMatrix.r[3].m128_f32[2];
	spotLightToShader.direction.x = viewToShader.viewMatrix.r[2].m128_f32[0];
	spotLightToShader.direction.y = viewToShader.viewMatrix.r[2].m128_f32[1];
	spotLightToShader.direction.z = viewToShader.viewMatrix.r[2].m128_f32[2];

	// Sending the camera position to the shaders
	viewToShader.cameraPosition.x = viewToShader.viewMatrix.r[3].m128_f32[0];
	viewToShader.cameraPosition.y = viewToShader.viewMatrix.r[3].m128_f32[1];
	viewToShader.cameraPosition.z = viewToShader.viewMatrix.r[3].m128_f32[2];
	viewToShader.viewMatrix = XMMatrixInverse(0, viewToShader.viewMatrix);

	// Setting the skybox to the position of the camera
	skyToShader.worldMatrix.r[3] = XMMatrixInverse(0, viewToShader.viewMatrix).r[3];
	skyToShader.worldMatrix.r[3].m128_f32[1] = skyToShader.worldMatrix.r[3].m128_f32[1] + 0.2f;
	
}

// Initilizes render to texture texture
void DEMO_APP::InitilizeRenderToTexture()
{
	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = BACKBUFFER_WIDTH;
	textureDesc.Height = BACKBUFFER_HEIGHT;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	Device->CreateTexture2D(&textureDesc, NULL, &renderToTexture.p);

	// Render target 
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Device->CreateRenderTargetView(renderToTexture, &renderTargetViewDesc, &RenderTargetViewToTexture.p);

	Device->CreateShaderResourceView(renderToTexture, NULL, &renderToShaderResourceView);
}

void DEMO_APP::RenderToTexture()
{
	CComPtr<ID3D11ShaderResourceView> nullShaderResourceView;
	DeviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView.p);
	DeviceContext->RSSetViewports(1, &viewPort);
	// Setting the shaders for the bumped objects
	DeviceContext->VSSetShader(VS_ShaderBumpObject, NULL, NULL);
	DeviceContext->PSSetShader(PS_ShaderBumpObject, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayoutNorObject);

	// Setting the target

	DeviceContext->OMSetRenderTargets(1, &RenderTargetViewToTexture.p, depthRenderToTextureView.p);

	// Clearing the render target
	Color color{ 0, 0.58f, 0.92f, 1 };
	DeviceContext->ClearDepthStencilView(depthRenderToTextureView.p, D3D11_CLEAR_DEPTH, 1, 0);
	DeviceContext->ClearRenderTargetView(RenderTargetViewToTexture.p, color.GetColor());

	// Setting everything together 
	DeviceContext->VSSetShader(VS_Shader.p, NULL, NULL);
	DeviceContext->PSSetShader(PS_Shader.p, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->PSSetSamplers(0, 1, &samplerState.p);

	// Setting constant buffers - first parameter is the regester (b0) in shader
	DeviceContext->VSSetConstantBuffers(0, 1, &constBuffer.p);
	DeviceContext->PSSetConstantBuffers(0, 1, &constBuffer.p);
	// Scene const buffer
	DeviceContext->VSSetConstantBuffers(1, 1, &constBufferScene.p);
	DeviceContext->PSSetConstantBuffers(1, 1, &constBufferScene.p);
	// Lights const buffer
	DeviceContext->VSSetConstantBuffers(2, 1, &constBufferDirLight.p);
	DeviceContext->PSSetConstantBuffers(2, 1, &constBufferDirLight.p);
	DeviceContext->VSSetConstantBuffers(3, 1, &constBufferPointLight.p);
	DeviceContext->PSSetConstantBuffers(3, 1, &constBufferPointLight.p);
	DeviceContext->VSSetConstantBuffers(4, 1, &constBufferSpotLight.p);
	DeviceContext->PSSetConstantBuffers(4, 1, &constBufferSpotLight.p);

	// Constant buffer mapping
	// Scene mapping
	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferScene, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &renderToTargetView, sizeof(SCENE_TO_VRAM));
	DeviceContext->Unmap(constBufferScene, NULL);
	// Lights mapping
	// Directional light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferDirLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &dirLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferDirLight, NULL);
	// Point light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferPointLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &pointLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferPointLight, NULL);
	// Spot light
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(constBufferSpotLight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &spotLightToShader, sizeof(LIGHT_TO_VRAM));
	DeviceContext->Unmap(constBufferSpotLight, NULL);

	DeviceContext->VSSetShader(VS_ShaderBumpObject, NULL, NULL);
	DeviceContext->PSSetShader(PS_ShaderBumpObject, NULL, NULL);
	DeviceContext->IASetInputLayout(inputLayoutNorObject);
	pyrObj.Render(DeviceContext, constBuffer, pyrBumpShaderResourceView);
	groundObj.Render(DeviceContext, constBuffer, mainBumpShaderResourceView);
	appleObj.Render(DeviceContext, constBuffer);

}

void DEMO_APP::PostProcessingCreation()
{  
	// Creation of the texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = BACKBUFFER_WIDTH;
	textureDesc.Height = BACKBUFFER_HEIGHT;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	Device->CreateTexture2D(&textureDesc, NULL, &postTexture.p);

	// Render target 
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Device->CreateRenderTargetView(postTexture, &renderTargetViewDesc, &postRenderTargetView.p);

	Device->CreateShaderResourceView(postTexture, NULL, &postShaderResourceView);
}

void DEMO_APP::PostRender()
{
    DeviceContext->OMSetRenderTargets(1, &RenderTargetViewHolder.p, depthView.p);
	DeviceContext->IASetInputLayout(inputLayout);
	unsigned int stride = sizeof(SVector);
	unsigned int offset = 0;

	// Set the buffers
	DeviceContext->IASetVertexBuffers(0, 1, &quadPostBuffer.p, &stride, &offset);
	DeviceContext->IASetIndexBuffer(quadPostIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DeviceContext->VSSetShader(VS_PostProcessing, NULL, NULL);
	DeviceContext->PSSetShader(PS_PostProcessing, NULL, NULL);

	// Setting constant buffers - first parameter is the regester (b0) in shader
	DeviceContext->VSSetConstantBuffers(0, 1, &postConstBuffer.p);
	DeviceContext->PSSetConstantBuffers(0, 1, &postConstBuffer.p);

	// Set the shader resource - for the texture
	DeviceContext->PSSetShaderResources(0, 1, &postShaderResourceView.p);

	// Don't render to the shader anymore.
	//CComPtr<ID3D11DepthStencilView> nulldepthView;
	//DeviceContext->OMSetRenderTargets(1, &RenderTargetViewHolder.p, nulldepthView.p);
	//DeviceContext->ClearDepthStencilView(postdepthView.p, D3D11_CLEAR_DEPTH, 0.5f, 0);
	DeviceContext->PSSetShaderResources(1, 1, &zShaderToResourceView.p);

	// Post effect information
	dataToShader[0] = float(Timer.TotalTimeExact());

	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	ZeroMemory(&mapSubresource, sizeof(mapSubresource));
	DeviceContext->Map(postConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSubresource);
	memcpy(mapSubresource.pData, &dataToShader, sizeof(float) * 2);
	DeviceContext->Unmap(postConstBuffer, NULL);

	// Drawing the star
	DeviceContext->DrawIndexed(6, 0, 0);

	// Don't read from the shader anymore
	CComPtr<ID3D11ShaderResourceView> nullShaderResourceView;
	DeviceContext->PSSetShaderResources(0, 1, &nullShaderResourceView.p);
	DeviceContext->PSSetShaderResources(1, 1, &nullShaderResourceView.p);
	DeviceContext->OMSetRenderTargets(1, &RenderTargetViewHolder.p, depthView.p);

	// Cleaning the depth buffer so it gives the infinite look
	DeviceContext->ClearDepthStencilView(depthView.p, D3D11_CLEAR_DEPTH, 1, 0);


	// Present to the screen
	//Swapchain->Present(0, 0);
}

