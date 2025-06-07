#include "stdafx.h"
#include "CStereoDirect3D.h"

#define BOTH 0
#define LEFT_ONLY 1
#define RIGHT_ONLY 2

CStereoDirect3D::CStereoDirect3D(HWND hWnd)
{
	m_HWnd = hWnd;
	m_LRTexture = nullptr;
	m_BlackTexture = nullptr;
	m_SourceTexture = nullptr;
	m_SourceRect = { 0, 0, 0, 0 };
	m_LastImageDimensions = { 0, 0, 0 };
	m_Frame = nullptr;
	m_LastTimeMeasuring = std::chrono::high_resolution_clock::now();
	m_FrequencyInHz.store(0);
	m_LRBoth.store(0);
	m_SwapLR.store(FALSE);
	m_VerticalLR.store(FALSE);
	m_ImageDataUpdated.store(FALSE);
	m_WindowSizeChanged.store(FALSE);
	//--------------------------------------------------------
	mMutexDrawBlt = new std::mutex();
	//--------------------------------------------------------
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	//--------------------------------------------------------
	CreateDevice();
}
CStereoDirect3D::~CStereoDirect3D()
{
	ReleaseDevice();
	if (m_D3D) { m_D3D->Release(); m_D3D = nullptr; }
	//--------------------------------------------------------
	if (mMutexDrawBlt != nullptr)
	{
		delete mMutexDrawBlt;
		mMutexDrawBlt = nullptr;
	}
}
void CStereoDirect3D::CreateDevice()
{
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //Enable vsync
	m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_HWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_Device);
}
void CStereoDirect3D::ReleaseDevice()
{
	m_Device->SetTexture(0, nullptr);
	m_Device->SetTexture(1, nullptr);
	m_Device->SetRenderTarget(0, nullptr);
	m_Device->SetStreamSource(0, nullptr, 0, 0);
	ReleaseTextures();
	if (m_Device) { m_Device->Release(); m_Device = nullptr; }
}
void CStereoDirect3D::ReleaseTextures()
{ 
	if (m_LRTexture) { m_LRTexture->Release(); m_LRTexture = nullptr; }
	if (m_BlackTexture) { m_BlackTexture->Release(); m_BlackTexture = nullptr; }
}
void CStereoDirect3D::ReInitTextures(ImageDimensions imageDimensions)
{
	if ((m_LastImageDimensions.Width != imageDimensions.Width) || 
		(m_LastImageDimensions.Height != imageDimensions.Height) || 
		(m_LastImageDimensions.Channels != imageDimensions.Channels))
	{
		ReleaseTextures();
		CreateBlackTexture();
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_Device->CreateTexture(imageDimensions.Width, imageDimensions.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_LRTexture, nullptr);
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_ImageDataUpdated.store(FALSE);
		//--------------------------------------------------------------------------------------------------------------------------------------------------------
		m_LastImageDimensions = { imageDimensions.Width, imageDimensions.Height, imageDimensions.Channels };
	}
}
void CStereoDirect3D::ResetDevice()
{
	ReleaseDevice();
	CreateDevice();
	m_LastImageDimensions = { 0, 0, 0 };
}
void CStereoDirect3D::CreateBlackTexture()
{
	// Create 1x1 black texture
	m_Device->CreateTexture(1, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_BlackTexture, nullptr);
	// Fill the texture with black (ARGB: 0xFF000000)
	D3DLOCKED_RECT rect;
	if (SUCCEEDED(m_BlackTexture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD)))
	{
		*(DWORD*)rect.pBits = 0xFF000000;
		m_BlackTexture->UnlockRect(0);
	}
}
void CStereoDirect3D::SelectTextureAndRect(BOOL isLeft)
{
	RECT sourceRectBlack = { 0, 0, 1, 1 };
	RECT sourceRectLeftHorizontal = { 0, 0, m_LastImageDimensions.Width / 2, m_LastImageDimensions.Height };
	RECT sourceRectRightHorizontal = { m_LastImageDimensions.Width / 2, 0, m_LastImageDimensions.Width, m_LastImageDimensions.Height };
	RECT sourceRectLeftVertical = { 0, 0, m_LastImageDimensions.Width, m_LastImageDimensions.Height / 2 };
	RECT sourceRectRightVertical = { 0, m_LastImageDimensions.Height / 2, m_LastImageDimensions.Width, m_LastImageDimensions.Height };
	if (isLeft)
	{
		if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == LEFT_ONLY))
		{
			if (m_VerticalLR)
			{
				if (m_SwapLR.load())
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectRightVertical;
				}
				else
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectLeftVertical;
				}
			}
			else //HorizontalLR
			{
				if (m_SwapLR.load())
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectRightHorizontal;
				}
				else
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectLeftHorizontal;
				}
			}
		}
		else //m_LRBoth.load() == RIGHT_ONLY
		{
			m_SourceTexture = m_BlackTexture;
			m_SourceRect = sourceRectBlack;
		}
	}
	else //IsRight
	{
		if ((m_LRBoth.load() == BOTH) || (m_LRBoth.load() == RIGHT_ONLY))
		{
			if (m_VerticalLR)
			{
				if (m_SwapLR.load())
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectLeftVertical;
				}
				else
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectRightVertical;
				}
			}
			else //HorizontalLR
			{
				if (m_SwapLR.load())
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectLeftHorizontal;
				}
				else
				{
					m_SourceTexture = m_LRTexture;
					m_SourceRect = sourceRectRightHorizontal;
				}
			}
		}
		else //m_LRBoth.load() == LEFT_ONLY
		{
			m_SourceTexture = m_BlackTexture;
			m_SourceRect = sourceRectBlack;
		}
	}
}
void CStereoDirect3D::DrawOnLRTexture(AVFrame* frame)
{
	D3DLOCKED_RECT rect;
	m_LRTexture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
	memcpy(rect.pBits, frame->data[0], frame->height * frame->linesize[0]);
	m_LRTexture->UnlockRect(0);
}
void CStereoDirect3D::DrawImageRGB(AVFrame* frame)
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
}
void CStereoDirect3D::Blt(BOOL isLeft)
{
	std::unique_lock<std::mutex> lock1(*mMutexDrawBlt); // Lock the mutex
	if (m_ImageDataUpdated.load())
	{
		if (m_Frame)
		{
			ReInitTextures({ m_Frame->width, m_Frame->height, m_Frame->linesize[0] / m_Frame->width });
			DrawOnLRTexture(m_Frame);
			av_frame_free(&m_Frame);
		}
		m_ImageDataUpdated.store(FALSE);
	}
	lock1.unlock();
	LPDIRECT3DSURFACE9 backBuffer = nullptr;
	m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
	if (backBuffer) 
	{
		SelectTextureAndRect(isLeft);
		m_Device->BeginScene();
		IDirect3DSurface9* sourceSurface = nullptr;
		if (m_SourceTexture) m_SourceTexture->GetSurfaceLevel(0, &sourceSurface);
		m_Device->StretchRect(sourceSurface, &m_SourceRect, backBuffer, nullptr, D3DTEXF_LINEAR);
		m_Device->EndScene();
		HRESULT hr = m_Device->Present(nullptr, nullptr, nullptr, nullptr); //Blocks until new vsync.
		backBuffer->Release();
		if (hr == D3DERR_DEVICELOST)
		{
			while (m_Device->TestCooperativeLevel() == D3DERR_DEVICELOST) std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (m_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
		}
		if (m_WindowSizeChanged.load())
		{
			ResetDevice();
			m_WindowSizeChanged.store(FALSE);
		}
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_LastTimeMeasuring);
		if (duration.count() > 0)
		{
			m_FrequencyInHz.store((INT)(1000000.0 / (double)duration.count()));
		}
		m_LastTimeMeasuring = now;
	}
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
void CStereoDirect3D::StereoWindowSizeChanged()
{
	m_WindowSizeChanged.store(TRUE);
}