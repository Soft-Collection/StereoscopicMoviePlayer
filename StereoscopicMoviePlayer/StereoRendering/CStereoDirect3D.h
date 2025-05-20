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
	} ImageData;
	typedef struct {
		INT Width;
		INT Height;
		INT Channels;
	} ImageDimensions;
private:
	HWND m_HWnd;
	LPDIRECT3DSURFACE9 m_LeftSurface;
	LPDIRECT3DSURFACE9 m_RightSurface;
	LPDIRECT3DSURFACE9 m_BlackSurface;
	LPDIRECT3DSURFACE9 m_SysMemSurface;
	ImageDimensions m_LastImageDimensions;
	std::chrono::high_resolution_clock::time_point m_LastTimeMeasuring;
	std::atomic<int> m_FrequencyInHz;
	std::atomic<int> m_LRBoth;
	std::atomic<BOOL> m_SwapLR;
	std::atomic<BOOL> m_VerticalLR;
	std::atomic<BOOL> m_LastVerticalLR;
private:
	std::mutex* mMutexDrawBlt;
	LPDIRECT3D9 m_D3D;
	LPDIRECT3DDEVICE9 m_Device;
private:
	BOOL ReInit(ImageDimensions imageDimensions);
	BOOL CreateBlackSurface();
	BOOL DrawOnLRSurface(ImageData idat);
public:
	CStereoDirect3D(HWND hWnd);
	~CStereoDirect3D();
	BOOL DrawImageRGB(ImageData idat);
	BOOL DrawImage(ImageData idat){ return(DrawImageRGB(idat)); }
	BOOL Blt(bool isLeft);
	INT GetFrequency();
	void StereoLRBoth(int lrboth);
	void StereoSwapLR(BOOL swaplr);
	void StereoVerticalLR(BOOL verticallr);
};
#endif // __CSTEREODIRECT3D_H__