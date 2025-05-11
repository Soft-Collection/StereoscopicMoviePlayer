#ifndef __CSTEREODIRECT3D_H__
#define __CSTEREODIRECT3D_H__

#include <windows.h>
#include <d3d9.h>
#include <tchar.h>
#include <mutex>
#include <chrono>
#include <atomic>

#pragma comment(lib, "d3d9.lib")

class CStereoDirect3D
{
public:
	typedef struct {
		BYTE* DataPtr;
		INT Width;
		INT Height;
		INT Channels;
		BOOL IsLeft;
		LPDIRECT3DSURFACE9 Surface;
	} ImageData;
private:
	HWND m_HWnd;
	ImageData m_Left;
	ImageData m_Right;
	std::chrono::high_resolution_clock::time_point m_LastTimeMeasuring;
	std::atomic<int> m_FrequencyInHz;
	std::atomic<int> m_LRBoth;
	std::atomic<BOOL> m_SwapLR;
private:
	std::mutex* mMutexDispose;
	std::mutex* mMutexDrawBlt;
	LPDIRECT3D9 m_D3D;
	LPDIRECT3DDEVICE9 m_Device;
private:
	LPDIRECT3DSURFACE9 CreateSurface(ImageData idat);
public:
	CStereoDirect3D(HWND hWnd);
	~CStereoDirect3D();
	BOOL DrawImageRGB(ImageData idat);
	BOOL DrawImage(ImageData idat){ return(DrawImageRGB(idat)); }
	BOOL Blt(bool isLeft);
	INT GetFrequency();
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
};
#endif // __CSTEREODIRECT3D_H__