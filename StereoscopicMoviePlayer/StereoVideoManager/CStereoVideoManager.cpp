#include "stdafx.h"
#include "CStereoVideoManager.h"
#include <thread>
#include <Windows.h>
#include <iostream>

CStereoVideoManager::CStereoVideoManager(HWND hWnd)
{
	mHWnd = hWnd;
	mStereoDirect2D = new CStereoDirect2D(hWnd);
	mComPortName = std::wstring(L"");
	mComPort = NULL;
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
CStereoVideoManager::~CStereoVideoManager()
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
	if (mStereoDirect2D != NULL)
	{
		delete mStereoDirect2D;
		mStereoDirect2D = NULL;
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
	if (mMutexRender2 != nullptr)
	{
		delete mMutexRender2;
		mMutexRender2 = nullptr;
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
	if (mMutexWave2 != nullptr)
	{
		delete mMutexWave2;
		mMutexWave2 = nullptr;
	}
	//----------------------------------------------------
	if (mMutexWave1 != nullptr)
	{
		delete mMutexWave1;
		mMutexWave1 = nullptr;
	}
}
void CStereoVideoManager::StereoStart()
{
	if (!mThreadRenderRunning.load())
	{
		mThreadRenderRunning = true;
		mThreadRender = new std::thread(&CStereoVideoManager::ThreadRenderFunction, this);
	}
}
void CStereoVideoManager::StereoStop()
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
	std::unique_lock<std::mutex> lock1(*mMutexRender1); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->Blt(TRUE, this, nullptr);
	}
	lock1.unlock();
}
BOOL CStereoVideoManager::StereoIsStarted()
{
	return mThreadRenderRunning.load();
}
int CStereoVideoManager::StereoGetFrequency()
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		return mStereoDirect2D->GetFrequency();
	}
	lock1.unlock();
	return 0;
}
void CStereoVideoManager::StereoSetCOMPort(LPCWSTR comPort)
{
	if (mComPortName != std::wstring(comPort))
	{
		std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
		if (mComPort != NULL)
		{
			delete mComPort;
			mComPort = NULL;
		}
		mComPort = new CComPort(std::wstring(comPort));
		mComPortName = std::wstring(comPort);
		lock1.unlock();
	}
}
void CStereoVideoManager::StereoSetGlassesTimeOffset(int offset)
{
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		mComPort->SendGlassesTimeOffset(offset);
	}
	lock1.unlock();
}
void CStereoVideoManager::StereoLRBoth(int lrboth)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoLRBoth(lrboth);
	}
	lock1.unlock();
}
void CStereoVideoManager::StereoSwapLR(BOOL swaplr)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoSwapLR(swaplr);
	}
	lock1.unlock();
}
void CStereoVideoManager::StereoVerticalLR(BOOL verticallr)
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoVerticalLR(verticallr);
	}
	lock1.unlock();
}
void CStereoVideoManager::StereoWindowSizeChanged()
{
	std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->StereoWindowSizeChanged();
	}
	lock1.unlock();
}
void CStereoVideoManager::PlayerOpen(LPCWSTR fileName)
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		mPlayer->Open(std::wstring(fileName));
		mPlayer->Stop();
	}
	lock1.unlock();
}
void CStereoVideoManager::PlayerClose()
{
	std::unique_lock<std::mutex> lock2(*mMutexRender2); // Lock the mutex
	if (mStereoDirect2D != NULL)
	{
		mStereoDirect2D->DrawImage(NULL);
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
BOOL CStereoVideoManager::PlayerIsOpened()
{
	std::unique_lock<std::mutex> lock1(*mMutexPlayer); // Lock the mutex
	if (mPlayer != NULL)
	{
		return mPlayer->IsOpened();
	}
	lock1.unlock();
	return false;
}
void CStereoVideoManager::PlayerPlay()
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
void CStereoVideoManager::PlayerPause()
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
void CStereoVideoManager::PlayerStop()
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
BOOL CStereoVideoManager::PlayerIsPlaying()
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
BOOL CStereoVideoManager::PlayerIsEOF()
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
INT64 CStereoVideoManager::PlayerGetDuration()
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
INT64 CStereoVideoManager::PlayerGetCurrentPlayingTime()
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
void CStereoVideoManager::PlayerSeek(INT64 seek_target_ms)
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
int CStereoVideoManager::PlayerGetNumberOfAudioTracks()
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
void CStereoVideoManager::PlayerSetAudioTrack(int audio_track_index)
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
void CStereoVideoManager::ThreadRenderFunction()
{
	while (mThreadRenderRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexRender1); // Lock the mutex
		mImageToPlayIsLeft = !mImageToPlayIsLeft;
		if (mStereoDirect2D != NULL)
		{
			mStereoDirect2D->Blt(mImageToPlayIsLeft, this, SendSyncStatic);
		}
		lock1.unlock();
	}
}
void CStereoVideoManager::SendSyncStatic(void* user, int syncType)
{
	if (user == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)user;
	cStereoVideoManager->SendSync(syncType);
}
void CStereoVideoManager::SendSync(int syncType)
{
	std::unique_lock<std::mutex> lock1(*mMutexCOMPort); // Lock the mutex
	if (mComPort != NULL)
	{
		mComPort->SendSync(syncType);
	}
	lock1.unlock();
}
void CStereoVideoManager::OnNewVideoFrameStatic(void* user, AVFrame* frame)
{
	if (user == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)user;
	cStereoVideoManager->OnNewVideoFrame(frame);
}
void CStereoVideoManager::OnNewVideoFrame(AVFrame* frame)
{
	if (mThreadRenderRunning.load())
	{
		std::unique_lock<std::mutex> lock1(*mMutexRender2); // Lock the mutex
		if (mStereoDirect2D != NULL)
		{
			mStereoDirect2D->DrawImage(frame);
		}
		lock1.unlock();
	}
	else
	{
		av_frame_free(&frame);
	}
}
void CStereoVideoManager::OnNewAudioFrameStatic(void* user, AVFrame* frame)
{
	if (user == NULL) return;
	CStereoVideoManager* cStereoVideoManager = (CStereoVideoManager*)user;
	cStereoVideoManager->OnNewAudioFrame(frame);
}
void CStereoVideoManager::OnNewAudioFrame(AVFrame* frame)
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
void CStereoVideoManager::PlayerMute(BOOL mute)
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
UINT16 CStereoVideoManager::PlayerGetVolume()
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
void CStereoVideoManager::PlayerSetVolume(UINT16 volume)
{
	std::unique_lock<std::mutex> lock1(*mMutexWave1); // Lock the mutex
	if (mWave != NULL)
	{
		mWave->SetVolume(volume, volume);
	}
	lock1.unlock();
}