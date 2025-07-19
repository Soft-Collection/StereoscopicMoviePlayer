#include "stdafx.h"
#include "CDeviationBuffer.h"

#define increment_modulo(value, modulo) ((value) = (((value) < ((modulo) - 1)) ? (value) + 1 : 0))

CDeviationBuffer::CDeviationBuffer(DOUBLE threshold)
{
    mThreshold = threshold;
    Clear();
}

CDeviationBuffer::~CDeviationBuffer()
{
}

void CDeviationBuffer::Clear()
{
    memset((void*)mBuffer, 0, sizeof(INT) * DEVIATION_BUFFER_SIZE);
    mSum = 0;
    mHead = 0;
    mTail = 0;
    mSize = 0;
    mAverage = 0.0;
    mDelayTimeInUS = 30000;
}

void CDeviationBuffer::AddDeviation(INT value)
{
    if (mSize < DEVIATION_BUFFER_SIZE)
    {
        mSize++;
    }
    else // mSize == DEVIATION_BUFFER_SIZE
    {
        mSum -= mBuffer[mTail];
        increment_modulo(mTail, DEVIATION_BUFFER_SIZE);
    }
    mBuffer[mHead] = value;
    mSum += value;
    increment_modulo(mHead, DEVIATION_BUFFER_SIZE);
    mAverage = (DOUBLE)mSum / (DOUBLE)mSize;
    if (mAverage < mThreshold)
    {
        mDelayTimeInUS += 10;
    }
    else
    {
        mDelayTimeInUS -= 10;
    }
}

INT CDeviationBuffer::GetDelayTimeInUS()
{
    return mDelayTimeInUS;
}