#ifndef FFMPEG_HEADER
#define FFMPEG_HEADER

#ifdef FFMPEG_ENABLED

#include <SpectrumFoundation.h>

EXTERN_C
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/opt.h>
	#include <libswscale/swscale.h>
}


#endif // FFMPEG_ENABLED

#endif // FFMPEG_HEADER