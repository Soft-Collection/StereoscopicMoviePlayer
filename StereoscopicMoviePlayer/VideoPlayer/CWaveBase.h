#if !defined(__CWAVEBASE_H__)
#define __CWAVEBASE_H__

#include "CFFCommon.h"

class CWaveBase 
{
public:
	enum 
	{
		SPS_8K=8000,
		SPS_11K=11025,
		SPS_22K=22050,
		SPS_44K=44100
	};
	enum
	{
		CH_MONO=1,
		CH_STEREO=2
	};
public:
	const char* m_result;
	MMRESULT res;
public:
	CWaveBase();
	virtual ~CWaveBase();
	const char* GetLastError();
	void GetMMResult(MMRESULT res);
};

#endif // !defined(__CWAVEBASE_H__)
