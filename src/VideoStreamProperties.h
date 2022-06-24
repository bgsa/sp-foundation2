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

    API_EXPORT void* sp_video_stream_properties_create();

    API_EXPORT void sp_video_stream_properties_init(SpVideoStreamProperties* properties);

    API_EXPORT void sp_video_stream_properties_init_args(SpVideoStreamProperties* properties, sp_int width, sp_int height, AVPixelFormat format = AV_PIX_FMT_RGB24, sp_int channels = 3);

    API_EXPORT void sp_video_stream_properties_release(SpVideoStreamProperties* properties);

}

#endif // SP_VIDEO_STREAM_PROPERTIES_HEADER