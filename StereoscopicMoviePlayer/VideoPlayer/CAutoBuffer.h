#ifndef __CAUTOBUFFER_H__
#define __CAUTOBUFFER_H__

#include "CFFCommon.h"
#include "CVectorQueue.h"
#include "CVectorQueue.cpp"
#include <mutex>
#include <thread>
#include <atomic>

template<typename T>
class CAutoBuffer
{
private:
	typedef void(*dOnTReceived)(void* user, T t, INT deviation);
	typedef void(*dOnClear)(T* t);
	typedef void(*dProperty1)(T a, INT64** b);
private:
	std::mutex*       mMutexBuffer;
	std::thread*      mBufferThread;
	std::atomic<bool> mBufferThreadRunning;
	HANDLE            mQueueEmptyEvent;
	HANDLE            mQueueFullEvent;
	void*             mUser;
	dOnTReceived      mOnTReceived;
	dOnClear          mOnClear;
	CVectorQueue<T>*  mQueue;
public:
	CAutoBuffer(void* user, dOnTReceived onTReceived, dOnClear onClear, dProperty1 prop1);
	~CAutoBuffer();
	void Enqueue(T packet);
	void Clear();
private:
	void MyBufferThreadFunction();
};

#endif //__CAUTOBUFFER_H__