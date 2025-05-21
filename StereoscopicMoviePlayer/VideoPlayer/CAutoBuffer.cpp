#include "stdafx.h"
#include "CAutoBuffer.h"

template<typename T>
CAutoBuffer<T>::CAutoBuffer(void* user, dOnTReceived onTReceived, dOnClear onClear)
{
	mMutexBuffer = new std::mutex();
	//-------------------------------------------------------
	mUser = user;	
	mOnTReceived = onTReceived;
	mOnClear = onClear;
	//-------------------------------------------------------
	mQueue = new std::queue<T>();
	//-------------------------------------------------------
	mBufferThreadRunning.store(true);
	mBufferThread = new std::thread(&CAutoBuffer::MyBufferThreadFunction, this);
}

template<typename T>
CAutoBuffer<T>::~CAutoBuffer()
{
	if (mBufferThreadRunning.load())
	{
		mBufferThreadRunning.store(false);
		if (mBufferThread && mBufferThread->joinable())
		{
			mBufferThread->join();
		}
		delete mBufferThread;
		mBufferThread = nullptr;
	}
	Clear();	
	if (mQueue != NULL)
	{
		delete mQueue;
		mQueue = NULL;
	}
	if (mMutexBuffer != nullptr)
	{
		delete mMutexBuffer;
		mMutexBuffer = nullptr;
	}
}

template<typename T>
void CAutoBuffer<T>::Enqueue(T t)
{
	std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
	if (mQueue != NULL) mQueue->push(t);
	lock1.unlock();
}

template<typename T>
void CAutoBuffer<T>::Clear()
{
	if (mQueue != NULL)
	{
		while (TRUE)
		{
			std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
			if (mQueue->empty()) break;
			T temp = mQueue->front();
			mQueue->pop();
			if (mOnClear != NULL) mOnClear(&temp);
			lock1.unlock();
		}
	}
}

template<typename T>
void CAutoBuffer<T>::MyBufferThreadFunction()
{
	while (mBufferThreadRunning.load())
	{
		if (mQueue != NULL)
		{
			BOOL sleepNeeded = FALSE;
			std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
			if (!mQueue->empty())
			{
				T temp = mQueue->front();
				mQueue->pop();
				if (mOnTReceived != NULL)
				{
					mOnTReceived(mUser, temp);
				}
			}
			else
			{
				sleepNeeded = TRUE;
			}
			lock1.unlock();
			if (sleepNeeded)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
}