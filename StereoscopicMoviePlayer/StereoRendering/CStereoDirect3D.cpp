#include "stdafx.h"
#include "CStereoDirect3D.h"

#define BOTH 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

CStereoDirect3D::CStereoDirect3D(HWND hWnd)
{
	m_LeftSurface = nullptr;
	m_RightSurface = nullptr;
	m_BlackSurface = nullptr;
	m_SysMemSurface = nullptr;
	m_LastImageDimensions = { 0, 0, 0 };
	m_Frame = nullptr;
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	m_LRBoth.store(0);
	m_SwapLR.store(FALSE);
	m_VerticalLR.store(FALSE);
	m_LastVerticalLR.store(FALSE);
	m_ImageDataUpdated.store(FALSE);
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
	m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_Device);
	//--------------------------------------------------------
	CreateBlackSurface();
}
CStereoDirect3D::~CStereoDirect3D()
{
	if (m_BlackSurface) { m_BlackSurface->Release(); m_BlackSurface = nullptr; }
	if (m_LeftSurface) { m_LeftSurface->Release(); m_LeftSurface = nullptr; }
	if (m_RightSurface) { m_RightSurface->Release(); m_RightSurface = nullptr; }
	if (m_SysMemSurface) { m_SysMemSurface->Release(); m_SysMemSurface = nullptr; }
	if (m_Device) { m_Device->Release(); m_Device = nullptr; }
	if (m_D3D) { m_D3D->Release(); m_D3D = nullptr; }
	//--------------------------------------------------------
	if (mMutexDrawBlt != nullptr)
	{
		delete mMutexDrawBlt;
		mMutexDrawBlt = nullptr;
	}
}
BOOL CStereoDirect3D::ReInit(ImageDimensions imageDimensions)
{
	if ((m_LastImageDimensions.Width != imageDimensions.Width) || 
		(m_LastImageDimensions.Height != imageDimensions.Height) || 
		(m_LastImageDimensions.Channels != imageDimensions.Channels) || 
		(m_LastVerticalLR.load() != m_VerticalLR.load()))
	{
		if (m_LeftSurface) { m_LeftSurface->Release(); m_LeftSurface = nullptr; }
		if (m_RightSurface) { m_RightSurface->Release(); m_RightSurface = nullptr; }
		if (m_SysMemSurface) { m_SysMemSurface->Release(); m_SysMemSurface = nullptr; }
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		if (m_VerticalLR.load())
		{
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width, imageDimensions.Height / 2, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_SysMemSurface, nullptr);
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width, imageDimensions.Height / 2, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_LeftSurface, nullptr);
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width, imageDimensions.Height / 2, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_RightSurface, nullptr);
		}
		else //HorizontalLR
		{
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width / 2, imageDimensions.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_SysMemSurface, nullptr);
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width / 2, imageDimensions.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_LeftSurface, nullptr);
			m_Device->CreateOffscreenPlainSurface(imageDimensions.Width / 2, imageDimensions.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_RightSurface, nullptr);
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_ImageDataUpdated.store(FALSE);
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_LastImageDimensions = { imageDimensions.Width, imageDimensions.Height, imageDimensions.Channels };
		m_LastVerticalLR.store(m_VerticalLR.load());
	}
	return TRUE;
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
BOOL CStereoDirect3D::DrawOnLRSurface(AVFrame* frame, BOOL isLeft)
{
	D3DLOCKED_RECT rect;
	if (SUCCEEDED(m_SysMemSurface->LockRect(&rect, nullptr, 0))) {
		BYTE* pixels = (BYTE*)rect.pBits;
		int channels = frame->linesize[0] / frame->width;
		if (channels == 4)
		{
			if (m_VerticalLR.load())
			{
				if (isLeft)
				{
					memcpy(pixels, frame->data[0], frame->height * frame->width * channels / 2); //Left
				}
				else //IsRight
				{
					memcpy(pixels, frame->data[0] + (frame->height * frame->width * channels / 2), frame->height * frame->width * channels / 2); //Right
				}
			}
			else //HorizontalLR
			{
				for (int y = 0; y < frame->height; y++)
				{
					if (isLeft)
					{
						memcpy(pixels + (y * frame->width / 2 * channels), frame->data[0] + (y * frame->width * channels), frame->width / 2 * channels); //Left
					}
					else //IsRight
					{
						memcpy(pixels + (y * frame->width / 2 * channels), frame->data[0] + (y * frame->width * channels + frame->width / 2 * channels), frame->width / 2 * channels); //Right
					}
				}
			}
		}
		m_SysMemSurface->UnlockRect();
	}
	m_Device->UpdateSurface(m_SysMemSurface, nullptr, isLeft ? m_LeftSurface : m_RightSurface, nullptr);
	return TRUE;
}
BOOL CStereoDirect3D::DrawImageRGB(AVFrame* frame)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (!m_ImageDataUpdated.load())
	{
		m_Frame = frame;
		m_ImageDataUpdated.store(TRUE);
	}
	else
	{
		av_frame_free(&m_Frame);
	}
	lock1.unlock();
	return TRUE;
}
BOOL CStereoDirect3D::Blt(bool isLeft)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (m_ImageDataUpdated.load())
	{
		if (m_Frame)
		{
			ReInit({ m_Frame->width, m_Frame->height, m_Frame->linesize[0] / m_Frame->width });
			DrawOnLRSurface(m_Frame, TRUE);
			DrawOnLRSurface(m_Frame, FALSE);
			av_frame_free(&m_Frame);
		}
		m_ImageDataUpdated.store(FALSE);
	}
	lock1.unlock();
	LPDIRECT3DSURFACE9 backBuffer = nullptr;
	m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer) {
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
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
		if (duration.count() > 0)
		{
			m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
		}
		m_LastTimeMeasuring = now;
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