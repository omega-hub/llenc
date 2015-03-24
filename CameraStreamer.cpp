#include <omegaGl.h>

#include "CameraStreamer.h"
#include "Encoder.h"

using namespace llenc;

///////////////////////////////////////////////////////////////////////////////
CameraStreamer::CameraStreamer():
    myEncoder(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
CameraStreamer::~CameraStreamer()
{
    if(myEncoder != NULL)
    {
        myEncoder->shutdown();
        delete myEncoder;
        myEncoder = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::initialize(Camera* c, const DrawContext& context)
{
    Renderer* r = context.renderer;

    Vector2i& size = context.tile->pixelSize;

    myRenderTarget = r->createRenderTarget(RenderTarget::RenderToTexture);
    myRenderTexture = r->createTexture();
    myRenderTexture->initialize(size[0], size[1]);
    myDepthTexture = r->createTexture();
    myDepthTexture->initialize(size[0], size[1], GL_DEPTH_COMPONENT);
    myRenderTarget->setTextureTarget(myRenderTexture, myDepthTexture);

    myEncoder = new Encoder();
    myEncoder->initialize(size[0], size[1]);
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::beginDraw(Camera* cam, DrawContext& context)
{
    if(myRenderTarget == NULL) initialize(cam, context);

    myRenderTarget->bind();
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::endDraw(Camera* cam, DrawContext& context)
{
    myRenderTarget->unbind();
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::startFrame(Camera* cam, const FrameInfo& frame)
{
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::finishFrame(Camera* cam, const FrameInfo& frame)
{
    myEncoder->encodeFrame(myRenderTarget);
}
