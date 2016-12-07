#ifndef _DXGRAPHICS_H
#define _DXGRAPHICS_H

//--------------------------Start 3D-----------------------------------
#define D3DFVF_MYVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct VERTEX
{
	float x, y, z;
	float tu, tv;
};

struct QUAD
{
	VERTEX vertices[4];
	LPDIRECT3DVERTEXBUFFER9 buffer;
	LPDIRECT3DTEXTURE9 texture;
};

extern D3DXVECTOR3 cameraSource;
extern D3DXVECTOR3 cameraTarget;

//function prototypes
void SetPosition(QUAD*,int,float,float,float);
void SetVertex(QUAD*,int,float,float,float,float,float);
VERTEX CreateVertex(float,float,float,float,float);
QUAD* CreateQuad(char*);
void DeleteQuad(QUAD*);
void DrawQuad(QUAD*);
void SetIdentity();
void SetCamera(float,float,float,float,float,float);
void SetPerspective(float,float,float,float);
void ClearScene(D3DXCOLOR);

//--------------------------End 3D-------------------------------------

//function prototypes
int Init_Direct3D(HWND, int, int, int);
LPDIRECT3DSURFACE9 LoadSurface(char*,D3DCOLOR);
LPDIRECT3DTEXTURE9 LoadTexture(char*,D3DCOLOR);
void ResetDevice(HWND);

//variable declarations
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern D3DPRESENT_PARAMETERS d3dpp;

#endif

