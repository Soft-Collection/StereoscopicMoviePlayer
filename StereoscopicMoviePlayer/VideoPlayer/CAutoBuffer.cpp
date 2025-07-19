#include "stdafx.h"
#include "CAutoBuffer.h"

#define BUFFER_SIZE 20
#define LOWER_SIZE 10

template<typename T>
CAutoBuffer<T>::CAutoBuffer(void* user, dOnTReceived onTReceived, dOnClear onClear, dProperty1 prop1)
{
	mMutexBuffer = new std::mutex();
	//-------------------------------------------------------
	mQueueEmptyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	mQueueFullEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	//-------------------------------------------------------
	mUser = user;
	mOnTReceived = onTReceived;
	mOnClear = onClear;
	//-------------------------------------------------------
	mQueue = new CVectorQueue<T>(prop1);
	//-------------------------------------------------------
	mBufferThreadRunning.store(true);
	mBufferThread = new std::thread(&CAutoBuffer::MyBufferThreadFunction, this);
}

template<typename T>
CAutoBuffer<T>::~CAutoBuffer()
{
	Clear();
	if (mBufferThreadRunning.load())
	{
		SetEvent(mQueueEmptyEvent);
		mBufferThreadRunning.store(false);
		if (mBufferThread && mBufferThread->joinable())
		{
			mBufferThread->join();
		}
		delete mBufferThread;
		mBufferThread = nullptr;
	}
	if (mQueue != NULL)
	{
		delete mQueue;
		mQueue = NULL;
	}
	if (mQueueFullEvent != nullptr)
	{
		CloseHandle(mQueueFullEvent);
		mQueueFullEvent = nullptr;
	}
	if (mQueueEmptyEvent != nullptr)
	{
		CloseHandle(mQueueEmptyEvent);
		mQueueEmptyEvent = nullptr;
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
	WaitForSingleObject(mQueueFullEvent, INFINITE);
	std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
	if (mQueue != NULL)
	{
		mQueue->push(t);
		if (mQueue->size() >= BUFFER_SIZE) ResetEvent(mQueueFullEvent);
		if (mQueue->size() >= LOWER_SIZE) SetEvent(mQueueEmptyEvent);
	}
	lock1.unlock();
}

template<typename T>
void CAutoBuffer<T>::Clear()
{
	std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
	if (mQueue != NULL)
	{
		while (TRUE)
		{
			if (mQueue->empty())
			{
				ResetEvent(mQueueEmptyEvent);
				break;
			}
			T temp = mQueue->front();
			mQueue->pop();
			SetEvent(mQueueFullEvent);
			if (mOnClear != NULL) mOnClear(&temp);
		}
	}
	lock1.unlock();
}

template<typename T>
void CAutoBuffer<T>::MyBufferThreadFunction()
{
	while (mBufferThreadRunning.load())
	{
		WaitForSingleObject(mQueueEmptyEvent, INFINITE);
		std::unique_lock<std::mutex> lock1(*mMutexBuffer); // Lock the mutex
		if (mQueue != NULL)
		{
			if (!mQueue->empty())
			{
				T temp = mQueue->front();
				mQueue->pop();
				if (mQueue->size() < LOWER_SIZE) ResetEvent(mQueueEmptyEvent);
				SetEvent(mQueueFullEvent);
				lock1.unlock();
				if (mOnTReceived != NULL) mOnTReceived(mUser, temp, mQueue->size() - ((BUFFER_SIZE + LOWER_SIZE) / 2));
			}
		}
	}
}