#include <VideoStream.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void sp_video_stream_release_packet(AVPacket* packet)
{
    av_packet_free(&packet);
}

void sp_video_stream_release_frame(AVFrame* frame)
{
    av_frame_free(&frame);
}

void* sp_video_stream_create()
{
    SpVideoStream* stream = (SpVideoStream*) std::malloc(sizeof(SpVideoStream));

#ifdef DEBUG
    //av_log_set_level(AV_LOG_DEBUG | AV_LOG_VERBOSE);
    av_log_set_level(AV_LOG_VERBOSE | AV_LOG_DEBUG | AV_LOG_ERROR);
#else
    av_log_set_level(AV_LOG_QUIET);
#endif

    stream->streamContext = nullptr;
    stream->videoCodecContext = nullptr;
    stream->videoCodecId = AV_CODEC_ID_NONE;
    stream->audioCodecId = AV_CODEC_ID_NONE;
    stream->videoStreamIndex = -1;

    stream->width = stream->height = 0;
    stream->duration = 0;
    stream->bitRate = 0;

    stream->framesPerSeconds = 0.0f;

    stream->readFrameStatus = -1;

    return stream;
}

bool sp_video_stream_init_context(SpVideoStream* stream, const char* url)
{
    sp_ushort attempt = 1;
try_again:

    AVDictionary* options = NULL;
    //av_dict_set(&options, "video_size", "640x480", 0);
    //av_dict_set(&options, "pixel_format", "rgb24", 0);

    stream->streamContext = avformat_alloc_context();
    sp_int err = avformat_open_input(&stream->streamContext, url, nullptr, &options);

    AVDictionaryEntry* e;
    if (e = av_dict_get(options, "", NULL, AV_DICT_IGNORE_SUFFIX))
    {
        fprintf(stderr, "Option %s not recognized by the demuxer.\n", e->key);
    }
    av_dict_free(&options);

    if (err < 0) 
    {
        std::cerr << "cannot open input: " << err << std::endl;
        std::cerr << "Errono: " << errno << std::endl;

        char errorDescription[1024];
        av_strerror(err, errorDescription, 1024);

        std::cerr << errorDescription << std::endl;

        avformat_free_context(stream->streamContext);
        stream->streamContext = nullptr;

        if (errno == EINTR) // Interrupted System Call ??
        {
            if (attempt < 100)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                attempt++;
                std::cerr << "Attempt " << attempt << "..." << std::endl;
                goto try_again;
            }
        }

        return false;
    }

    err = avformat_find_stream_info(stream->streamContext, nullptr);
    if (err < 0) 
    {
        std::cerr << "Error: cannot find stream info: " << err << std::endl;
        avformat_free_context(stream->streamContext);
        stream->streamContext = nullptr;
        return false;
    }

    auto streamsLength = stream->streamContext->nb_streams;
    if (streamsLength == 0)
    {
        std::cerr << "Error: No Streams found! " << std::endl;

        avformat_free_context(stream->streamContext);
        stream->streamContext = nullptr;
        
        if (attempt < 100)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            attempt++;
            std::cerr << "Attempt " << attempt << "..." << std::endl;
            goto try_again;
        }
    }

    return true;
}

bool sp_video_stream_init_codec_context(SpVideoStream* stream)
{
    auto streamsLength = stream->streamContext->nb_streams;

    // Simply finding the first video stream, preferrably H.264. Others are ignored below
    for (sp_uint i = 0; i < streamsLength; i++)
    {
        auto* codecParameters = stream->streamContext->streams[i]->codecpar;

        switch (codecParameters->codec_type)
        {
        case AVMEDIA_TYPE_VIDEO:
        {
            stream->videoStreamIndex = i;
            stream->videoCodecId = codecParameters->codec_id;

            if (codecParameters->codec_id == AV_CODEC_ID_H264)
                break;
        }
        case AVMEDIA_TYPE_AUDIO:
        {
            stream->audioCodecId = codecParameters->codec_id;
            break;
        }
        }
    }

    if (stream->videoStreamIndex < 0)
    {
        std::cerr << "failed to find find video stream" << std::endl;
        avformat_free_context(stream->streamContext);
        stream->streamContext = nullptr;
        return false;
    }

    const AVCodec* decoder = avcodec_find_decoder(stream->videoCodecId);

    stream->videoCodecContext = avcodec_alloc_context3(decoder);

    if (avcodec_open2(stream->videoCodecContext, decoder, NULL) < 0)
    {
        std::cerr << "Error to open Video Decoder" << std::endl;
        avformat_free_context(stream->streamContext);
        stream->streamContext = nullptr;
        return false;
    }

    return true;
}

