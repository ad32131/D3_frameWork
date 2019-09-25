#pragma once
#include <d3d9.h>

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


class Direct
{
private:
	//-----------------------------------------------------------------------------
	// Global variables
	//-----------------------------------------------------------------------------
	LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z, rhw; // The transformed position for the vertex
		DWORD color;        // The vertex color
	};
public:
	

	Direct();
	~Direct();
	HRESULT InitD3D(HWND hWnd);
	VOID Cleanup();
	VOID Render();
	HRESULT InitVB();
};

