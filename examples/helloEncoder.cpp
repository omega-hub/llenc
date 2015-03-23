/******************************************************************************
*	An example of encoding using the low-latency encoder module. NVIDIA GRID SDK
*   support is needed to run this example.
******************************************************************************/
#include <omega.h>
#include <omegaGl.h>

#include "cube.h"

#include "llenc/CameraStreamer.h"
#include "llenc/Encoder.h"

using namespace omega;
using namespace llenc;

class HelloApplication;

///////////////////////////////////////////////////////////////////////////////
// Reads encoded frame data and writes it to disk.
class FileOutputThread: public Thread
{
public:
    void threadProc()
    {
        // Open stream
        output = fopen("out.h264", "wb");

        while(!SystemManager::instance()->isExitRequested())
        {
            Encoder* e = streamer->getEncoder();
            if(e != NULL && e->dataAvailable())
            {
                const void* data;
                uint32_t size;
                if(e->lockBitstream(&data, &size))
                {
                    fwrite(data, 1, size, output);
                    e->unlockBitstream();
                }
            }
        }
        fclose(output);
    }

    FILE* output;
    Ref<CameraStreamer> streamer;
};

///////////////////////////////////////////////////////////////////////////////
class HelloRenderPass: public RenderPass
{
public:
    HelloRenderPass(Renderer* client, HelloApplication* app) : 
        myApplication(app),
        RenderPass(client, "HelloRenderPass") 
    {}

    virtual void initialize()
    {
        RenderPass::initialize();
        myCube = new Cube(0.2f);
    }

    virtual void render(Renderer* client, const DrawContext& context)
    {
        if(context.task == DrawContext::SceneDrawTask)
        {
            client->getRenderer()->beginDraw3D(context);

            // Enable depth testing and lighting.
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);
        
            // Setup light.
            glEnable(GL_LIGHT0);
            glEnable(GL_COLOR_MATERIAL);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, Color(1.0, 1.0, 1.0).data());
            glLightfv(GL_LIGHT0, GL_POSITION, Vector3s(0.0f, 0.0f, 1.0f).data());

            // Draw a rotating cube.
            glTranslatef(0, 2, -2); 
            glRotatef(10, 1, 0, 0);
            glRotatef((float)context.frameNum * 0.1f, 0, 1, 0);
            glRotatef((float)context.frameNum * 0.2f, 1, 0, 0);
            
            myCube->draw();

            client->getRenderer()->endDraw();
        }
    }

private:
    Ref<Cube> myCube;
    HelloApplication* myApplication;
};

///////////////////////////////////////////////////////////////////////////////
class HelloApplication : public EngineModule
{
public:
    HelloApplication(): 
        EngineModule("HelloApplication")
    {}

    virtual void initializeRenderer(Renderer* r) 
    { 
        RenderPass* rp = new HelloRenderPass(r, this);
        r->addRenderPass(rp);
    }

    virtual void initialize()
    {
        myStreamer = new CameraStreamer();
        getEngine()->getDefaultCamera()->addListener(myStreamer);

        myFileOutputThread = new FileOutputThread();
        myFileOutputThread->streamer = myStreamer;
        myFileOutputThread->start();
    }

    virtual void dispose()
    {
        myFileOutputThread->stop();
        delete myFileOutputThread;
    }

    Ref<CameraStreamer> myStreamer;
    FileOutputThread* myFileOutputThread;
};

///////////////////////////////////////////////////////////////////////////////
// ApplicationBase entry point
int main(int argc, char** argv)
{
    Application<HelloApplication> app("helloEncoder");
    return omain(app, argc, argv);
}
