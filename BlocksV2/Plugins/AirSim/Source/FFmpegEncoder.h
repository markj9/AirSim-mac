#pragma once

#include "CoreMinimal.h"

// Include FFmpeg headers inside extern "C" to avoid C++ name mangling.
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}


/**
 * A simple FFmpeg encoder class that encodes raw BGRA frames into H.264/H.265.
 */
class FFmpegEncoder
{
public:
    FFmpegEncoder();
    ~FFmpegEncoder();

    /**
     * Initializes the encoder.
     *
     * @param InWidth      The width of the frame.
     * @param InHeight     The height of the frame.
     * @param InFps        Frames per second.
     * @param InBitRate    Target bitrate in bits per second.
     * @param CodecID      Codec to use (e.g., AV_CODEC_ID_H264 or AV_CODEC_ID_HEVC).
     * @return true if initialization was successful.
     */
    bool InitializeEncoder(int32 InWidth, int32 InHeight, int32 InFps, int32 InBitRate, AVCodecID CodecID);

    /**
     * Encodes a single frame.
     *
     * @param InRawData  Pointer to the raw BGRA bitmap data.
     * @param InDataSize Size of the raw data in bytes.
     * @param state metadata which is passed to the callback handler
     * @return true if the frame was successfully processed.
     */
    bool EncodeFrame(const uint8* InRawData, int32 InDataSize, const void* state = nullptr);

    /**
     * Flushes the encoder by sending a null frame so that all pending packets are processed.
     */
    void FlushEncoder();

    // Callback function that will be called when an encoded packet is ready.
    TFunction<void(const uint8* /*PacketData*/, int32 /*PacketSize*/, const void* state)> OnPacketEncodedCallback;

    
private:
    // FFmpeg structures.
    AVCodecContext* CodecCtx;
    const AVCodec*        Codec;
    AVFrame*        Frame;
    AVPacket*       Packet;
    SwsContext*     SwsCtx;

    // Encoder parameters.
    int32 Width;
    int32 Height;
    int32 Fps;
    int32 FrameIndex;
};
