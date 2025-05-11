#if !defined(__CWAVERECORDING_H__)
#define __CWAVERECORDING_H__

#include <queue>
#include "CWaveBase.h"

typedef struct 
{
	HWAVEIN hwi;
	UINT uMsg;
	DWORD dwInstance;
	DWORD dwParam1;
	DWORD dwParam2;
} WAVEINPROCDATA;

void CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

class CWaveRecording : public CWaveBase 
{
public:
	enum eRecordingState
	{
		Initialized = 0,
		Opened = 1,
		RecordingStarted = 2
	};
	CWaveRecording(DWORD buffersCount, DWORD bufferLength, DWORD nSamplesPerSec, WORD wBitsPerSample, WORD nChannels);
	virtual ~CWaveRecording();
	BOOL Open();
	BOOL Close();
	BOOL GetRecordingState(DWORD* recordingState);
	BOOL GetBuffer(char* pData, ULONG offset, ULONG* size);
	BOOL StartRecord();
	BOOL StopRecord();
	BOOL GetPosition(LPMMTIME pmmt);
	static BOOL GetDevCaps(LPWAVEINCAPS pwic);
private:
	BOOL PrepareBuffers();
	BOOL DestroyBuffers();
	BOOL PrepareHeaders();
	BOOL AddBuffers();
	BOOL UnprepareHeaders();
public:
	HANDLE m_hevBufferArrived;
	std::queue<WAVEINPROCDATA>* m_queue;
	CRITICAL_SECTION m_QueueCriticalSection;
private:
	HWAVEIN mHWaveIn;
	eRecordingState mRecordingState;
	BOOL mBuffersAreReady;
	BOOL mHeadersAreReady;
	DWORD mBuffersCount;
	DWORD mBufferLength;
	char** mBuffers;
	WAVEHDR* mWaveHeaders;
	WAVEFORMATEX mPCMfmt;
	CRITICAL_SECTION m_HeadersCriticalSection;
	BOOL mCloseRequestPerformed;
};

#endif // !defined(__CWAVERECORDING_H__)
