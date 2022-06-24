#include <VideoStreamProperties.h>
#include <stdlib.h>

void* sp_video_stream_properties_create()
{
    return std::malloc(sizeof(SpVideoStreamProperties));
}

void sp_video_stream_properties_init(SpVideoStreamProperties* properties)
{
    properties->format = AV_PIX_FMT_NONE;
    properties->width = properties->height = 0;
    properties->channels = 0;
}

void sp_video_stream_properties_init_args(SpVideoStreamProperties* properties, sp_int width, sp_int height, AVPixelFormat format, sp_int channels)
{
    properties->width = width;
    properties->height = height;
    properties->format = format;
    properties->channels = channels;
}

void sp_video_stream_properties_release(SpVideoStreamProperties* properties)
{
    if (properties != nullptr)
    {
        std::free(properties);
        properties = nullptr;
    }
}