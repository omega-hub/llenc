#ifndef __CAMERA_STREAMER_H__
#define __CAMERA_STREAMER_H__

#include <omega.h>

namespace llenc
{
    using namespace omega;

    class Encoder;

    ///////////////////////////////////////////////////////////////////////////
    class CameraStreamer : public ICameraListener, public ReferenceType
    {
    public:
        CameraStreamer();
        ~CameraStreamer();

        void initialize(Camera* c, const DrawContext& context);
        void reset(Camera* c, const DrawContext& context);

        virtual void endDraw(Camera* cam, DrawContext& context);
        virtual void beginDraw(Camera* cam, DrawContext& context);
        virtual void startFrame(Camera* cam, const FrameInfo& frame);
        virtual void finishFrame(Camera* cam, const FrameInfo& frame);

        Encoder* getEncoder() { return myEncoder; }

        void setTargetFps(int fps) { myTargetFps = fps; }
        int getTargetFps() { return myTargetFps; }

    private:
        Encoder* myEncoder;

        Ref<RenderTarget> myRenderTarget;
        Ref<Texture> myRenderTexture;
        Ref<Texture> myDepthTexture;

        // FPS stuff
        int myTargetFps;
        Timer myTimer;
        double myLastFrameTime;
    };
};

#endif
