#include "stdafx.h"
#include "CWaveRecording.h"

CWaveRecording::CWaveRecording(DWORD buffersCount, DWORD bufferLength, DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels)
{
	InitializeCriticalSection(&m_HeadersCriticalSection);
	InitializeCriticalSection(&m_QueueCriticalSection);
	mHWaveIn = NULL;
	mRecordingState = Initialized;
	mBuffersAreReady = FALSE;
	mHeadersAreReady = FALSE;
   	mPCMfmt.cbSize = 0;
	mPCMfmt.wFormatTag = WAVE_FORMAT_PCM;
	mPCMfmt.nChannels = nChannels;
	mPCMfmt.nSamplesPerSec = nSamplesPerSec;
	mPCMfmt.wBitsPerSample = wBitsPerSample;
	mPCMfmt.nBlockAlign =     mPCMfmt.nChannels * (mPCMfmt.wBitsPerSample / 8);
	mPCMfmt.nAvgBytesPerSec = mPCMfmt.nBlockAlign * mPCMfmt.nSamplesPerSec;
	mBuffersCount = buffersCount;
	mBufferLength = bufferLength;
	mBuffers = NULL;
	mWaveHeaders = NULL;
	m_queue = new std::queue<WAVEINPROCDATA>;
	m_hevBufferArrived = CreateEvent(NULL, TRUE, FALSE, NULL); //manual reset event
	mCloseRequestPerformed = FALSE;
}
CWaveRecording::~CWaveRecording()
{
	Close();
	delete m_queue;
	DeleteCriticalSection(&m_QueueCriticalSection);
	DeleteCriticalSection(&m_HeadersCriticalSection);
}
BOOL CWaveRecording::Open()
{
	if (mRecordingState != Initialized) return FALSE;
	res = waveInOpen(&mHWaveIn, (UINT)WAVE_MAPPER, &mPCMfmt, (DWORD)WaveInProc, (DWORD)this, CALLBACK_FUNCTION);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR)
	{
		mHWaveIn = NULL;
		return FALSE;
	}
	if (!PrepareBuffers()) return FALSE;
	if (!PrepareHeaders()) return FALSE;
	if (!AddBuffers()) return FALSE;
	mCloseRequestPerformed = FALSE;
	mRecordingState = Opened;
	return TRUE;
}
BOOL CWaveRecording::Close()
{
	if (mRecordingState ==  RecordingStarted)
	{
		if (!StopRecord()) return FALSE;
	}
	if (mRecordingState !=  Opened) return FALSE;
	mCloseRequestPerformed = TRUE;
	EnterCriticalSection(&m_HeadersCriticalSection);
	while (m_queue->size() != mBuffersCount)
	{
		Sleep(200);
		res = waveInReset(mHWaveIn);
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR)
		{
			LeaveCriticalSection(&m_HeadersCriticalSection);
			return FALSE;
		}
	}
	LeaveCriticalSection(&m_HeadersCriticalSection);
	if (!UnprepareHeaders()) return FALSE;
	if (!DestroyBuffers()) return FALSE;
	res = waveInClose(mHWaveIn);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mHWaveIn = NULL;
	mRecordingState =  Initialized;
	return (TRUE);
}
BOOL CWaveRecording::GetRecordingState(DWORD* recordingState)
{
	*recordingState = (DWORD)mRecordingState;
	return (TRUE);
}
BOOL CWaveRecording::PrepareBuffers()
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
BOOL CWaveRecording::DestroyBuffers()
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
BOOL CWaveRecording::PrepareHeaders()
{
	BOOL retVal = TRUE;
	EnterCriticalSection(&m_HeadersCriticalSection);
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		res = waveInPrepareHeader(mHWaveIn, &mWaveHeaders[i], sizeof(WAVEHDR));
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR) retVal = FALSE;
	}
	mHeadersAreReady = TRUE;
	LeaveCriticalSection(&m_HeadersCriticalSection);
    return retVal;
}
BOOL CWaveRecording::AddBuffers()
{
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		res = waveInAddBuffer(mHWaveIn, &mWaveHeaders[i], sizeof(WAVEHDR));
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR) return FALSE;
	}
    return TRUE;
}
BOOL CWaveRecording::UnprepareHeaders()
{
	BOOL retVal = TRUE;
	EnterCriticalSection(&m_HeadersCriticalSection);
	mHeadersAreReady = FALSE;
	for (UINT i = 0; i < mBuffersCount; i++)
	{
		res = waveInUnprepareHeader(mHWaveIn, &mWaveHeaders[i], sizeof(WAVEHDR));
		GetMMResult(res);
		if (res != MMSYSERR_NOERROR) retVal = FALSE;
	}
	LeaveCriticalSection(&m_HeadersCriticalSection);
    return retVal;
}
BOOL CWaveRecording::GetBuffer(char* pData, ULONG offset, ULONG* size)
{
	BOOL retVal = TRUE;
	DWORD dwObject = WaitForSingleObject(m_hevBufferArrived, mBufferLength * 10000 / (mPCMfmt.nSamplesPerSec * mPCMfmt.nChannels));
	if (dwObject == WAIT_TIMEOUT) retVal = FALSE;
	else
	{
		if (mRecordingState !=  RecordingStarted) retVal = FALSE;
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
					WAVEINPROCDATA wiProcData = m_queue->front();
					LeaveCriticalSection(&m_QueueCriticalSection);	
					CWaveRecording* pWave = (CWaveRecording*) wiProcData.dwInstance;
					CopyMemory(pData + (offset * (mPCMfmt.wBitsPerSample / 8)), ((WAVEHDR*)wiProcData.dwParam1)->lpData, ((WAVEHDR*)wiProcData.dwParam1)->dwBytesRecorded);
					*size = ((WAVEHDR*)wiProcData.dwParam1)->dwBytesRecorded / (mPCMfmt.wBitsPerSample / 8);
					if (mCloseRequestPerformed) retVal = FALSE;
					else
					{
						res = waveInAddBuffer(wiProcData.hwi, (WAVEHDR*)wiProcData.dwParam1, sizeof(WAVEHDR)); //May be reused.
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
			}
			LeaveCriticalSection(&m_HeadersCriticalSection);
		}
	}
	return retVal;
}
BOOL CWaveRecording::StartRecord()
{
	if (mRecordingState !=  Opened) return FALSE;
	res = waveInStart(mHWaveIn);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mRecordingState =  RecordingStarted;
	return (TRUE);
}
BOOL CWaveRecording::StopRecord()
{
	if (mRecordingState !=  RecordingStarted) return FALSE;
	res = waveInReset(mHWaveIn);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	res = waveInStop(mHWaveIn);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR) return FALSE;
	mRecordingState =  Opened;
	return (TRUE);
}
BOOL CWaveRecording::GetPosition(LPMMTIME pmmt)
{
	res = waveInGetPosition(mHWaveIn, pmmt, sizeof(MMTIME));
	GetMMResult(res);
	return (res == MMSYSERR_NOERROR);
}
BOOL CWaveRecording::GetDevCaps(LPWAVEINCAPS pwic)
{
	return (waveInGetDevCaps(WAVE_MAPPER, pwic, sizeof(WAVEINCAPS)) == MMSYSERR_NOERROR);
}
void CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg == WIM_DATA)
	{
		CWaveRecording* pWave = (CWaveRecording*) dwInstance;
		WAVEINPROCDATA wiProcData = {hwi, uMsg, dwInstance, dwParam1, dwParam2};
		EnterCriticalSection(&pWave->m_QueueCriticalSection);
		pWave->m_queue->push(wiProcData);
		LeaveCriticalSection(&pWave->m_QueueCriticalSection);
		SetEvent(pWave->m_hevBufferArrived);
	}
}