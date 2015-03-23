/*!
 * \copyright
 * Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
*/

#pragma once
#define NVIFR_VERSION 0x22

#define NVIFR_MAX_REF_FRAMES 0x10
#define NVIFR_STRUCT_VERSION(typeName, ver) (NvU32)(sizeof(typeName) | ((ver)<<16) | (NVIFR_VERSION << 24))

typedef unsigned long NvU32;
typedef unsigned long long NvU64;
typedef unsigned char NvU8;

#define MAX_N_BUFFERS 3

typedef enum
{
    NVIFR_SUCCESS = 0,
    NVIFR_ERROR_GENERIC = -1,
    NVIFR_ERROR_INVALID_PTR = -2,
    NVIFR_ERROR_INVALID_PARAM = -3,
    NVIFR_ERROR_DX_DRIVER_FAILURE = -4,
    NVIFR_ERROR_H264_ENC_FAILURE = -5,
    NVIFR_ERROR_H264_ENC_NOT_INITIALIZED = -6,
    NVIFR_ERROR_FILE_NOT_FOUND = -7,
    NVIFR_ERROR_OUT_OF_MEMORY = -8,
    NVIFR_ERROR_INVALID_CALL = -9,
    NVIFR_ERROR_UNSUPPORTED_INTERFACE = -10,
    NVIFR_ERROR_NOT_SUPPORTED = -11,
    NVIFR_ERROR_UNSUPPORTED_DEVICE = -12,
    NVIFR_ERROR_NVAPI_FAILURE = -13,
    NVIFR_ERROR_INCOMPATIBLE_VERSION = -14
} NVIFRRESULT;

typedef enum
{
    NVIFR_FORMAT_ARGB      = 0,      /**< ARGB (32bit storage per pixel). */
    NVIFR_FORMAT_RGB          ,      /**< RGB (24bit storage per pixel). */
    NVIFR_FORMAT_YUV_420      ,      /**< YYYYUV, UV Subsamples by factor of 2 in hieght and width. */
    NVIFR_FORMAT_RGB_PLANAR   ,      /**< RGB planar, RRRRRRRR GGGGGGGG BBBBBBBB. */
    NVIFR_FORMAT_RESERVED     ,      /**< Reserved format, should not be used. */
    NVIFR_FORMAT_YUV_444      ,      /**< YUV planar, YYYYYYYY UUUUUUUU VVVVVVVV. */
    NVIFR_FORMAT_LAST         ,      /**< Sentinel enum value. Not to be used. */
}NVIFR_BUFFER_FORMAT;

typedef struct
{
    NvU32   dwVersion;              /**< [in]  Struct version. Set to NVIFR_CREATE_PARAMS_VER.*/
    NvU32   dwInterfaceType;        /**< [in]  NvIFR Interface to be instantiated.*/
    void*   pDevice;                /**< [in]  DirectX device to be used for capturing RenderTarget data .*/
    void*   pPrivateData;           /**< [in]  Reserved. */
    NvU32   dwPrivateDataSize;      /**< [in]  Reserved. */
    NvU32   dwNvIFRVersion;         /**< [out] Highest NvIFR API version supported by underlying NvIFR driver. */
    void*   pNvIFR;                 /**< [out] Pointer to the instantiated NvIFR object. */
    NVIFRRESULT result;             /**< [out] Status code for the NvIFR object instantiation. */
    NvU32   dwReserved[27];         /**< [in]  Reserved. Set to 0. */
    void*   pReserved[29];          /**< [in]  Reserved. Set to NULL. */
} NVIFR_CREATE_PARAMS;
#define NVIFR_CREATE_PARAMS_VER NVIFR_STRUCT_VERSION(NVIFR_CREATE_PARAMS, 1);

#if DIRECT3D_VERSION  == 0x0900
typedef void * IFRSharedSurfaceHandle;
// Shared Surface Extension for Dx9 only
typedef BOOL (__stdcall * NvIFR_CreateSharedSurfaceEXTFunctionType) (IDirect3DDevice9 * pDevice, NvU32 dwWidth, NvU32 dwHeight, IFRSharedSurfaceHandle * phIFRSharedSurface);
typedef BOOL (__stdcall * NvIFR_DestroySharedSurfaceEXTFunctionType) (IDirect3DDevice9 * pDevice, IFRSharedSurfaceHandle hIFRSharedSurface);
typedef BOOL (__stdcall * NvIFR_CopyToSharedSurfaceEXTFunctionType) (IDirect3DDevice9 * pDevice, IFRSharedSurfaceHandle hIFRSharedSurface, IDirect3DSurface9 * pSurface);
typedef BOOL (__stdcall * NvIFR_CopyFromSharedSurfaceEXTFunctionType) (IDirect3DDevice9 * pDevice, IFRSharedSurfaceHandle hIFRSharedSurface, IDirect3DSurface9 * pSurface);

#endif


// "NvIFR_Create" Dll entry point

typedef NVIFRRESULT (__stdcall * NvIFR_CreateFunctionExType) (void *pParams);


