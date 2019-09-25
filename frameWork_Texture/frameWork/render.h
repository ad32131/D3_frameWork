#pragma once

#include<d3dx9.h>

class C_RENDER
{
private:
	struct S_VERTEX
	{
		D3DXVECTOR3 vPosition; // The 3D position for the vertex
		D3DXVECTOR3 vNormal;   // The surface normal for the vertex
		DWORD dwColor;        // The vertex color
		float fu1, fv1;
		float fu2, fv2;
	};
	struct S_LINE
	{
		D3DXVECTOR3 vPosition; // The 3D position for the vertex
		DWORD dwColor;        // The vertex color
	};
	struct S_INDEX
	{
		WORD w0;
		WORD w1;
		WORD w2;
	};
	enum
	{
		E_D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX2),
		E_D3DFVF_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE),
	};

private:
	LPDIRECT3D9             m_pD3D ; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice ; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB ; // Buffer to hold vertices
	LPDIRECT3DVERTEXBUFFER9 m_pVBLINE; // Buffer to hold vertices
	LPDIRECT3DINDEXBUFFER9  m_pIB;
	LPDIRECT3DTEXTURE9      m_pTexture1 ; // Our texture
	LPDIRECT3DTEXTURE9      m_pTexture2 ; // Our texture

public:
	C_RENDER();
	HRESULT initD3D(HWND hWnd);
	HRESULT initGeometry();
	VOID cleanup();
	VOID setupMatrices();
	VOID render();
	void setupLights();
};