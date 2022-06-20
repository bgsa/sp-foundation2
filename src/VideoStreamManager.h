#ifndef SP_VIDEO_STREAM_MANAGER_HEADER
#define SP_VIDEO_STREAM_MANAGER_HEADER

#include <queue>
#include <mutex>
#include <condition_variable>
#include <FFMpeg.h>

#include <SpectrumFoundation.h>
#include <VideoStream.h>

EXTERN_C
{
    struct SpVideoStreamManager
    {
        SpVideoStream* stream;

        std::queue<sp_byte*> framesBuffer;
        std::vector<sp_byte*> frames;
        sp_uint maxFrameBuffer;
        sp_uint currentFrame = 0;
        std::mutex frameBufferMutex;

        std::mutex videoFrameMutex;
        std::condition_variable videoFrameSync;

        std::thread streamThread;
        sp_bool isStreamRunning;
    };

    API_EXPORT void sp_video_stream_manager_init(SpVideoStreamManager* manager, SpVideoStream* stream, const sp_uint maxFrameBuffer = 120);

    API_EXPORT sp_bool sp_video_stream_manager_is_running(const SpVideoStreamManager* const manager);

    API_EXPORT sp_byte* sp_video_stream_manager_next_frame(SpVideoStreamManager* manager);

    API_EXPORT void sp_video_stream_manager_start(SpVideoStreamManager* manager);

    API_EXPORT void sp_video_stream_manager_run(SpVideoStreamManager* streamManager);

}

#endif // SP_VIDEO_STREAM_MANAGER_HEADER