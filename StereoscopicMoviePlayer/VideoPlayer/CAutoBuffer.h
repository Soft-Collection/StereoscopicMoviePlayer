#ifndef __CAUTOBUFFER_H__
#define __CAUTOBUFFER_H__

#include "CFFCommon.h"
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

template<typename T>
class CAutoBuffer
{
private:
	typedef void(*dOnTReceived)(void* user, T t);
	typedef void(*dOnClear)(T* t);
private:
	std::mutex*       mMutexBuffer;
	std::thread*      mBufferThread;
	std::atomic<bool> mBufferThreadRunning;
	void*             mUser;
	dOnTReceived      mOnTReceived;
	dOnClear          mOnClear;
	std::queue<T>*    mQueue;
public:
	CAutoBuffer(void* user, dOnTReceived onTReceived, dOnClear onClear);
	~CAutoBuffer();
	void Enqueue(T packet);
	void Clear();
private:
	void MyBufferThreadFunction();
};

#endif //__CAUTOBUFFER_H__