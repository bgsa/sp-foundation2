#ifndef SP_VIDEO_STREAM_PROPERTIES_HEADER
#define SP_VIDEO_STREAM_PROPERTIES_HEADER

#include <FFMpeg.h>

#include <SpectrumFoundation.h>

EXTERN_C
{
    struct SpVideoStreamProperties
    {
        AVPixelFormat format;
        sp_int width;
        sp_int height;
        sp_int channels;
    };

    API_EXPORT inline void sp_video_stream_properties_init(SpVideoStreamProperties* properties)
    {
        properties->format = AV_PIX_FMT_NONE;
        properties->width = properties->height = 0;
        properties->channels = 0;
    }

    API_EXPORT inline void sp_video_stream_properties_init_args(SpVideoStreamProperties* properties, sp_int width, sp_int height, AVPixelFormat format = AV_PIX_FMT_RGB24, sp_int channels = 3)
    {
        properties->width = width;
        properties->height = height;
        properties->format = format;
        properties->channels = channels;
    }

}

#endif // SP_VIDEO_STREAM_PROPERTIES_HEADER