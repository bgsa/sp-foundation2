#include <VideoStreamManager.h>

void sp_video_stream_manager_init(SpVideoStreamManager* manager, SpVideoStream* stream, const sp_uint maxFrameBuffer)
{
    manager->stream = stream;
    manager->maxFrameBuffer = maxFrameBuffer;

    manager->frames.resize(maxFrameBuffer); // FPS
    for (sp_size i = 0; i < maxFrameBuffer; i++)
        manager->frames[i] = (sp_byte*)std::calloc(3 * sp_video_stream_frame_width(stream) * sp_video_stream_frame_height(stream), sizeof(sp_byte));

    manager->isStreamRunning = false;
}

sp_bool sp_video_stream_manager_is_running(const SpVideoStreamManager* const manager)
{
    return manager->isStreamRunning;
}

void sp_video_stream_manager_start(SpVideoStreamManager* manager)
{
    manager->streamThread = std::move(std::thread(sp_video_stream_manager_run, manager));
    manager->streamThread.detach();

    manager->isStreamRunning = true;
}

void sp_video_stream_manager_run(SpVideoStreamManager* streamManager)
{
    while (sp_video_stream_next_packet(streamManager->stream))
    {
        while (sp_video_stream_next_frame(streamManager->stream, streamManager->frames[streamManager->currentFrame]))
        {
            streamManager->frameBufferMutex.lock();
            streamManager->framesBuffer.push(streamManager->frames[streamManager->currentFrame]);
            streamManager->currentFrame++;
            streamManager->frameBufferMutex.unlock();

            if (streamManager->currentFrame == streamManager->maxFrameBuffer)
                streamManager->currentFrame = 0;

            if (streamManager->framesBuffer.size() == streamManager->maxFrameBuffer)
            {
                std::unique_lock videoFrameLock(streamManager->videoFrameMutex);
                streamManager->videoFrameSync.wait(videoFrameLock);
            }
        }
    }

    streamManager->isStreamRunning = false;
}

sp_byte* sp_video_stream_manager_next_frame(SpVideoStreamManager* manager)
{
    if (manager->framesBuffer.empty())
        return nullptr;

    manager->frameBufferMutex.lock();
    sp_byte* frame = manager->framesBuffer.front();
    manager->framesBuffer.pop();
    manager->frameBufferMutex.unlock();

    manager->videoFrameSync.notify_all();

    return frame;
}