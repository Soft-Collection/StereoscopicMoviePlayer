// VideoPlayerDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "../VideoPlayer/CFFMpegPlayer.h"
#include "../VideoPlayer/CFFMpegPlayer.cpp"
#include "../VideoPlayer/CWavePlaying.h"
#include "../VideoPlayer/CWavePlaying.cpp"

int vidcnt = 0;
int aucnt = 0;
CWavePlaying* wave = NULL;

void OnNewVideoFrame(void* user, AVFrame* frame)
{
    std::cout << "Video: " << frame->pts << std::endl;
}

void OnNewAudioFrame(void* user, AVFrame* frame)
{
    int bitsPerSample = 0;
    switch (frame->format)
    {
    case AV_SAMPLE_FMT_U8:
        bitsPerSample = 8;
        break;
    case AV_SAMPLE_FMT_S16:
        bitsPerSample = 16;
        break;
    case AV_SAMPLE_FMT_S32:
        bitsPerSample = 32;
        break;
    case AV_SAMPLE_FMT_FLT:
        bitsPerSample = 32;
        break;
    case AV_SAMPLE_FMT_DBL:
        bitsPerSample = 64;
        break;
    case AV_SAMPLE_FMT_U8P:
        bitsPerSample = 8;
        break;
    case AV_SAMPLE_FMT_S16P:
        bitsPerSample = 16;
        break;
    case AV_SAMPLE_FMT_S32P:
        bitsPerSample = 32;
        break;
    case AV_SAMPLE_FMT_FLTP:
        bitsPerSample = 32;
        break;
    case AV_SAMPLE_FMT_DBLP:
        bitsPerSample = 64;
        break;
    }
    if (wave == NULL)
    {
        wave = new CWavePlaying(10, 100000, frame->sample_rate, bitsPerSample, frame->ch_layout.nb_channels);
        wave->Open();
    }
    if (wave != NULL)
    {
        wave->Play((char*)frame->data[0], 0, frame->nb_samples * (bitsPerSample / 8));
    }
    std::cout << "Audio: " << frame->pts << std::endl;
}

int main()
{
    CFFMpegPlayer* player = new CFFMpegPlayer(NULL, OnNewVideoFrame, OnNewAudioFrame);
    player->Open(L"C:\\Users\\A\\Desktop\\Avatar.1080p.SBS.FULLHD3D\\ancient-avatar.3d.1080p.x264.mkv");
    INT64 xxx = player->GetDuration();
    //player->Seek(xxx / 2);
    player->Seek(0);
    player->SetAudioTrack(1);
    player->Play();
    Sleep(2000);
    player->GetCurrentPlayingTime();
    player->SetAudioTrack(0);
    Sleep(2000);
    player->SetAudioTrack(1);
    Sleep(2000);
    player->Pause();
    Sleep(2000);
    player->Play();
    while (true)
    {
    }
}


//if (sampleConversionData.ParamsChanged)
//{
//	mWavePlaying = new CWavePlaying(10, 10000, sampleConversionData.SamplesPerSec, sampleConversionData.BitsPerSample, sampleConversionData.Channels);
//	mWavePlaying->Open();
//}
//mWavePlaying->Play((char*)*sampleConversionData.Data, 0, (ULONG)sampleConversionData.LineSize[0] / (sampleConversionData.BitsPerSample / 8));


/*
    // Assume you already have the AVFrame *frame with 6-channel audio (after decode)
    // and you want stereo output in S16 (int16_t) interleaved
    struct SwrContext* swr_ctx = swr_alloc_set_opts(
        NULL,
        // Output (stereo, S16)
        av_get_default_channel_layout(2),      // out_ch_layout
        AV_SAMPLE_FMT_S16,                     // out_sample_fmt
        samplesPerSec,                         // out_sample_rate
        // Input (whatever the frame has)
        Channels ? Channels : av_get_default_channel_layout(Channels), // in_ch_layout
        AV_SAMPLE_FMT_S16,                     // in_sample_fmt
        samplesPerSec,                         // in_sample_rate
        0, NULL);
    // Check for allocation error!
    swr_init(swr_ctx);
    // Prepare output buffer
    uint8_t* out_data;
    int out_linesize;
    int nb_out_ch = 2;
    int nb_samples = (int)av_rescale_rnd(swr_get_delay(swr_ctx, samplesPerSec) + bitsPerSample / 8, samplesPerSec, samplesPerSec, AV_ROUND_UP);
    av_samples_alloc(&out_data, &out_linesize, nb_out_ch, nb_samples, AV_SAMPLE_FMT_S16, 0);
    // Do the conversion
    int out_samples = swr_convert(swr_ctx, &out_data, nb_samples, (const uint8_t**)frameData, bitsPerSample / 8);
    // Use out_data (interleaved stereo int16_t)
    swr_free(&swr_ctx);
    av_freep(&out_data);
*/







// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
