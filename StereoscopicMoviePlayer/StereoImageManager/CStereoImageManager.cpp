#include "stdafx.h"
#include "CStereoImageManager.h"
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoImageManager::CStereoImageManager(HWND hWnd)
{
	mHWnd = hWnd;
	mStereoDirect3D = new CStereoDirect3D(hWnd);
	mComPortName = std::wstring(L"");
	mComPort = NULL;
	//----------------------------------------------------
	mLeftImage = NULL;
	mRightImage = NULL;
	//----------------------------------------------------
	mMutexWave = new std::mutex();
	mMutexPlayer = new std::mutex();
	mMutexRender = new std::mutex();
	mThreadRenderRunning.store(false);
	mThreadRender = nullptr;
	mMutexCOMPort = new std::mutex();
	mThreadCOMPortRunning.store(false);
	mThreadCOMPort = nullptr;
	mCOMPortEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//----------------------------------------------------
	mImageToPlayIsLeft = true;
	//----------------------------------------------------
	mPlayer = new CFFMpegPlayer(this, OnNewVideoFrameStatic, OnNewAudioFrameStatic);
	mWave = NULL;
}
CStereoImageManager::~CStereoImageManager()
{
	StereoStop();
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock3(*mMutexWave); // Lock the mutex
	if (mWave != NULL)
	{
		delete mWave;
		mWave = NULL;
	}
	lock3.unlock();
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock4(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		mPlayer->Close();
		delete mPlayer;
		mPlayer = NULL;
	}
	lock4.unlock();
	//----------------------------------------------------
	if (mRightImage != NULL)
	{
		delete mRightImage;
		mRightImage = NULL;
	}
	//----------------------------------------------------
	if (mLeftImage != NULL)
	{
		delete mLeftImage;
		mLeftImage = NULL;
	}
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		delete mComPort;
		mComPort = NULL;
	}
	lock1.unlock();
	//----------------------------------------------------
	std::unique_lock<std::mutex> lock2(*mMutexRender); // Lock the mutex
	if (mStereoDirect3D != NULL)
	{
		delete mStereoDirect3D;
		mStereoDirect3D = NULL;
	}
	lock2.unlock();
	//----------------------------------------------------
	if (mMutexCOMPort != nullptr)
	{
		delete mMutexCOMPort;
		mMutexCOMPort = nullptr;
	}
	//----------------------------------------------------
	if (mMutexRender != nullptr)
	{
		delete mMutexRender;
		mMutexRender = nullptr;
	}
	//----------------------------------------------------
	if (mMutexPlayer != nullptr)
	{
		delete mMutexPlayer;
		mMutexPlayer = nullptr;
	}
	//----------------------------------------------------
	if (mMutexWave != nullptr)
	{
		delete mMutexWave;
		mMutexWave = nullptr;
	}
}
void CStereoImageManager::StereoStart()
{
	if (!mThreadRenderRunning.load())
	{
		mThreadRenderRunning = true;
		mThreadRender = new std::thread(&CStereoImageManager::ThreadRenderFunction, this);
	}
	if (!mThreadCOMPortRunning.load())
	{
		mThreadCOMPortRunning = true;
		mThreadCOMPort = new std::thread(&CStereoImageManager::ThreadCOMPortFunction, this);
	}
}
void CStereoImageManager::StereoStop()
{
	SetEvent(mCOMPortEvent);
	if (mThreadCOMPortRunning.load())
	{
		mThreadCOMPortRunning.store(false);
		if (mThreadCOMPort && mThreadCOMPort->joinable())
		{
			mThreadCOMPort->join();
		}
		delete mThreadCOMPort;
		mThreadCOMPort = nullptr;
	}
	if (mThreadRenderRunning.load())
	{
		mThreadRenderRunning.store(false);
		if (mThreadRender && mThreadRender->joinable())
		{
			mThreadRender->join();
		}
		delete mThreadRender;
		mThreadRender = nullptr;
	}
}
BOOL CStereoImageManager::StereoIsStarted()
{
	return mThreadRenderRunning.load();
}
int CStereoImageManager::StereoGetFrequency()
{
	if (mStereoDirect3D != NULL)
	{
		return mStereoDirect3D->GetFrequency();
	}
	return 0;
}
void CStereoImageManager::StereoSetCOMPort(LPCWSTR comPort)
{
	if (mComPortName != std::wstring(comPort))
	{
		if (mComPort != NULL)
		{
			delete mComPort;
			mComPort = NULL;
		}
		mComPort = new CComPort(std::wstring(comPort));
		mComPortName = std::wstring(comPort);
	}
}
void CStereoImageManager::StereoSetGlassesTimeOffset(int offset)
{
	if (mComPort != NULL)
	{
		mComPort->SendGlassesTimeOffset(offset);
	}
}
void CStereoImageManager::StereoSetTransparentTimePercent(int percent)
{
	if (mComPort != NULL)
	{
		mComPort->SendTransparentTimePercent(percent);
	}
}
void CStereoImageManager::StereoLRBoth(int lrboth)
{
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->StereoLRBoth(lrboth);
	}
}
void CStereoImageManager::StereoSwapLR(BOOL swaplr)
{
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->StereoSwapLR(swaplr);
	}
}
void CStereoImageManager::PlayerOpen(LPCWSTR fileName)
{
	if (mPlayer != NULL)
	{
		mPlayer->Open(std::wstring(fileName));
	}
}
void CStereoImageManager::PlayerClose()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Close();
		}
	}
}
void CStereoImageManager::PlayerReopen()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Reopen();
		}
	}
}
BOOL CStereoImageManager::PlayerIsOpened()
{
	if (mPlayer != NULL)
	{
		return mPlayer->IsOpened();
	}
	return false;
}
void CStereoImageManager::PlayerPlay()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Play();
		}
	}
}
void CStereoImageManager::PlayerPause()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Pause();
		}
	}
}
void CStereoImageManager::PlayerStop()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Stop();
		}
	}
}
BOOL CStereoImageManager::PlayerIsPlaying()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->IsPlaying();
		}
	}
	return false;
}
INT64 CStereoImageManager::PlayerGetDuration()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetDuration();
		}
	}
	return 0;
}
INT64 CStereoImageManager::PlayerGetCurrentPlayingTime()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetCurrentPlayingTime();
		}
	}
	return 0;
}
void CStereoImageManager::PlayerSeek(INT64 seek_target_seconds)
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Seek(seek_target_seconds);
		}
	}
}
int CStereoImageManager::PlayerGetNumberOfAudioTracks()
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetNumberOfAudioTracks();
		}
	}
	return 0;
}
void CStereoImageManager::PlayerSetAudioTrack(int audio_track_index)
{
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->SetAudioTrack(audio_track_index);
		}
	}
}
void CStereoImageManager::ThreadRenderFunction()
{
	while (mThreadRenderRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexRender); // Lock the mutex
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mStereoDirect3D != NULL)
		{
			mStereoDirect3D->Blt(mImageToPlayIsLeft);
		}
		if (mImageToPlayIsLeft)
		{
			SetEvent(mCOMPortEvent);
		}
		lock1.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
