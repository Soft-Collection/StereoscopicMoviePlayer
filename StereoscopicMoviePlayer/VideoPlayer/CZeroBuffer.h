#ifndef __CZEROBUFFER_H__
#define __CZEROBUFFER_H__

#include "CFFCommon.h"

template<typename T>
class CZeroBuffer
{
private:
	typedef void(*dOnTReceived)(void* user, T t);
private:
	void*             mUser;
	dOnTReceived      mOnTReceived;
public:
	CZeroBuffer(void* user, dOnTReceived onTReceived);
	~CZeroBuffer();
	void Enqueue(T packet);
};

#endif //__CZEROBUFFER_H__