void sp_video_stream_properties_init(SpVideoStream* stream)
{
    stream->duration = stream->streamContext->duration;
    //auto durationMinutes = (double)streamContext->duration / 1000000 / 60; // minutes

    stream->bitRate = static_cast<sp_double>(stream->streamContext->bit_rate / 1000.0);

    AVStream* videoStream = stream->streamContext->streams[stream->videoStreamIndex];

    if (videoStream->nb_frames == 0)
    {
        stream->framesPerSeconds = static_cast<sp_float>(videoStream->r_frame_rate.num) / videoStream->r_frame_rate.den;
    }
    else
    {
        // auto fpsCalculado = (double)videoStream->nb_frames / streamContext->duration * 1000000;
        stream->framesPerSeconds = static_cast<sp_float>(videoStream->avg_frame_rate.num) / videoStream->avg_frame_rate.den;
    }

    stream->width = videoStream->codecpar->width;
    stream->height = videoStream->codecpar->height;
}

void sp_video_stream_open(SpVideoStream* stream, const sp_char* url, SpVideoStreamProperties* outputProperties)
{
 #ifdef DEBUG
    outputProperties->format = AV_PIX_FMT_RGB24;
    std::cout << "Width: " << outputProperties->width << std::endl;
    std::cout << "Height: " << outputProperties->height << std::endl;
    std::cout << "format: " << outputProperties->format << std::endl;
    std::cout << "channels: " << outputProperties->channels << std::endl;
    std::cout << "URL: " << url << std::endl;
#endif
    stream->outputProperties = outputProperties;

    if (!sp_video_stream_init_context(stream, url))
        return;

    if (!sp_video_stream_init_codec_context(stream))
        return;

    sp_video_stream_properties_init(stream);
}

sp_int sp_video_stream_frame_number(const SpVideoStream* const stream)
{
    assert(stream->videoCodecContext != nullptr);

    return stream->videoCodecContext->frame_number;
}

sp_int sp_video_stream_frame_width(const SpVideoStream* const stream)
{
    return stream->width;
}

sp_int sp_video_stream_frame_height(const SpVideoStream* const stream)
{
    return stream->height;
}

sp_uint sp_video_stream_duration(const SpVideoStream* const stream)
{
    return stream->duration;
}

sp_float sp_video_stream_frame_rate(const SpVideoStream* const stream)
{
    return stream->framesPerSeconds;
}

sp_bool sp_video_stream_next_packet(SpVideoStream* stream)
{
    sp_bool hasVideoFrame = false;

    while (!hasVideoFrame)
    {
        AVPacket* packet = av_packet_alloc();

        stream->readFrameStatus = av_read_frame(stream->streamContext, packet);

        if (stream->readFrameStatus < 0)
        {
            // -541478725   
            // On Error, the packet will be blank
            // Otherelse, it has no more packet

            std::cerr << "read packet error" << std::endl;
            sp_video_stream_release_packet(packet);
            return false;
        }

        //if (currentPacket->stream_index == videoStreamIndex)
        if (packet->stream_index == stream->videoStreamIndex)
        {
            //int sendPacketResult = avcodec_send_packet(videoCodecContext, currentPacket);
            const sp_int sendPacketResult = avcodec_send_packet(stream->videoCodecContext, packet);

            if (sendPacketResult == AVERROR(EAGAIN)) {
                // Decoder can't take packets right now. Make sure you are draining it.
                sp_video_stream_release_packet(packet);
                continue;
            }
            else if (sendPacketResult < 0) {
                // Failed to send the packet to the decoder
                sp_video_stream_release_packet(packet);
                continue;
            }
            else if (sendPacketResult > 0)
            {
                sp_video_stream_release_packet(packet);
                continue;
            }

            hasVideoFrame = true;
        }

        sp_video_stream_release_packet(packet);
    }

    return true;
}

sp_bool sp_video_stream_next_frame(SpVideoStream* stream, sp_byte* data)
{
    AVFrame* frame = av_frame_alloc();

    sp_int ret = avcodec_receive_frame(stream->videoCodecContext, frame);

    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
    {
        //std::cout << "avcodec_receive_frame: " << ret << std::endl;
        sp_video_stream_release_frame(frame);
        return false;
    }
    else if (ret < 0)
    {
        // erro durante decode
        sp_video_stream_release_frame(frame);
        return true;
    }
    else if (ret > 0)
    {
        // erro durante decode
        sp_video_stream_release_frame(frame);
        return true;
    }

    SwsContext* conversor = sws_getContext(
        frame->width, frame->height, stream->videoCodecContext->pix_fmt,
        stream->outputProperties->width, stream->outputProperties->height, stream->outputProperties->format,
        SWS_POINT, NULL, NULL, NULL
    );

    sp_int dataStride = stream->outputProperties->channels * stream->outputProperties->width;

    sws_scale(conversor, frame->data, frame->linesize, 0, frame->height, &data, &dataStride);

    sws_freeContext(conversor);

    sp_video_stream_release_frame(frame);

    return true;
}

void sp_video_stream_close(SpVideoStream* stream)
{
    avformat_close_input(&stream->streamContext);

    if (stream->videoCodecContext != nullptr)
    {
        avcodec_free_context(&stream->videoCodecContext);
    }
}

void sp_video_stream_release(SpVideoStream* stream)
{
    if (stream->streamContext != nullptr)
    {
        avformat_free_context(stream->streamContext);
    }

    std::free(stream);
    stream = NULL;
}