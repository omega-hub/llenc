#include <omegaGl.h>

#include "Encoder.h"


using namespace llenc;

///////////////////////////////////////////////////////////////////////////////
Encoder::Encoder()
{
    myMaxOutstandingTransfers = 4;
    myReceivedTransfers = 0;
    myTransferCounter = 0;

}

///////////////////////////////////////////////////////////////////////////////
bool Encoder::initialize(int width, int height, int fps, int quality)
{
    NV_IFROGL_H264_ENC_CONFIG config;
    memset(&config, 0, sizeof(config));

    config.profile = quality;
    config.frameRateNum = fps;
    config.frameRateDen = 1;
    config.width = width;
    config.height = height;
    config.avgBitRate = width * height * 5.69;
    config.GOPLength = 75;
    config.rateControl = NV_IFROGL_H264_RATE_CONTROL_CBR;
    config.stereoFormat = NV_IFROGL_H264_STEREO_NONE;
    config.preset = NV_IFROGL_H264_PRESET_LOW_LATENCY_HP;

    if(!myNVIFR.initialize())
    {
        owarn("Encoder::initialize: IVIFR API init failed.");
        return false;
    }

    NVIFROGLSTATUS st = myNVIFR.nvIFROGLCreateSession(&mySession, NULL);
    if(st != NV_IFROGL_SUCCESS)
    {
        owarn("Encoder::initialize: IFR session init failed.");
        return false;
    }

    if(myNVIFR.nvIFROGLCreateTransferToH264EncObject(mySession, &config, &myTransferObject) != NV_IFROGL_SUCCESS)
    {
        owarn("Encoder::initialize: IFR session init failed.");
    }
    omsg("Encoder initialized");

    return true;
}

///////////////////////////////////////////////////////////////////////////////
void Encoder::shutdown()
{
    myNVIFR.nvIFROGLDestroyTransferObject(myTransferObject);
    myNVIFR.nvIFROGLDestroySession(mySession);

}

///////////////////////////////////////////////////////////////////////////////
bool Encoder::encodeFrame(RenderTarget* source)
{
    oassert(source != NULL);

    while(myTransferCounter - myReceivedTransfers > myMaxOutstandingTransfers)
    {
        myDataReleasedEvent.wait();
    }

    if(myNVIFR.nvIFROGLTransferFramebufferToH264Enc(
        myTransferObject,
        NULL,
        source->getId(),
        GL_COLOR_ATTACHMENT0,
        GL_NONE
        ) != NV_IFROGL_SUCCESS)
    {
        owarn("Encoder::encodeFrame: failed to transfer data to encoder");

        return false;
    }

    myTransferStartedEvent.signal();
    myTransferCounter++;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
bool Encoder::dataAvailable()
{
    return (myTransferCounter > myReceivedTransfers);
}

///////////////////////////////////////////////////////////////////////////////
bool Encoder::lockBitstream(const void** stptr, uint32_t* bytes)
{
    uintptr_t datasize;

    while(myTransferCounter == myReceivedTransfers) myTransferStartedEvent.wait();

    if(myNVIFR.nvIFROGLLockTransferData(
        myTransferObject, &datasize, stptr) != NV_IFROGL_SUCCESS) return false;

    *bytes = datasize;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
void Encoder::unlockBitstream()
{
    myNVIFR.nvIFROGLReleaseTransferData(myTransferObject);
    myReceivedTransfers++;
    myDataReleasedEvent.signal();
}
