#ifndef __CFFCOMMON_H__
#define __CFFCOMMON_H__

#include <windows.h>
#include "MMSystem.h"

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavdevice/avdevice.h"
	#include "libavformat/avformat.h"
	#include "libavformat/avio.h"
	#include "libswscale/swscale.h"
	#include "libswresample/swresample.h"
	#include "libavutil/imgutils.h"
	#include "libavutil/opt.h"
	#include "libavutil/log.h"
}

class CFFCommon
{
public:
	typedef struct
	{
		AVPixelFormat    PixelFormatsID;
		int              ResizedWidth;
		int              ResizedHeight;
	} ColorTargetParams;
	typedef struct
	{
		AVSampleFormat   SampleFormatsID;
		AVChannelLayout  ChannelLayout;
		int              SampleRate;
	} SampleTargetParams;
};

#endif //__CFFCOMMON_H__