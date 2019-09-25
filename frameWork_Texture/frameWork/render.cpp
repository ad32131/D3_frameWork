#include "stdafx.h"

C_RENDER::C_RENDER() :
	m_pD3D(nullptr),
	m_pd3dDevice(nullptr),
	m_pVB(nullptr),
	m_pIB(nullptr),
	m_pTexture1(nullptr),
	m_pTexture2(nullptr),
	m_pVBLINE(nullptr)
{
}

HRESULT C_RENDER::initD3D(HWND hWnd)
{
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice)))
	{
		return E_FAIL;
	}

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}


HRESULT C_RENDER::initGeometry()
{
	S_VERTEX arVertices[4] =
	{
		{ D3DXVECTOR3( -1.0f,-1.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f), 0xffff0000,
		0.0f,1.0f ,0.0f,1.0f },
		{ D3DXVECTOR3(-1.0f,1.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f), 0xff0000ff,
		0.0f,0.0f ,0.0f,0.0f },
		{ D3DXVECTOR3(1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f), 0xffffffff,
		1.0f,1.0f ,1.0f,1.0f },
		{ D3DXVECTOR3(1.0f, 1.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f), 0xffffffff, 
		1.0f,0.0f ,1.0f,0.0f },
	};
	S_INDEX arIndex[2] = 
	{
		{0,2,1},
		{1,2,3}
	};

	if (FAILED(m_pd3dDevice->CreateVertexBuffer(4 * sizeof(S_VERTEX),
		0, E_D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, arVertices, sizeof(S_VERTEX) * 4);
	m_pVB->Unlock();

	if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(S_INDEX) * 2, 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &m_pIB, nullptr)))
		return E_FAIL;

	void *pIB = nullptr;
	m_pIB->Lock(0, 0, &pIB, 0);
	memcpy(pIB, arIndex, sizeof(S_INDEX) * 2);
	m_pIB->Unlock();

	// Use D3DX to create a texture from a file based image
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"banana.bmp", &m_pTexture1)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"..\\banana.bmp", &m_pTexture1)))
		{
			MessageBox(NULL, L"Could not find banana.bmp", L"Textures.exe", MB_OK);
			return E_FAIL;
		}
	}

	// Use D3DX to create a texture from a file based image
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"space.bmp", &m_pTexture2)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"..\\space.bmp", &m_pTexture2)))
		{
			MessageBox(NULL, L"Could not find space.bmp", L"Textures.exe", MB_OK);
			return E_FAIL;
		}
	}


	if (FAILED(m_pd3dDevice->CreateVertexBuffer(400 * sizeof(S_LINE),
		0, E_D3DFVF_LINE,
		D3DPOOL_DEFAULT, &m_pVBLINE, NULL)))
		return E_FAIL;

	S_LINE *pLine = nullptr;
	if (FAILED(m_pVBLINE->Lock(0, 0, (void**)&pLine, 0)))
		return E_FAIL;

	float fDistance = 1.0f;
	for (int i = 0; i < 100; i++)
	{
		pLine[i * 4 + 0].vPosition = D3DXVECTOR3(-50.0f + (fDistance * (float)i), 0.0f,-50.0f);
		pLine[i * 4 + 1].vPosition = D3DXVECTOR3(-50.0f + (fDistance * (float)i), 0.0f, 50.0f);
		pLine[i * 4 + 2].vPosition = D3DXVECTOR3(-50.0f , 0.0f, -50.0f + (fDistance * (float)i));
		pLine[i * 4 + 3].vPosition = D3DXVECTOR3(50.0f , 0.0f, -50.0f + (fDistance * (float)i));

		pLine[i * 4 + 0].dwColor = 0xffffffff;
		pLine[i * 4 + 1].dwColor = 0xffffffff;
		pLine[i * 4 + 2].dwColor = 0xffffffff;
		pLine[i * 4 + 3].dwColor = 0xffffffff;
	}

	m_pVBLINE->Unlock();

	return S_OK;
}

VOID C_RENDER::setupLights()
{
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pd3dDevice->SetMaterial(&mtrl);

	// Set up a white, directional light, with an oscillating direction.
	// Note that many lights may be active at a time (but each one slows down
	// the rendering of our scene). However, here we are just using one. Also,
	// we need to set the D3DRS_LIGHTING renderstate to enable lighting
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));

//	vecDir = D3DXVECTOR3(0.5f, 0.5f, -5.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	m_pd3dDevice->SetLight(0, &light);
	m_pd3dDevice->LightEnable(0, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light.
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

VOID C_RENDER::cleanup()
{
	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pTexture1 != NULL)
		m_pTexture1->Release();

	if (m_pTexture2 != NULL)
		m_pTexture2->Release();

	if (m_pVB != NULL)
		m_pVB->Release();

	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();
}

VOID C_RENDER::setupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 5.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID C_RENDER::render()
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		setupLights();
		setupMatrices();

		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(S_VERTEX));
		m_pd3dDevice->SetFVF(E_D3DFVF_CUSTOMVERTEX);
		m_pd3dDevice->SetIndices(m_pIB);

		m_pd3dDevice->SetTexture(0, m_pTexture1);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		
		m_pd3dDevice->SetTexture(1, m_pTexture2);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00ffffff);
		m_pd3dDevice->SetIndices(nullptr);
		m_pd3dDevice->SetTexture(0, nullptr);
		m_pd3dDevice->SetTexture(1, nullptr);
		m_pd3dDevice->SetStreamSource(0, m_pVBLINE, 0, sizeof(S_LINE));
		m_pd3dDevice->SetFVF(E_D3DFVF_LINE);
		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 200);
		
		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
