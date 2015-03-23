/*!
 * \file
 * A helper class to access the NvIFR API.
 *
 * \copyright    
 * Copyright 2013-2014 NVIDIA Corporation.  All rights reserved.    
 * \n\n    
 * Please refer to the NVIDIA end user license agreement (EULA) associated    
 * with this source code for terms and conditions that govern your use of    
 * this software. Any use, reproduction, disclosure, or distribution of    
 * this software and related documentation outside the terms of the EULA    
 * is strictly prohibited. 
 */

#ifndef NVIFR_API_H
#define NVIFR_API_H

#include <string.h>
#include "NvIFR/NvIFROpenGL.h"

/*!
 * This class can be used to access the NvIFR API. To use this define a
 * variable e.g. 'static NvIFRAPI nvIFR;' call 'nvIFR.initialize()' and
 * call the API entry points through this class, e.g. 'nvIFR.nvIFROGLCreateSession(...)'.
 */
class NvIFRAPI
{
public:
    //! entry points
    PNVIFROGLCREATESESSION nvIFROGLCreateSession;
    PNVIFROGLDESTROYSESSION nvIFROGLDestroySession;
    PNVIFROGLCREATETRANSFERTOSYSOBJECT nvIFROGLCreateTransferToSysObject;
    PNVIFROGLCREATETRANSFERTOH264ENCOBJECT nvIFROGLCreateTransferToH264EncObject;
    PNVIFROGLDESTROYTRANSFEROBJECT nvIFROGLDestroyTransferObject;
    PNVIFROGLTRANSFERFRAMEBUFFERTOSYS nvIFROGLTransferFramebufferToSys;
    PNVIFROGLTRANSFERFRAMEBUFFERTOH264ENC nvIFROGLTransferFramebufferToH264Enc;
    PNVIFROGLLOCKTRANSFERDATA nvIFROGLLockTransferData;
    PNVIFROGLRELEASETRANSFERDATA nvIFROGLReleaseTransferData;
    PNVIFROGLGETERROR nvIFROGLGetError;
    PNVIFROGLDEBUGMESSAGECALLBACK nvIFROGLDebugMessageCallback;

    /*!
     * Constructor.
     */
    NvIFRAPI() :
        nvIFROGLCreateSession(NULL),
        nvIFROGLDestroySession(NULL),
        nvIFROGLCreateTransferToSysObject(NULL),
        nvIFROGLCreateTransferToH264EncObject(NULL),
        nvIFROGLDestroyTransferObject(NULL),
        nvIFROGLTransferFramebufferToSys(NULL),
        nvIFROGLTransferFramebufferToH264Enc(NULL),
        nvIFROGLLockTransferData(NULL),
        nvIFROGLReleaseTransferData(NULL),
        nvIFROGLGetError(NULL),
        nvIFROGLDebugMessageCallback(NULL)
    {
    }

    /*!
     * Initialize the NvIFR API entry points
     *
     * /return false if failed
     */
    bool initialize()
    {
        NV_IFROGL_API_FUNCTION_LIST apiFunctionList;

        memset(&apiFunctionList, 0, sizeof(apiFunctionList));
        apiFunctionList.version = NVIFROGL_VERSION;
        if (NvIFROGLCreateInstance(&apiFunctionList) != NV_IFROGL_SUCCESS) {
            return false;
        }

        nvIFROGLCreateSession = apiFunctionList.nvIFROGLCreateSession;
        nvIFROGLDestroySession = apiFunctionList.nvIFROGLDestroySession;
        nvIFROGLCreateTransferToSysObject = apiFunctionList.nvIFROGLCreateTransferToSysObject;
        nvIFROGLCreateTransferToH264EncObject = apiFunctionList.nvIFROGLCreateTransferToH264EncObject;
        nvIFROGLDestroyTransferObject = apiFunctionList.nvIFROGLDestroyTransferObject;
        nvIFROGLTransferFramebufferToSys = apiFunctionList.nvIFROGLTransferFramebufferToSys;
        nvIFROGLTransferFramebufferToH264Enc = apiFunctionList.nvIFROGLTransferFramebufferToH264Enc;
        nvIFROGLLockTransferData = apiFunctionList.nvIFROGLLockTransferData;
        nvIFROGLReleaseTransferData = apiFunctionList.nvIFROGLReleaseTransferData;
        nvIFROGLGetError = apiFunctionList.nvIFROGLGetError;
        nvIFROGLDebugMessageCallback = apiFunctionList.nvIFROGLDebugMessageCallback;
        return true;
    }
};

#endif // NVIFR_API_H

