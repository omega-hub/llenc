#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <omega.h>
#include "NvIFR/NvIFR_API.h"


namespace llenc
{
    using namespace omega;

    ///////////////////////////////////////////////////////////////////////////
    class Encoder
    {
    public:
        Encoder();

        bool initialize(int width, int height, int fps = 30, int quality = 100);
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
