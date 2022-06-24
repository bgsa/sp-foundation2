#ifndef SP_VIDEO_STREAM_HEADER
#define SP_VIDEO_STREAM_HEADER

#include <FFMpeg.h>

#include <SpectrumFoundation.h>
#include <VideoStreamProperties.h>

EXTERN_C
{
    struct SpVideoStream
    {
        AVFormatContext* streamContext;
        AVCodecContext* videoCodecContext;
        AVCodecID videoCodecId;
        AVCodecID audioCodecId;
        sp_int videoStreamIndex;
        sp_int width, height;
        sp_uint64 duration;
        sp_double bitRate;
        sp_float framesPerSeconds;
        sp_int readFrameStatus;

        SpVideoStreamProperties* outputProperties;
    };

    API_EXPORT void* sp_video_stream_create();

    API_EXPORT void sp_video_stream_init(SpVideoStream* stream);

    API_EXPORT void sp_video_stream_open(SpVideoStream* stream, const sp_char* url, SpVideoStreamProperties* outputProperties);

    API_EXPORT sp_int sp_video_stream_frame_number(const SpVideoStream* const stream);

    API_EXPORT sp_int sp_video_stream_frame_width(const SpVideoStream* const stream);

    API_EXPORT sp_int sp_video_stream_frame_height(const SpVideoStream* const stream);

    API_EXPORT sp_uint sp_video_stream_duration(const SpVideoStream* const stream);

    API_EXPORT sp_float sp_video_stream_frame_rate(const SpVideoStream* const stream);

    API_EXPORT sp_bool sp_video_stream_next_packet(SpVideoStream* stream);

    API_EXPORT sp_bool sp_video_stream_next_frame(SpVideoStream* stream, sp_byte* data);

    API_EXPORT void sp_video_stream_close(SpVideoStream* stream);

    API_EXPORT void sp_video_stream_release(SpVideoStream* stream);

}

#endif // SP_VIDEO_STREAM_HEADER