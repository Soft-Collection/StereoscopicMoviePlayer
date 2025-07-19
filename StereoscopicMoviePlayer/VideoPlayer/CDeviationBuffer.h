#ifndef DeviationBuffer_h
#define DeviationBuffer_h

#include "CFFCommon.h"

#define DEVIATION_BUFFER_SIZE 20

class CDeviationBuffer
{
public:
  CDeviationBuffer(DOUBLE threshold);
  ~CDeviationBuffer();
  void Clear();
  void AddDeviation(INT value);
  INT GetDelayTimeInUS();
private:
  INT mBuffer[DEVIATION_BUFFER_SIZE];
  INT mSum;
  INT mHead;
  INT mTail;
  INT mSize;
  DOUBLE mThreshold;
  DOUBLE mAverage;
  INT mDelayTimeInUS;
};

#endif // CDeviationBuffer_h