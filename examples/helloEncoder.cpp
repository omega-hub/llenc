/******************************************************************************
* THE OMEGA LIB PROJECT
*-----------------------------------------------------------------------------
* Copyright 2010-2015		Electronic Visualization Laboratory,
*							University of Illinois at Chicago
* Authors:
*  Alessandro Febretti		febret@gmail.com
*-----------------------------------------------------------------------------
* Copyright (c) 2010-2015, Electronic Visualization Laboratory,
* University of Illinois at Chicago
* All rights reserved.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer. Redistributions in binary
* form must reproduce the above copyright notice, this list of conditions and
* the following disclaimer in the documentation and/or other materials provided
* with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*-----------------------------------------------------------------------------
* What's in this file:
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
class HelloRenderPass: public RenderPass
{
public:
    HelloRenderPass(Renderer* client, HelloApplication* app) : 
        myApplication(app),
        RenderPass(client, "HelloRenderPass") 
    {}
    virtual void initialize();
    virtual void render(Renderer* client, const DrawContext& context);

private:
    Ref<Cube> myCube;
    HelloApplication* myApplication;
};

///////////////////////////////////////////////////////////////////////////////
class HelloApplication : public EngineModule
{
    friend class HelloRenderPass;
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
        // Open stream
        output = fopen("out.h264", "wb");

        myStreamer = new CameraStreamer();
        getEngine()->getDefaultCamera()->addListener(myStreamer);
    }

    virtual void dispose()
    {
        fclose(output);
    }

    virtual void update(const UpdateContext& uc)
    {
        Encoder* e = myStreamer->getEncoder();
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

    FILE* output;
    Ref<CameraStreamer> myStreamer;
};

///////////////////////////////////////////////////////////////////////////////
void HelloRenderPass::initialize()
{
    RenderPass::initialize();
    myCube = new Cube(0.2f);
}

///////////////////////////////////////////////////////////////////////////////
void HelloRenderPass::render(Renderer* client, const DrawContext& context)
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

///////////////////////////////////////////////////////////////////////////////
// ApplicationBase entry point
int main(int argc, char** argv)
{
    Application<HelloApplication> app("helloEncoder");
    return omain(app, argc, argv);
}
