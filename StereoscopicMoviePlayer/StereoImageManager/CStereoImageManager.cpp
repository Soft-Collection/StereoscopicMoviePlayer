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
	mMutexWave1 = new std::mutex();
	mMutexWave2 = new std::mutex();
	mMutexPlayer = new std::mutex();
	mMutexRender1 = new std::mutex();
	mMutexRender2 = new std::mutex();
	mThreadRenderRunning.store(false);
	mThreadRender = nullptr;
	mMutexCOMPort = new std::mutex();
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
	std::unique_lock<std::mutex> lock3(*mMutexWave1); // Lock the mutex
	std::unique_lock<std::mutex> lock6(*mMutexWave2); // Lock the mutex
	if (mWave != NULL)
	{
		delete mWave;
		mWave = NULL;
	}
	lock6.unlock();
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
	std::unique_lock<std::mutex> lock2(*mMutexRender1); // Lock the mutex
	std::unique_lock<std::mutex> lock5(*mMutexRender2); // Lock the mutex
	if (mStereoDirect3D != NULL)
	{
		delete mStereoDirect3D;
		mStereoDirect3D = NULL;
	}
	lock5.unlock();
	lock2.unlock();
	//----------------------------------------------------
	if (mMutexCOMPort != nullptr)
	{
		delete mMutexCOMPort;
		mMutexCOMPort = nullptr;
	}
	//----------------------------------------------------
	if (mMutexRender1 != nullptr)
	{
		delete mMutexRender1;
		mMutexRender1 = nullptr;
	}
	//----------------------------------------------------
	if (mMutexPlayer != nullptr)
	{
		delete mMutexPlayer;
		mMutexPlayer = nullptr;
	}
	//----------------------------------------------------
	if (mMutexWave1 != nullptr)
	{
		delete mMutexWave1;
		mMutexWave1 = nullptr;
	}
}
void CStereoImageManager::StereoStart()
{
	if (!mThreadRenderRunning.load())
	{
		mThreadRenderRunning = true;
		mThreadRender = new std::thread(&CStereoImageManager::ThreadRenderFunction, this);
	}
}
void CStereoImageManager::StereoStop()
{
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
void CStereoImageManager::StereoVerticalLR(BOOL verticallr)
{
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->StereoVerticalLR(verticallr);
	}
}
void CStereoImageManager::StereoWindowSizeChanged()
{
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->StereoWindowSizeChanged();
	}
}
void CStereoImageManager::PlayerOpen(LPCWSTR fileName)
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		mPlayer->Open(std::wstring(fileName));
	}
	lock1.unlock();
}
void CStereoImageManager::PlayerClose()
{
	std::unique_lock<std::mutex> lock2(*mMutexRender2); // Lock the mutex
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->DrawImage(NULL);
	}
	lock2.unlock();
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Close();
		}
	}
	lock1.unlock();
}
BOOL CStereoImageManager::PlayerIsOpened()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		return mPlayer->IsOpened();
	}
	lock1.unlock();
	return false;
}
void CStereoImageManager::PlayerPlay()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Play();
		}
	}
	lock1.unlock();
}
void CStereoImageManager::PlayerPause()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Pause();
		}
	}
	lock1.unlock();
}
void CStereoImageManager::PlayerStop()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Stop();
		}
	}
	lock1.unlock();
}
BOOL CStereoImageManager::PlayerIsPlaying()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->IsPlaying();
		}
	}
	lock1.unlock();
	return false;
}
BOOL CStereoImageManager::PlayerIsEOF()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->IsEOF();
		}
	}
	lock1.unlock();
	return false;
}
INT64 CStereoImageManager::PlayerGetDuration()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetDuration();
		}
	}
	lock1.unlock();
	return 0;
}
INT64 CStereoImageManager::PlayerGetCurrentPlayingTime()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetCurrentPlayingTime();
		}
	}
	lock1.unlock();
	return 0;
}
void CStereoImageManager::PlayerSeek(INT64 seek_target_ms)
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->Seek(seek_target_ms);
		}
	}
	lock1.unlock();
}
int CStereoImageManager::PlayerGetNumberOfAudioTracks()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			return mPlayer->GetNumberOfAudioTracks();
		}
	}
	lock1.unlock();
	return 0;
}
void CStereoImageManager::PlayerSetAudioTrack(int audio_track_index)
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		if (mPlayer->IsOpened())
		{
			mPlayer->SetAudioTrack(audio_track_index);
		}
	}
	lock1.unlock();
}
void CStereoImageManager::ThreadRenderFunction()
{
	while (mThreadRenderRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexRender1); // Lock the mutex
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mStereoDirect3D != NULL)
		{
			mStereoDirect3D->Blt(mImageToPlayIsLeft);
		}
		lock1.unlock();
		std::unique_lock<std::mutex> lock2(*mMutexCOMPort); // Lock the mutex
		if (mImageToPlayIsLeft)
		{
			if (mComPort != NULL)
			{
				mComPort->SendSync();
			}
		}
		lock2.unlock();
	}
}
void CStereoImageManager::OnNewVideoFrameStatic(void* user, AVFrame* frame)
{
	if (user == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)user;
	cStereoImageManager->OnNewVideoFrame(frame);
}
void CStereoImageManager::OnNewVideoFrame(AVFrame* frame)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect3D != NULL)
	{
		mStereoDirect3D->DrawImage(frame);
	}
	lock1.unlock();
}
void CStereoImageManager::OnNewAudioFrameStatic(void* user, AVFrame* frame)
{
	if (user == NULL) return;
	CStereoImageManager* cStereoImageManager = (CStereoImageManager*)user;
	cStereoImageManager->OnNewAudioFrame(frame);
}
void CStereoImageManager::OnNewAudioFrame(AVFrame* frame)
{
	int bitsPerSample = 0;
	switch (frame->format)
	{
	case AV_SAMPLE_FMT_U8:
		bitsPerSample = 8;
		break;
	case AV_SAMPLE_FMT_S16:
		bitsPerSample = 16;
		break;
	case AV_SAMPLE_FMT_S32:
		bitsPerSample = 32;
		break;
	case AV_SAMPLE_FMT_FLT:
		bitsPerSample = 32;
		break;
	case AV_SAMPLE_FMT_DBL:
		bitsPerSample = 64;
		break;
	case AV_SAMPLE_FMT_U8P:
		bitsPerSample = 8;
		break;
	case AV_SAMPLE_FMT_S16P:
		bitsPerSample = 16;
		break;
	case AV_SAMPLE_FMT_S32P:
		bitsPerSample = 32;
		break;
	case AV_SAMPLE_FMT_FLTP:
		bitsPerSample = 32;
		break;
	case AV_SAMPLE_FMT_DBLP:
		bitsPerSample = 64;
		break;
	}
	std::unique_lock<std::mutex> lock1(*mMutexWave2); // Lock the mutex
	if (mWave == NULL)
	{
		mWave = new CWavePlaying(10, 100000, frame->sample_rate, bitsPerSample, frame->ch_layout.nb_channels);
		mWave->Open();
	}
	if (mWave != NULL)
	{
		mWave->Play((char*)frame->data[0], 0, frame->nb_samples * (bitsPerSample / 8));
	}
	lock1.unlock();
}
void CStereoImageManager::PlayerMute(BOOL mute)
{
	std::unique_lock<std::mutex> lock1(*mMutexWave1); // Lock the mutex
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
	std::unique_lock<std::mutex> lock1(*mMutexWave1); // Lock the mutex
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
	std::unique_lock<std::mutex> lock1(*mMutexWave1); // Lock the mutex
	if (mWave != NULL)
	{
		mWave->SetVolume(volume, volume);
	}
	lock1.unlock();
}