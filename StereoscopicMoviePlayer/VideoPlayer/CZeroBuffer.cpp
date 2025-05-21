#include "stdafx.h"
#include "CZeroBuffer.h"

template<typename T>
CZeroBuffer<T>::CZeroBuffer(void* user, dOnTReceived onTReceived)
{
	mUser = user;	
	mOnTReceived = onTReceived;
}

template<typename T>
CZeroBuffer<T>::~CZeroBuffer()
{
}

template<typename T>
void CZeroBuffer<T>::Enqueue(T t)
{
	if (mOnTReceived != NULL)
	{
		mOnTReceived(mUser, t);
	}
}
