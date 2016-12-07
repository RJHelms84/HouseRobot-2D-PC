//Direct3D framework source code

#include <d3d9.h>
#include <d3dx9.h>
#include "dxgraphics.h"
#include "dxinput.h"

//variable declarations
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer=NULL;
D3DPRESENT_PARAMETERS d3dpp;

//--------------------------Start 3D-----------------------------------

D3DXVECTOR3 cameraSource;
D3DXVECTOR3 cameraTarget;

void SetPosition(QUAD* quad, int ivert, float x, float y, float z)
{
	quad->vertices[ivert].x = x;
	quad->vertices[ivert].y = y;
	quad->vertices[ivert].z = z;
}

void SetVertex(QUAD* quad, int ivert, float x, float y, float z, float tu, float tv)
{
	SetPosition(quad, ivert, x, y, z);
	quad->vertices[ivert].tu = tu;
	quad->vertices[ivert].tv = tv;
}

VERTEX CreateVertex(float x, float y, float z, float tu, float tv)
{
	VERTEX vertex;
	vertex.x = x;
	vertex.y = y;
	vertex.z = z;
	vertex.tu = tu;
	vertex.tv = tv;
	return vertex;
}

QUAD *CreateQuad(char *textureFilename)
{
	QUAD *quad = (QUAD*)malloc(sizeof(QUAD));

	//load texture
	D3DXCreateTextureFromFile(d3ddev, textureFilename, &quad->texture);

	//create the vertex buffer for this quad
	d3ddev->CreateVertexBuffer(
		4*sizeof(VERTEX),
		0,
		D3DFVF_MYVERTEX, D3DPOOL_DEFAULT,
		&quad->buffer,
		NULL);

	//create the four corners of this dual triangle strip
	//each vertex is x,y,z and the texture coordinates u,v
	quad->vertices[0] = CreateVertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	quad->vertices[1] = CreateVertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	quad->vertices[2] = CreateVertex(-1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	quad->vertices[3] = CreateVertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f);

	return quad;
}

void DeleteQuad(QUAD *quad)
{
	if(quad == NULL)
		return;

	//free the vertex buffer
	if(quad->buffer != NULL)
		quad->buffer->Release();

	//free the texture
	if(quad->texture != NULL)
		quad->texture->Release();

	//free the quad
	free(quad);
}

void DrawQuad(QUAD *quad)
{
	//fill the vertex buffer with this quad's vertices
	void *temp = NULL;
	quad->buffer->Lock(0, sizeof(quad->vertices), (void**)&temp, 0);
	memcpy(temp, quad->vertices, sizeof(quad->vertices));
	quad->buffer->Unlock();

	//draw the textured dual triangle strip
	d3ddev->SetTexture(0, quad->texture);
	d3ddev->SetStreamSource(0, quad->buffer, 0, sizeof(VERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void SetIdentity()
{
	//set default position, scale and rotation
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void ClearScene(D3DXCOLOR color)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

void SetCamera(float x, float y, float z, float lookx, float looky, float lookz)
{
	D3DXMATRIX matView;
	D3DXVECTOR3 updir(0.0f,1.0f,0.0f);

	//move the camera
	cameraSource.x = x;
	cameraSource.y = y;
	cameraSource.z = z;

	//point the camera
	cameraTarget.x = lookx;
	cameraTarget.y = looky;
	cameraTarget.z = lookz;

	//set up the camera view matrix
	D3DXMatrixLookAtLH(&matView, &cameraSource, &cameraTarget, &updir);
	d3ddev->SetTransform(D3DTS_VIEW, &matView);
}

void SetPerspective(float fieldOfView, float aspectRatio, float nearRange, float farRange)
{
	//set the perspective so things in the distance will look smaller
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, fieldOfView, aspectRatio, nearRange, farRange);
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProj);
}

//--------------------------End 3D-------------------------------------

int Init_Direct3D(HWND hwnd, int width, int height,  int fullscreen)
{
	d3d=Direct3DCreate9(D3D_SDK_VERSION);
	if(d3d==NULL)
	{
		MessageBoxA(hwnd,"Error initialising Direct3D","Error",MB_OK);
		return 0;
	}
	//set Direct3D presentation parameters
	//D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = 1;
	d3dpp.SwapEffect =D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount=1;
	d3dpp.BackBufferWidth=width;
	d3dpp.BackBufferHeight=height;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
		);

	if(d3ddev==NULL)
	{
		MessageBoxA(hwnd, "Error creating Direct3D device", "Error", MB_OK);
		return 0;
	}

	//clear the backbuffer to black
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0),1.0f,0);

	//create pointer to the backbuffer
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);

	return 1;
}

LPDIRECT3DSURFACE9 LoadSurface(char *filename, D3DCOLOR transcolor)
{
	LPDIRECT3DSURFACE9 image=NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename,&info);
	if(result!=D3D_OK)
		return NULL;

	//create surface
	result=d3ddev->CreateOffscreenPlainSurface(
		info.Width,		//width of the surface
		info.Height,	//height of the surface
		D3DFMT_X8R8G8B8,//surface format
		D3DPOOL_DEFAULT,//memory pool to use
		&image,			//pointer to the surface
		NULL
		);

	if(result!=D3D_OK)
		return NULL;

	//load surface from file into newly created surface
	result=D3DXLoadSurfaceFromFile(
		image, //destination surface
		NULL, //destination palette
		NULL, //destination rectangle
		filename, //source filename
		NULL, //source rectangle
		D3DX_DEFAULT, //controls how image is filtered
		transcolor, //for transparency (0 for none)
		NULL); //source image info (usually NULL)

	//make sure file was loaded ok
	if(result!=D3D_OK)
		return NULL;

	return image;
}

LPDIRECT3DTEXTURE9 LoadTexture(char* filename, D3DCOLOR transcolor)
{
	//the texture pointer
	LPDIRECT3DTEXTURE9 texture = NULL;

	//the struct for reading bitmap file info
	D3DXIMAGE_INFO info;

	//standard Windows return value
	HRESULT result;

	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFileA(filename,&info);
	if(result!=D3D_OK)
		return NULL;

	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileExA(
		d3ddev,						//Direct3D device object
		filename,					//bitmap filename
		info.Width,					//bitmap image width
		info.Height,				//bitmap image height
		1,							//mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,			//the type of surface (standard)
		D3DFMT_UNKNOWN,				//surface format (default)
		D3DPOOL_DEFAULT,			//memory class for the texture
		D3DX_DEFAULT,				//image filter
		D3DX_DEFAULT,				//mip filter
		transcolor,					//color key for transparfancy
		&info,						//bitmap file info from loaded file
		NULL,						//colour palette
		&texture					//destination texture
		);

	//make sure the bitmap texture was loaded correctly
	if(result!=D3D_OK)
		return NULL;

	return texture;
}

void ResetDevice(HWND hWnd)
{
	if(d3ddev)
	{
		//d3ddev->Reset(&d3dpp);
		void Kill_Keyboard();
		void Kill_Mouse();
		dinput->Release();
		Init_DirectInput(hWnd);
	}
}