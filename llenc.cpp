#include <omega.h>
#include <omegaToolkit.h>
#include <omegaGl.h>

#ifdef OMEGA_OS_WIN
    #include "GRID-2.3/NvIFR_API.h"
    #define DLL_EXPORT extern "C" __declspec(dllexport)
#elif defined OMEGA_OS_LINUX
    #include "GRID-2.2/NvIFR_API.h"
    #define DLL_EXPORT extern "C"
#else
    #error "llenc uses the NVIDIA GRID SDK, supported only on Windows and Linux."
#endif

using namespace omega;
using namespace omegaToolkit;

// NVIFR API object
bool sNVIFRInitialized = false;
NvIFRAPI NVIFR;

////////////////////////////////////////////////////////////////////////////////
// NVIDIA GRID Encoder class.
class Encoder: public IEncoder
{
public:
    Encoder();

    bool initialize();
    bool configure(int width, int height, int fps = 30, int quality = 100);
    void shutdown();

    bool encodeFrame(RenderTarget* rt);
    bool dataAvailable();
    bool lockBitstream(const void** stptr, uint32_t* bytes);
    void unlockBitstream();

private:
    NV_IFROGL_SESSION_HANDLE mySession;
    bool myTransferObjectInitialized;
    NV_IFROGL_TRANSFEROBJECT_HANDLE myTransferObject;
    
    ThreadEvent myDataReleasedEvent;
    ThreadEvent myTransferStartedEvent;

    int myMaxOutstandingTransfers;
    int myReceivedTransfers;
    int myTransferCounter;
};

////////////////////////////////////////////////////////////////////////////////
// Library entry point.
DLL_EXPORT IEncoder* createEncoder() 
{ return new Encoder(); }
    
////////////////////////////////////////////////////////////////////////////////
Encoder::Encoder()
{
    myTransferObjectInitialized = false;
    myMaxOutstandingTransfers = 4;
    myReceivedTransfers = 0;
    myTransferCounter = 0;

}

////////////////////////////////////////////////////////////////////////////////
void ifrcb(NV_IFROGL_DEBUG_SEVERITY s, const char* message, void* usr)
{
    omsg(message);
}

////////////////////////////////////////////////////////////////////////////////
bool Encoder::initialize()
{
    if(!sNVIFRInitialized)
    {
        olog(Debug, "myNVIFR.initialize");
        if(!NVIFR.initialize())
        {
            owarn("Encoder::initialize: IVIFR API init failed.");
            return false;
        }
        olog(Debug, "myNVIFR.nvIFROGLDebugMessageCallback");
        //myNVIFR.nvIFROGLDebugMessageCallback(ifrcb, NULL);
        
        sNVIFRInitialized = true;
    }
    
    olog(Debug, "myNVIFR.nvIFROGLCreateSession");
    NVIFROGLSTATUS st = NVIFR.nvIFROGLCreateSession(&mySession, NULL);
    if(st != NV_IFROGL_SUCCESS)
    {
        owarn("Encoder::initialize: IFR session init failed.");
        return false;
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Encoder::configure(int width, int height, int fps, int quality)
{
    olog(Debug, "[Encoder::initialize]");
    
    if(myTransferObjectInitialized)
    {
        NVIFR.nvIFROGLDestroyTransferObject(myTransferObject);
        myTransferCounter = 0;
        myReceivedTransfers = 0;
    }
    
    NV_IFROGL_H264_ENC_CONFIG config;
    memset(&config, 0, sizeof(config));

    // 66 = Baseline profile - this is the only supported profile for the
    // web-based decoder.
    config.profile = 66;
    config.frameRateNum = 20;
    config.frameRateDen = 1;
    config.width = width;
    config.height = height;
    config.avgBitRate = width * height * 5.69;
    config.peakBitRate = width * height * 5.69 * 8;
    config.GOPLength = 75;
    config.rateControl = NV_IFROGL_H264_RATE_CONTROL_VBR;
    config.stereoFormat = NV_IFROGL_H264_STEREO_NONE;
    config.preset = NV_IFROGL_H264_PRESET_LOW_LATENCY_HP;
    config.VBVBufferSize = config.avgBitRate / 20;
    config.VBVInitialDelay = config.avgBitRate / 20;
    
    olog(Debug, "myNVIFR.nvIFROGLCreateTransferToH264EncObject");
    if(NVIFR.nvIFROGLCreateTransferToH264EncObject(mySession, &config, &myTransferObject) != NV_IFROGL_SUCCESS)
    {
        owarn("Encoder::initialize: H264 encoder init failed.");
        return false;
    }

    olog(Verbose, "llenc Encoder initialized");
    myTransferObjectInitialized = true;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Encoder::shutdown()
{
    olog(Verbose, "llenc Encoder shutdown");
    
    NVIFR.nvIFROGLDestroyTransferObject(myTransferObject);
    NVIFR.nvIFROGLDestroySession(mySession);
}

////////////////////////////////////////////////////////////////////////////////
bool Encoder::encodeFrame(RenderTarget* source)
{
    oassert(source != NULL);

    while(myTransferCounter - myReceivedTransfers > myMaxOutstandingTransfers)
    {
        myDataReleasedEvent.wait();
    }

    if(NVIFR.nvIFROGLTransferFramebufferToH264Enc(
        myTransferObject,
        NULL,
        //0,
        //GL_FRONT_LEFT,
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

////////////////////////////////////////////////////////////////////////////////
bool Encoder::dataAvailable()
{
    //ofmsg("TransferPending %1%    Received %2%", %myTransferCounter %myReceivedTransfers);   
    return (myTransferCounter > myReceivedTransfers);
}

////////////////////////////////////////////////////////////////////////////////
bool Encoder::lockBitstream(const void** stptr, uint32_t* bytes)
{
    uintptr_t datasize;

    while(myTransferCounter == myReceivedTransfers) myTransferStartedEvent.wait();
    //ofmsg("lock %1%      %2%", %myReceivedTransfers %myTransferCounter);
    if(NVIFR.nvIFROGLLockTransferData(
        myTransferObject, &datasize, stptr) != NV_IFROGL_SUCCESS) return false;

    *bytes = datasize;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Encoder::unlockBitstream()
{
    NVIFR.nvIFROGLReleaseTransferData(myTransferObject);
    myReceivedTransfers++;
    myDataReleasedEvent.signal();
}
