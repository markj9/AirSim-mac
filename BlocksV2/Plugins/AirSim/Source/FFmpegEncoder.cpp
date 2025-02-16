#include "FFmpegEncoder.h"
#include "Logging/LogMacros.h"



FFmpegEncoder::FFmpegEncoder()
    : CodecCtx(nullptr)
    , Codec(nullptr)
    , Frame(nullptr)
    , Packet(nullptr)
    , SwsCtx(nullptr)
    , Width(0)
    , Height(0)
    , Fps(0)
    , FrameIndex(0)
{
    // No need to call avcodec_register_all() in FFmpeg 4.0 and later.
}

FFmpegEncoder::~FFmpegEncoder()
{
    // Free the scaling context.
    if (SwsCtx)
    {
        sws_freeContext(SwsCtx);
        SwsCtx = nullptr;
    }

    // Free the frame.
    if (Frame)
    {
        av_frame_free(&Frame);
        Frame = nullptr;
    }

    // Free the codec context.
    if (CodecCtx)
    {
        avcodec_free_context(&CodecCtx);
        CodecCtx = nullptr;
    }

    // Free the packet.
    if (Packet)
    {
        av_packet_free(&Packet);
        Packet = nullptr;
    }
}

bool FFmpegEncoder::InitializeEncoder(int32 InWidth, int32 InHeight, int32 InFps, int32 InBitRate, AVCodecID CodecID)
{
    Width = InWidth;
    Height = InHeight;
    Fps = InFps;
    FrameIndex = 0;

    // Find the encoder for the given codec (H.264 or H.265).
    Codec = avcodec_find_encoder(CodecID);
    if (!Codec)
    {
        UE_LOG(LogTemp, Error, TEXT("Encoder not found for codec ID %d"), CodecID);
        return false;
    }

    // Allocate the codec context.
    CodecCtx = avcodec_alloc_context3(Codec);
    if (!CodecCtx)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not allocate codec context"));
        return false;
    }

    // Set codec parameters.
    CodecCtx->width     = Width;
    CodecCtx->height    = Height;
    CodecCtx->time_base = AVRational{1, Fps};
    CodecCtx->framerate = AVRational{Fps, 1};
    CodecCtx->bit_rate  = InBitRate;
    CodecCtx->pix_fmt   = AV_PIX_FMT_YUV420P;

    // Open the codec.
    if (avcodec_open2(CodecCtx, Codec, nullptr) < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not open codec"));
        return false;
    }

    // Allocate a frame for encoding.
    Frame = av_frame_alloc();
    if (!Frame)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not allocate frame"));
        return false;
    }
    Frame->format = CodecCtx->pix_fmt;
    Frame->width  = CodecCtx->width;
    Frame->height = CodecCtx->height;

    // Allocate the frame buffer with a 32-byte alignment.
    if (av_frame_get_buffer(Frame, 32) < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not allocate frame buffer"));
        return false;
    }

    // Allocate an AVPacket using the new API.
    Packet = av_packet_alloc();
    if (!Packet)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not allocate AVPacket"));
        return false;
    }

    // Create a scaling context to convert from BGRA (raw data) to YUV420P (encoder input).
    // Assumes input raw data is in BGRA format.
    SwsCtx = sws_getContext(Width, Height, AV_PIX_FMT_BGRA,
                            Width, Height, CodecCtx->pix_fmt,
                            SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!SwsCtx)
    {
        UE_LOG(LogTemp, Error, TEXT("Could not initialize the conversion context"));
        return false;
    }

    // Get default coefficient tables for a common colorspace; here we use ITU-R BT.709.
    const int* inv_table = sws_getCoefficients(SWS_CS_ITU709);
    const int* table     = sws_getCoefficients(SWS_CS_ITU709);

    // Choose the input and output range:
    //  - Use 1 for MPEG (limited) range or 0 for full range. 
    //    (Most video encoders expect MPEG limited range; adjust if your source is full range.)
    int srcRange = 1;  
    int dstRange = 1;

    // Set brightness, contrast, and saturation adjustments.
    // Contrast and saturation are given in Q16 fixed-point format.
    // 1.0 in Q16 is (1 << 16) i.e. 65536.
    // If you don't want to change these values, use 0 for brightness and 1<<16 for contrast and saturation.
    int brightness = 0;          // No brightness offset.
    int contrast   = 1 << 16;      // 1.0 in fixed point.
    int saturation = 1 << 16;      // 1.0 in fixed point.

    // Set the colorspace details on the swscale context.
    int ret = sws_setColorspaceDetails(SwsCtx, inv_table, srcRange, table, dstRange, brightness, contrast, saturation);
    
    return true;
}

bool FFmpegEncoder::EncodeFrame(const uint8* InRawData, int32 InDataSize, const void* state)
{
    if (!InRawData || InDataSize <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid raw data provided for encoding"));
        return false;
    }

    // Prepare the source data pointers and stride for sws_scale.
    const uint8_t* SrcSlice[1] = { InRawData };
    int SrcStride[1] = { Width * 4 }; // BGRA: 4 bytes per pixel

    // Convert the BGRA raw data into YUV420P and store it in Frame.
    int ScaledHeight = sws_scale(SwsCtx, SrcSlice, SrcStride, 0, Height,
                                 Frame->data, Frame->linesize);
    if (ScaledHeight != Height)
    {
        UE_LOG(LogTemp, Warning, TEXT("sws_scale processed only %d of %d lines"), ScaledHeight, Height);
    }

    // Set the presentation timestamp (PTS).
    Frame->pts = FrameIndex++;

    // Send the frame for encoding.
    int ret = avcodec_send_frame(CodecCtx, Frame);
    if (ret < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Error sending frame to encoder: %d"), ret);
        return false;
    }

    // Retrieve and process encoded packets.
    while (true)
    {

        ret = avcodec_receive_packet(CodecCtx, Packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        else if (ret < 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Error during encoding: %d"), ret);
            return false;
        }

        // Now Packet contains the encoded data.
        //UE_LOG(LogTemp, Log, TEXT("Encoded packet size: %d"), Packet->size);
 
        // Here you can send Packet->data and Packet->size over UDP or write to a file.
        // For example: SendPacketOverUDP(Packet->data, Packet->size);
        
        // Invoke the callback if set.
        if (OnPacketEncodedCallback)
        {
            OnPacketEncodedCallback(Packet->data, Packet->size, state);
        }
        
        // Unreference the packet to prepare for the next encoding cycle.
        av_packet_unref(Packet);
    }

    return true;
}

void FFmpegEncoder::FlushEncoder()
{
    // Flush the encoder by sending a null frame.
    int ret = avcodec_send_frame(CodecCtx, nullptr);
    if (ret < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Error sending flush frame to encoder: %d"), ret);
        return;
    }

    // Retrieve remaining packets.
    while (true)
    {
        ret = avcodec_receive_packet(CodecCtx, Packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        else if (ret < 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Error during flushing encoding: %d"), ret);
            break;
        }
        UE_LOG(LogTemp, Log, TEXT("Flushed packet size: %d"), Packet->size);

        // Process the flushed packet as needed.

        // Unreference the packet after processing.
        av_packet_unref(Packet);
    }
}