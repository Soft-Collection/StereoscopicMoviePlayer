#include "stdafx.h"
#include "CWaveBase.h"
#include "Mmsystem.h"

CWaveBase::CWaveBase()
{
}
CWaveBase::~CWaveBase()
{
}
void CWaveBase::GetMMResult(MMRESULT res)
{
	switch (res)
	{
	case MMSYSERR_ALLOCATED: 
		m_result = "Specified resource is already allocated.";
		break;
	case MMSYSERR_BADDEVICEID:
		m_result = "Specified device identifier is out of range.";
		break;
	case MMSYSERR_NODRIVER:
		m_result = "No device driver is present.";
		break;
	case MMSYSERR_NOMEM:
		m_result = "Unable to allocate or lock memory.";
		break;
	case WAVERR_BADFORMAT:
		m_result = "Attempted to open with an unsupported waveform-audio format.";
		break;
	case WAVERR_UNPREPARED:
		m_result = "The buffer pointed to by the pwh parameter hasn't been prepared.";
		break;
	case WAVERR_SYNC:
		m_result = "The device is synchronous but waveOutOpen was called without using the WAVE_ALLOWSYNC flag.";
		break;
	case WAVERR_STILLPLAYING:
		m_result = "The buffer pointed to by the pwh parameter is still in the queue.";
		break;
	case MMSYSERR_NOTSUPPORTED:
		m_result = "Specified device is synchronous and does not support pausing.";
		break;
	case MMSYSERR_NOERROR:
		break;
	default:
		m_result = "Unspecified error";
	}
}
const char* CWaveBase::GetLastError()
{
	return m_result;
}