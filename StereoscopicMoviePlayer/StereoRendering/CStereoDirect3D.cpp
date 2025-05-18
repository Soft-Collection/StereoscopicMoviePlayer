#include "stdafx.h"
#include "CStereoDirect3D.h"

#define BOTH 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

CStereoDirect3D::CStereoDirect3D(HWND hWnd)
{
	m_HWnd = NULL;
	m_LeftSurface = nullptr;
	m_RightSurface = nullptr;
	m_BlackSurface = nullptr;
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	m_LRBoth.store(0);
	m_SwapLR.store(FALSE);
	m_VerticalLR.store(FALSE);
	//--------------------------------------------------------
	mMutexDrawBlt = new std::mutex();
	//--------------------------------------------------------
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	//--------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //Enable vsync
	m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_Device);
	//--------------------------------------------------------
	CreateBlackSurface();
}
CStereoDirect3D::~CStereoDirect3D()
{
	if (m_BlackSurface) m_BlackSurface->Release();
	if (m_LeftSurface) m_LeftSurface->Release();
	if (m_RightSurface) m_RightSurface->Release();
	if (m_Device) m_Device->Release();
	if (m_D3D) m_D3D->Release();
	//--------------------------------------------------------
	if (mMutexDrawBlt != nullptr)
	{
		delete mMutexDrawBlt;
		mMutexDrawBlt = nullptr;
	}
}
BOOL CStereoDirect3D::CreateBlackSurface()
{
	HRESULT hr;
	hr = m_Device->CreateRenderTarget(1, 1, D3DFMT_X8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_BlackSurface, nullptr);
	if (FAILED(hr)) return FALSE;
	IDirect3DSurface9* oldRT = nullptr;
	m_Device->GetRenderTarget(0, &oldRT);
	m_Device->SetRenderTarget(0, m_BlackSurface);
	m_Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_Device->SetRenderTarget(0, oldRT);
	if (oldRT) oldRT->Release();
	return TRUE;
}
BOOL CStereoDirect3D::CreateLRSurface(ImageData idat)
{
	LPDIRECT3DSURFACE9 sysMemSurface = nullptr;
	if (m_VerticalLR.load())
	{
		m_Device->CreateOffscreenPlainSurface(idat.Width, idat.Height / 2, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &sysMemSurface, nullptr);
	}
	else //HorizontalLR
	{
		m_Device->CreateOffscreenPlainSurface(idat.Width / 2, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &sysMemSurface, nullptr);
	}
	D3DLOCKED_RECT rect;
	if (SUCCEEDED(sysMemSurface->LockRect(&rect, nullptr, 0))) {
		BYTE* pixels = (BYTE*)rect.pBits;
		if (idat.Channels == 4)
		{
			if (m_VerticalLR.load())
			{
				if (idat.IsLeft)
				{
					memcpy(pixels, idat.DataPtr, idat.Height * idat.Width * idat.Channels / 2); //Left
				}
				else //IsRight
				{
					memcpy(pixels, idat.DataPtr + (idat.Height * idat.Width * idat.Channels / 2), idat.Height * idat.Width * idat.Channels / 2); //Right
				}
			}
			else //HorizontalLR
			{
				for (int y = 0; y < idat.Height; y++)
				{
					if (idat.IsLeft)
					{
						memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels), idat.Width / 2 * idat.Channels); //Left
					}
					else //IsRight
					{
						memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels + idat.Width / 2 * idat.Channels), idat.Width / 2 * idat.Channels); //Right
					}
				}
			}
		}
		sysMemSurface->UnlockRect();
	}

	if (m_VerticalLR.load())
	{
		m_Device->CreateOffscreenPlainSurface(idat.Width, idat.Height / 2, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, (idat.IsLeft) ? &m_LeftSurface : &m_RightSurface, nullptr);
	}
	else //HorizontalLR
	{
		m_Device->CreateOffscreenPlainSurface(idat.Width / 2, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, (idat.IsLeft) ? &m_LeftSurface : &m_RightSurface, nullptr);
	}
	m_Device->UpdateSurface(sysMemSurface, nullptr, (idat.IsLeft) ? m_LeftSurface : m_RightSurface, nullptr);
	sysMemSurface->Release();
	return TRUE;
}
BOOL CStereoDirect3D::DrawImageRGB(ImageData idat)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (m_LeftSurface) m_LeftSurface->Release();
	if (m_RightSurface) m_RightSurface->Release();
	idat.IsLeft = TRUE;
	CreateLRSurface(idat);
	idat.IsLeft = FALSE;
	CreateLRSurface(idat);
	lock1.unlock();
	return m_LeftSurface && m_RightSurface;
}
BOOL CStereoDirect3D::Blt(bool isLeft)
{
	LPDIRECT3DSURFACE9 backBuffer = nullptr;
	m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer) {
		std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
		LPDIRECT3DSURFACE9 surface = m_BlackSurface;
		if (isLeft)
		{
			if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == LEFT_ONLY))
			{
				if (m_SwapLR.load())
				{
					surface = m_RightSurface;
				}
				else
				{
					surface = m_LeftSurface;
				}
			}
		}
		else //IsRight
		{
			if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == RIGHT_ONLY))
			{
				if (m_SwapLR.load())
				{
					surface = m_LeftSurface;
				}
				else
				{
					surface = m_RightSurface;
				}
			}
		}
		m_Device->BeginScene();
		m_Device->StretchRect(surface, nullptr, backBuffer, nullptr, D3DTEXF_LINEAR);
		m_Device->EndScene();
		m_Device->Present(nullptr, nullptr, nullptr, nullptr); //Blocks until new vsync.
		lock1.unlock();
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
		if (duration.count() > 0)
		{
			m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
		}
		m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
		backBuffer->Release();
	}
	return TRUE;
}
INT CStereoDirect3D::GetFrequency()
{
	return m_FrequencyInHz.load();
}
void CStereoDirect3D::StereoLRBoth(int lrboth)
{
	m_LRBoth.store(lrboth);
}
void CStereoDirect3D::StereoSwapLR(BOOL swaplr)
{
	m_SwapLR.store(swaplr);
}
void CStereoDirect3D::StereoVerticalLR(BOOL verticallr)
{
	m_VerticalLR.store(verticallr);
}