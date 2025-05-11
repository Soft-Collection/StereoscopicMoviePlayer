#include "stdafx.h"
#include "CWavePlaying.h"

CWavePlaying::CWavePlaying(DWORD buffersCount, DWORD bufferLength, DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels)
{
	InitializeCriticalSection(&m_HeadersCriticalSection);
	InitializeCriticalSection(&m_QueueCriticalSection);
	mHWaveOut = NULL;
	mPlayingState =  Initialized;
	mLeftVolume = 0;
	mRightVolume = 0;
	mIsMuted = FALSE;
	mBuffersAreReady = FALSE;
	mHeadersAreReady = FALSE;
	mPCMfmt.wFormatTag = WAVE_FORMAT_PCM;
	mPCMfmt.nChannels = nChannels;
	mPCMfmt.nSamplesPerSec = nSamplesPerSec;
	mPCMfmt.wBitsPerSample = wBitsPerSample;
	mPCMfmt.nBlockAlign =     mPCMfmt.nChannels * (mPCMfmt.wBitsPerSample / 8);
	mPCMfmt.nAvgBytesPerSec = mPCMfmt.nBlockAlign * mPCMfmt.nSamplesPerSec;
	mPCMfmt.cbSize = 0;
	mBuffersCount = buffersCount;
	mBufferLength = bufferLength;
	mBuffers = NULL;
	mWaveHeaders = NULL;
	m_queue = new std::queue<WAVEOUTPROCDATA>;
	m_hevBufferArrived = CreateEvent(NULL, TRUE, TRUE, NULL); //manual reset event
}
CWavePlaying::~CWavePlaying()
{
	Close();
	delete m_queue;
	DeleteCriticalSection(&m_QueueCriticalSection);
	DeleteCriticalSection(&m_HeadersCriticalSection);
}
BOOL CWavePlaying::Open()
{
	if (mPlayingState != Initialized) return FALSE;
	res = waveOutOpen(&mHWaveOut, (UINT)WAVE_MAPPER, &mPCMfmt, (DWORD)WaveOutProc, (DWORD)this, CALLBACK_FUNCTION);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR)
	{
		//WCHAR errmsg[MAXERRORLENGTH + 1];
		//MMRESULT converr;
		//converr = waveOutGetErrorTextW(res, errmsg, MAXERRORLENGTH + 1);
		mHWaveOut = NULL;
		return FALSE;
	}
	if (!PrepareBuffers()) return FALSE;
	if (!PrepareHeaders()) return FALSE;
	mPlayingState = Opened;
	return TRUE;
}
BOOL CWavePlaying::Close()
{
	if (mPlayingState == PlayingStarted)
	{
		if (!Stop()) return FALSE;
	}
	if (!UnprepareHeaders()) return FALSE;
	if (!DestroyBuffers()) return FALSE;
	res = waveOutClose(mHWaveOut);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mHWaveOut = NULL;
	mPlayingState = Initialized;
	return (TRUE);
}
BOOL CWavePlaying::GetPlayingState(DWORD* playingState)
{
	*playingState = (DWORD)mPlayingState;
	return (TRUE);
}
BOOL CWavePlaying::PrepareBuffers()
{
	mBuffers = new char*[mBuffersCount];
	if (mBuffers == NULL) return FALSE;
	//------------------------------------
	mWaveHeaders = new WAVEHDR[mBuffersCount];
	if (mWaveHeaders == NULL) return FALSE;
	//------------------------------------
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		mBuffers[i] = new char[mBufferLength * (mPCMfmt.wBitsPerSample / 8)];
		if (mBuffers[i] == NULL) return FALSE;
		//------------------------------------
		mWaveHeaders[i].lpData = mBuffers[i];
		mWaveHeaders[i].dwBufferLength = mBufferLength * (mPCMfmt.wBitsPerSample / 8);
		mWaveHeaders[i].dwBytesRecorded = 0;
		mWaveHeaders[i].dwUser = 0;
		mWaveHeaders[i].dwFlags = 0;
		mWaveHeaders[i].reserved = 0;
		mWaveHeaders[i].lpNext = 0;
	}
	mBuffersAreReady = TRUE;
    return TRUE;
}
BOOL CWavePlaying::DestroyBuffers()
{
	mBuffersAreReady = FALSE;
	//------------------------------------
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		if (mBuffers[i] != NULL)
		{
			delete[] mBuffers[i];
			mBuffers[i] = NULL;
		}
	}
	//------------------------------------
	if (mBuffers != NULL)
	{
		delete[] mBuffers;
		mBuffers = NULL;
	}
	//------------------------------------
	if (mWaveHeaders != NULL)
	{
		delete[] mWaveHeaders;
		mWaveHeaders = NULL;
	}
    return TRUE;
}
BOOL CWavePlaying::PrepareHeaders()
{
	BOOL retVal = TRUE;
	EnterCriticalSection(&m_HeadersCriticalSection);
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		res = waveOutPrepareHeader(mHWaveOut, &mWaveHeaders[i], sizeof(WAVEHDR));
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR) retVal = FALSE;
		WAVEOUTPROCDATA woProcData = {mHWaveOut, 0, (DWORD)this, (DWORD)&mWaveHeaders[i], 0};
		EnterCriticalSection(&m_QueueCriticalSection);
		m_queue->push(woProcData);
		LeaveCriticalSection(&m_QueueCriticalSection);
	}
	mHeadersAreReady = TRUE;
	LeaveCriticalSection(&m_HeadersCriticalSection);
    return retVal;
}
BOOL CWavePlaying::UnprepareHeaders()
{
	BOOL retVal = TRUE;
	EnterCriticalSection(&m_HeadersCriticalSection);
	mHeadersAreReady = FALSE;
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		res = waveOutUnprepareHeader(mHWaveOut, &mWaveHeaders[i], sizeof(WAVEHDR));
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR) retVal = FALSE;
	}
	LeaveCriticalSection(&m_HeadersCriticalSection);
    return retVal;
}
BOOL CWavePlaying::Play(char* pData, ULONG offset, ULONG size)
{
	BOOL retVal = TRUE;
	DWORD dwObject = WaitForSingleObject(m_hevBufferArrived, mBufferLength * 10000 / (mPCMfmt.nSamplesPerSec * mPCMfmt.nChannels));
	if (dwObject == WAIT_TIMEOUT) retVal = FALSE;
	else
	{
		if ((mPlayingState !=  Opened) && (mPlayingState !=  PlayingStarted)) retVal = FALSE;
		else
		{
			EnterCriticalSection(&m_HeadersCriticalSection);
			if (!mHeadersAreReady) retVal = FALSE;
			else
			{
				if (m_queue->size() <= 0) retVal = FALSE;
				else
				{
					EnterCriticalSection(&m_QueueCriticalSection);
					WAVEOUTPROCDATA woProcData = m_queue->front();
					LeaveCriticalSection(&m_QueueCriticalSection);
					CWavePlaying* pWave = (CWavePlaying*) woProcData.dwInstance;
					CopyMemory(((WAVEHDR*)woProcData.dwParam1)->lpData, pData + (offset * (mPCMfmt.wBitsPerSample / 8)), size * (mPCMfmt.wBitsPerSample / 8));
					((WAVEHDR*)woProcData.dwParam1)->dwBufferLength = size * (mPCMfmt.wBitsPerSample / 8);
					res = waveOutWrite(mHWaveOut, (WAVEHDR*)woProcData.dwParam1, sizeof(WAVEHDR));
					GetMMResult(res);
					if (res != MMSYSERR_NOERROR) retVal = FALSE;
					else
					{
						EnterCriticalSection(&m_QueueCriticalSection);
						m_queue->pop();
						LeaveCriticalSection(&m_QueueCriticalSection);
						if (m_queue->size() == 0) ResetEvent(m_hevBufferArrived);
					}
				}
			}
			LeaveCriticalSection(&m_HeadersCriticalSection);
		}
	}
	if (retVal) mPlayingState =  PlayingStarted;
	return retVal;
}
BOOL CWavePlaying::Stop()
{
	if ((mPlayingState !=  PlayingStarted) && (mPlayingState !=  PlayingPaused)) return FALSE;
	res = waveOutReset(mHWaveOut);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mPlayingState =  Opened;
	return (TRUE);
}
BOOL CWavePlaying::Pause()
{
	if (mPlayingState !=  PlayingStarted) return FALSE;
	res = waveOutPause(mHWaveOut);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mPlayingState =  PlayingPaused;
	return (TRUE);
}
BOOL CWavePlaying::Resume()
{
	if (mPlayingState !=  PlayingPaused) return FALSE;
	res = waveOutRestart(mHWaveOut);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mPlayingState =  PlayingStarted;
	return (TRUE);
}
BOOL CWavePlaying::GetPitch(LPDWORD pdwPitch)
{
	res = waveOutGetPitch(mHWaveOut, pdwPitch);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::SetPitch(DWORD dwPitch)
{
	res = waveOutSetPitch(mHWaveOut, dwPitch);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::GetPlaybackRate(LPDWORD pdwRate)
{
	res = waveOutGetPlaybackRate(mHWaveOut, pdwRate);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::SetPlaybackRate(DWORD dwRate)
{
	res = waveOutSetPlaybackRate(mHWaveOut, dwRate);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::GetVolume(LPWORD pwLeftVolume, LPWORD pwRightVolume)
{
	DWORD pdwVolume;
	res = waveOutGetVolume(mHWaveOut, &pdwVolume);
	*pwLeftVolume = (WORD)((pdwVolume << 16) >> 16);
	*pwRightVolume = (WORD)(pdwVolume >> 16);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::SetVolume(WORD wLeftVolume, WORD wRightVolume)
{
	DWORD dwVolume = wRightVolume;
	dwVolume <<= 16;
	dwVolume += wLeftVolume;
	res = waveOutSetVolume(mHWaveOut, dwVolume);
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::Mute()
{
	if (!mIsMuted)
	{
		GetVolume(&mLeftVolume, &mRightVolume);
		SetVolume(0, 0);
		mIsMuted = TRUE;
	}
	return TRUE;
}
BOOL CWavePlaying::Unmute()
{
	if (mIsMuted)
	{
		SetVolume(mLeftVolume, mRightVolume);
		mIsMuted = FALSE;
	}
	return TRUE;
}
BOOL CWavePlaying::IsMuted()
{
	return mIsMuted;
}
BOOL CWavePlaying::GetPosition(LPMMTIME pmmt)
{
	res = waveOutGetPosition(mHWaveOut, pmmt, sizeof(MMTIME));
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWavePlaying::GetDevCaps(LPWAVEOUTCAPS pwoc)
{
	return (waveOutGetDevCaps(WAVE_MAPPER, pwoc, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR);
}
void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg == WOM_DONE)
	{
		CWavePlaying* pWave = (CWavePlaying*) dwInstance;
		WAVEOUTPROCDATA woProcData = {hwo, uMsg, dwInstance, dwParam1, dwParam2};
		EnterCriticalSection(&pWave->m_QueueCriticalSection);
		pWave->m_queue->push(woProcData);
		LeaveCriticalSection(&pWave->m_QueueCriticalSection);
		SetEvent(pWave->m_hevBufferArrived);
	}
}