void CStereoImageManager::ThreadCOMPortFunction()
{
	while (mThreadCOMPortRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
		WaitForSingleObject(mCOMPortEvent, INFINITE);
		if (mComPort != NULL)
		{
			mComPort->SendSync();
		}
		lock1.unlock();
	}
}
void CStereoImageManager::OnNewVideoFrameStatic(void* user, BYTE* frameData, int width, int height, int channels, INT64 pts)
{
	if (user == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)user;
	cStereoImageManager->OnNewVideoFrame(frameData, width, height, channels, pts);
}
void CStereoImageManager::OnNewAudioFrameStatic(void* user, BYTE* frameData, int nb_samples, int samplesPerSec, int bitsPerSample, int Channels, INT64 pts)
{
	if (user == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)user;
	cStereoImageManager->OnNewAudioFrame(frameData, nb_samples, samplesPerSec, bitsPerSample, Channels, pts);
}
void CStereoImageManager::OnNewVideoFrame(BYTE* frameData, int width, int height, int channels, INT64 pts)
{
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->DrawImage({ frameData, width, height, channels });
	}
}
void CStereoImageManager::OnNewAudioFrame(BYTE* frameData, int nb_samples, int samplesPerSec, int bitsPerSample, int Channels, INT64 pts)
{
	std::unique_lock<std::mutex> lock1(*mMutexWave); // Lock the mutex
	if (mWave == NULL)
	{
		mWave = new CWavePlaying(10, 100000, samplesPerSec, bitsPerSample, Channels);
		mWave->Open();
	}
	if (mWave != NULL)
	{
		mWave->Play((char*)frameData, 0, nb_samples * (bitsPerSample / 8));
	}
	lock1.unlock();
}
void CStereoImageManager::PlayerMute(BOOL mute)
{
	std::unique_lock<std::mutex> lock1(*mMutexWave); // Lock the mutex
	if (mWave != NULL)
	{
		if (mute)
		{
			mWave->Mute();
		}
		else
		{
			mWave->Unmute();
		}
	}
	lock1.unlock();
}
UINT16 CStereoImageManager::PlayerGetVolume()
{
	std::unique_lock<std::mutex> lock1(*mMutexWave); // Lock the mutex
	if (mWave != NULL)
	{
		WORD leftVolume;
		WORD rightVolume;
		mWave->GetVolume(&leftVolume, &rightVolume);
		return leftVolume;
	}
	lock1.unlock();
	return 0;
}
void CStereoImageManager::PlayerSetVolume(UINT16 volume)
{
	std::unique_lock<std::mutex> lock1(*mMutexWave); // Lock the mutex
	if (mWave != NULL)
	{
		mWave->SetVolume(volume, volume);
	}
	lock1.unlock();
}