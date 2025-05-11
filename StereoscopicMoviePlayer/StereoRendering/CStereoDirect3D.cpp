#include "stdafx.h"
#include "CStereoDirect3D.h"

#define BOTH 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

CStereoDirect3D::CStereoDirect3D(HWND hWnd)
{
	m_HWnd = NULL;
	m_Left = { 0, };
	m_Right = { 0, };
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	m_LRBoth.store(0);
	m_SwapLR.store(FALSE);
	//--------------------------------------------------------
	mMutexDispose = new std::mutex();
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
}
CStereoDirect3D::~CStereoDirect3D()
{
	std::unique_lock<std::mutex> lock1(*mMutexDispose); // Lock the mutex
	if (m_Left.Surface) m_Left.Surface->Release();
	if (m_Right.Surface) m_Right.Surface->Release();
	if (m_Device) m_Device->Release();
	if (m_D3D) m_D3D->Release();
	lock1.unlock();
	//--------------------------------------------------------
	if (mMutexDrawBlt != nullptr)
	{
		delete mMutexDrawBlt;
		mMutexDrawBlt = nullptr;
	}
	if (mMutexDispose != nullptr)
	{
		delete mMutexDispose;
		mMutexDispose = nullptr;
	}
}
LPDIRECT3DSURFACE9 CStereoDirect3D::CreateSurface(ImageData idat)
{
	LPDIRECT3DSURFACE9 sysMemSurface = nullptr;
	LPDIRECT3DSURFACE9 videoSurface = nullptr;
	m_Device->CreateOffscreenPlainSurface(idat.Width / 2, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &sysMemSurface, nullptr);
	D3DLOCKED_RECT rect;
	if (SUCCEEDED(sysMemSurface->LockRect(&rect, nullptr, 0))) {
		BYTE* pixels = (BYTE*)rect.pBits;
		if (idat.Channels == 4)
		{
			for (int y = 0; y < idat.Height; y++)
			{
				if (idat.IsLeft)
				{
					if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == LEFT_ONLY))
					{
						if (m_SwapLR.load())
						{
							memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels + idat.Width / 2 * idat.Channels), idat.Width / 2 * idat.Channels); //Right
						}
						else
						{
							memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels), idat.Width / 2 * idat.Channels); //Left
						}
					}
				}
				else //IsRight
				{
					if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == RIGHT_ONLY))
					{
						if (m_SwapLR.load())
						{
							memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels), idat.Width / 2 * idat.Channels); //Left
						}
						else
						{
							memcpy(pixels + (y * idat.Width / 2 * idat.Channels), idat.DataPtr + (y * idat.Width * idat.Channels + idat.Width / 2 * idat.Channels), idat.Width / 2 * idat.Channels); //Right
						}
					}
				}
			}
		}
		sysMemSurface->UnlockRect();
	}
	m_Device->CreateOffscreenPlainSurface(idat.Width / 2, idat.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &videoSurface, nullptr);
	m_Device->UpdateSurface(sysMemSurface, nullptr, videoSurface, nullptr);
	sysMemSurface->Release();
	return videoSurface;
}
BOOL CStereoDirect3D::DrawImageRGB(ImageData idat)
{
	std::unique_lock<std::mutex> lock1(*mMutexDispose); // Lock the mutex
	if (m_Left.Surface) m_Left.Surface->Release();
	if (m_Right.Surface) m_Right.Surface->Release();
	idat.IsLeft = TRUE;
	m_Left.Surface = CreateSurface(idat);
	idat.IsLeft = FALSE;
	m_Right.Surface = CreateSurface(idat);
	lock1.unlock();
	return m_Left.Surface && m_Right.Surface;
}
BOOL CStereoDirect3D::Blt(bool isLeft)
{
	std::unique_lock<std::mutex> lock1(*mMutexDispose); // Lock the mutex
	LPDIRECT3DSURFACE9 backBuffer = nullptr;
	m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer) {
		m_Device->BeginScene();
		LPDIRECT3DSURFACE9 srcSurface = isLeft ? m_Left.Surface : m_Right.Surface;
		m_Device->StretchRect(srcSurface, nullptr, backBuffer, nullptr, D3DTEXF_LINEAR);
		m_Device->EndScene();
		m_Device->Present(nullptr, nullptr, nullptr, nullptr); //Blocks until new vsync.
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
		if (duration.count() > 0)
		{
			m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
		}
		m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
		backBuffer->Release();
	}
	lock1.unlock();
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