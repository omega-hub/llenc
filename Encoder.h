#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <omega.h>

#ifdef OMEGA_OS_WINDOWS
    #include "GRID-2.3/NvIFR_API.h"
#elif defined OMEGA_OS_LINUX
    #include "GRID-2.2/NvIFR_API.h"
#else
    #error "llenc uses the NVIDIA GRID SDK, supported only on Windows and Linux."
#endif


namespace llenc
{
    using namespace omega;

    ///////////////////////////////////////////////////////////////////////////
    class Encoder
    {
    public:
        Encoder();

        bool initialize(int width, int height, int fps = 60, int quality = 100);
        void shutdown();

        bool encodeFrame(RenderTarget* rt);
        bool dataAvailable();
        bool lockBitstream(const void** stptr, uint32_t* bytes);
        void unlockBitstream();

    private:
        NvIFRAPI myNVIFR;
        NV_IFROGL_SESSION_HANDLE mySession;
        NV_IFROGL_TRANSFEROBJECT_HANDLE myTransferObject;
        
        ThreadEvent myDataReleasedEvent;
        ThreadEvent myTransferStartedEvent;

        int myMaxOutstandingTransfers;
        int myReceivedTransfers;
        int myTransferCounter;
    };
};

#endif
