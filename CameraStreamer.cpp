#include <omegaGl.h>

#include "CameraStreamer.h"
#include "Encoder.h"

using namespace llenc;

///////////////////////////////////////////////////////////////////////////////
CameraStreamer::CameraStreamer():
    myEncoder(NULL),
    myTargetFps(20)
{
    myTimer.start();
    myLastFrameTime = 0;
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
    Vector2i size = context.tile->pixelSize;
    
    oflog(Verbose, "[CameraStreamer::initialize] tile pixel size: <%1%>", %size);

    myRenderTarget = r->createRenderTarget(RenderTarget::RenderToTexture);
    myRenderTexture = r->createTexture();
    myRenderTexture->initialize(size[0], size[1], Texture::TypeRectangle);
    myDepthTexture = r->createTexture();
    myDepthTexture->initialize(size[0], size[1], Texture::TypeRectangle, Texture::ChannelDepth, Texture::FormatFloat);
    myRenderTarget->setTextureTarget(myRenderTexture, myDepthTexture);

    Encoder* e = new Encoder();
    e->initialize(size[0], size[1]);
    myEncoder = e;
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::reset(Camera* c, const DrawContext& context)
{
    Renderer* r = context.renderer;
    Vector2i size = context.tile->pixelSize;
    
    oflog(Verbose, "[CameraStreamer::reset] tile pixel size: <%1%>", %size);
    myRenderTexture->resize(size[0], size[1]);
    myDepthTexture->resize(size[0], size[1]);
    
    // Recreate the encoder.
    myEncoder->shutdown();
    delete myEncoder;
    Encoder* e = new Encoder();
    e->initialize(size[0], size[1]);
    myEncoder = e;
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::beginDraw(Camera* cam, DrawContext& context)
{
    if(myRenderTarget == NULL) initialize(cam, context);
    
    // If the output tile size changed, reset the encoder.
    if(context.tile->pixelSize[0] != myRenderTarget->getWidth() ||
        context.tile->pixelSize[1] != myRenderTarget->getHeight())
    {
        reset(cam, context);
    }

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
    if(myRenderTarget != NULL) myRenderTarget->clear();
}

///////////////////////////////////////////////////////////////////////////////
void CameraStreamer::finishFrame(Camera* cam, const FrameInfo& frame)
{
    // If enough time has passed since the last frime, send a new one
    // to the encoder.
    if(myTimer.getElapsedTimeInMilliSec() - myLastFrameTime > (1000.0f / myTargetFps))
    {
        myEncoder->encodeFrame(myRenderTarget);
        myLastFrameTime = myTimer.getElapsedTimeInMilliSec();
    }
}
