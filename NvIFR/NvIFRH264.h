
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

#ifndef _NVIFR_H264_H_
#define _NVIFR_H264_H_

#include "NvIFR.h"
#define NVIFR_TOH264HWENCODER (0x2203)

typedef enum
{
    NVIFR_H264_STEREO_NONE               =      0,       /**< Mono capture. */
    NVIFR_H264_STEREO_COMPRESSED_SBS             ,       /**< Stereo capture, views stored as squeezed side by side */
    NVIFR_H264_STEREO_SEI_TB                     ,       /**< Stereo capture, views stored in top bottom format, with SEI messages */
    NVIFR_H264_STEREO_MVC                        ,       /**< Stereo capture, views encoded using MVC standard */
    NVIFR_H264_STEREO_FORMAT_LAST                ,       /**< Sentinel value, not to be used. */
} NVIFR_H264_STEREO_FORMAT;

typedef enum _NVIFR_H264_ENC_PARAMS_RC_MODE
{
    NVIFR_H264_ENC_PARAMS_RC_CONSTQP                = 0x0,        /**< Constant QP mode */
    NVIFR_H264_ENC_PARAMS_RC_VBR                    = 0x1,        /**< Variable bitrate mode */
    NVIFR_H264_ENC_PARAMS_RC_CBR                    = 0x2,        /**< Constant bitrate mode */
    NVIFR_H264_ENC_PARAMS_RC_2_PASS_QUALITY         = 0x8,        /**< Multi pass encoding optimized for image quality and works best with single frame VBV buffer size */
    NVIFR_H264_ENC_PARAMS_RC_2_PASS_FRAMESIZE_CAP   = 0x10,       /**< Multi pass encoding optimized for maintaining frame size and works best with single frame VBV buffer size */
    NVIFR_H264_ENC_PARAMS_RC_CBR_IFRAME_2PASS       = 0x20        /**< Constant bitrate mode using two pass only for IDR frame or I frame. */
} NVIFR_H264_ENC_PARAMS_RC_MODE;

typedef enum
{
    NVIFR_H264_ENC_PARAM_FLAG_FORCEIDR           = 0x2,           /**< Encode the current frame as an IDR picture. */
    NVIFR_H264_ENC_PARAM_FLAG_DYN_BITRATE_CHANGE = 0x20,          /**< Indicates change in bitrate from current frame onwards */
} NVIFR_H264_ENC_PARAM_FLAGS;

typedef enum
{
    NVIFR_H264_PRESET_LOW_LATENCY_HP       = 0,                   /**< Use for Fastest encode, with suboptimal quality */
    NVIFR_H264_PRESET_RESERVED                ,                   /**< Reserved. Do not use. */
    NVIFR_H264_PRESET_LOW_LATENCY_HQ          ,                   /**< Use for better overall quality, compromising on encoding speed. */
    NVIFR_H264_PRESET_LOW_LATENCY_DEFAULT     ,                   /**< Use for better quality than NVIFR_H264_PRESET_LOW_LATENCY_HP and higher speed than NVIFR_H264_PRESET_LOW_LATENCY_HQ. */
    NVIFR_H264_PRESET_RESERVED2               ,                   /**< Do not use. */
    NVIFR_H264_PRESET_LOSSLESS_HP             ,                   /**< Use for lossless encoding at higher performance. */
    NVIFR_H264_PRESET_LAST                    ,                   /**< Sentinel value. Do not use. */
} NVIFR_H264_PRESET;

typedef enum
{
    NVIFR_H264_ENC_SLICING_MODE_DISABLED            = 0,          /**< Picture will be encoded as one slice. */
    NVIFR_H264_ENC_SLICING_MODE_FIXED_NUM_MBS          ,          /**< Picture will be divided into slices of n MBs, where n = dwSlicingModeParam. */
    NVIFR_H264_ENC_SLICING_MODE_FIXED_NUM_BYTES        ,          /**< Picture will be divided into slices of n Bytes, where n = dwSlicingModeParam. */
    NVIFR_H264_ENC_SLICING_MODE_FIXED_NUM_MB_ROWS      ,          /**< Picture will be divided into slices of n rows of MBs, where n = dwSlicingModeParam. */
    NVIFR_H264_ENC_SLICING_MODE_FIXED_NUM_SLICES       ,          /**< Picture will be divided into n+1 slices, where n = dwSlicingModeParam. */
    NVIFR_H264_ENC_SLICING_MODE_LAST                   ,          /**< Sentine value. Do not use. */
} NVIFR_H264_ENC_SLICING_MODE;

typedef struct 
{
    NvU32                               dwVersion;      /**< [in]: Set to NVIFR_H264HWENCODER_CONFIG_VER. */
    NvU32                               dwProfile;      /**< [in]: Encoding Profile */
    NvU32                               dwFrameRateNum; /**< [in]: Video framerate numerator */
    NvU32                               dwFrameRateDen; /**< [in]: Video framerate denominator */
    NvU32                               dwAvgBitRate;   /**< [in]: Target average itrate for encoder */
    NvU32                               dwPeakBitRate;  /**< [in]: Maximum bitrate that the encoder can hit in VBR\Const qP mode */
    NvU32                               dwGOPLength;    /**< [in]: Number frames in a GOP, same as distance between consecutive I-Frames. */
    NvU32                               dwQP;           /**< [in]: Initial QP in case of CBR\VBR. Target QP in case of Const QP mode */
    NvU32                               maxRCQP[3];                    /**< [in]: maxRCQP[0] , maxRCQP[1] , maxRCQP[2] are the max QP for P , B and I frame respectively. */
    NvU32                               bUseMaxRCQP             :1;    /**< [in]: Set this to 1 , if client want to specify maxRCQP[] */
    NvU32                               bRepeatSPSPPSHeader     :1;    /**< [in]: Fill SPS PPS header for every IDR frame*/
    NvU32                               bEnableIntraRefresh     :1;    /**< [in]: Set to 1, to enable gradual decoder refresh or intra-refresh. If the GOP structure uses B frames this will be ignored. */
    NvU32                               bEnableAQ               :1;    /**< [in]: Set to 1 if client wants to enable adaptive quantization. */
    NvU32                               bEnableYUV444Encoding   :1;    /**< [in]: Set this to 1 to enable YUV444 encoding. */
    NvU32                               bReserved               :27;   /**< [in]: Reserved. Set to 0. */
    NVIFR_H264_ENC_SLICING_MODE         dwSlicingMode;                 /**< [in]: Refer to enum NVIFR_H264_ENC_SLICING_MODE for usage. */
    NvU32                               dwSlicingModeParam;            /**< [in]: Refer to enum NVIFR_H264_ENC_SLICING_MODE for usage. */
    NvU32                               dwMaxNumRefFrames;             /**< [in]: This is used to specify the DPB size used for encoding. Setting this to 0 will allow encoder to use the default DPB size.
                                                                            Low latency applications are recommended to use a large DPB size(recommended size is 16) so that it allows clients to invalidate
                                                                            corrupt frames and use older frames for reference to improve error resiliency */
    NVIFR_H264_PRESET                   ePresetConfig;                 /**< [in]: Specifies the encoding preset used for fine tuning for encoding parameters */
    NVIFR_H264_ENC_PARAMS_RC_MODE       eRateControl;                  /**< [in]: Specifies the rate control mode used for encoding */
    NvU32                               dwVBVBufferSize;               /**< [in]: Specifies VBV buffer size in bits. Overridden by NvIFR in case of NVIFR_H264_ENC_PARAMS_RC_2_PASS_QUALITY or NVIFR_H264_ENC_PARAMS_RC_2_PASS_FRAMESIZE_CAP rate control modes.*/
    NvU32                               dwVBVInitialDelay;             /**< [in]: Specifies the VBV initial delay in bits. Overridden by NvIFR in case of NVIFR_H264_ENC_PARAMS_RC_2_PASS_QUALITY or NVIFR_H264_ENC_PARAMS_RC_2_PASS_FRAMESIZE_CAP rate control modes. */
    NvU32                               dwReserved[45];                /**< [in]: Reserved. Set to 0. */
    void*                               pReserved[32];                 /**< [in]: Reserved. Set to NULL. */
} NvIFR_H264HWEncoder_Config;
#define NVIFR_H264HWENCODER_CONFIG_VER NVIFR_STRUCT_VERSION(NvIFR_H264HWEncoder_Config, 2)

typedef struct
{
    NvU32                               dwVersion;                                     /**< [in]: Set to NVIFR_H264HWENCODER_PARAM_VER. */
    NvU32                               dwEncodeParamFlags;                            /**< [in]: Specifies bit-wise OR`ed encode param flags. See NVIFR_H264_ENC_PARAM_FLAGS enum. */
    NvU32                               dwNewAvgBitRate;                               /**< [in]: Specifies the new target average bitrate. Used to adjust bitrate on the fly */
    NvU32                               dwNewPeakBitRate;                              /**< [in]: Specifies the new maximum bitrate. Used to adjust bitrate on the fly */
    NvU32                               dwNewVBVBufferSize;                            /**< [in]: Specifies new VBV buffer size in bits. Set to one frame size for best latency. */
    NvU32                               dwNewVBVInitialDelay;                          /**< [in]: Specifies new VBV initial delay in bits. Set to one frame size for best latency. */
    NvU32                               dwNumRefFramesToInvalidate;                    /**< [in]: This specifies number of encoder references which the clients wants to invalidate*/
    NvU32                               bInvalidateRefrenceFrames :1;                  /**< [in]: Set this to 1 if client wants encoder references to be invalidated*/
    NvU32                               bStartIntraRefresh        :1;                  /**< [in]: Set this to 1 if client wants to force Intra Refresh with intra refresh period dwIntraRefreshCnt*/
    NvU32                               bReserved                 :30;                 /**< [in]: Reserved. Set to 0. */
    NvU64                               ulInvalidFrameTimeStamp[NVIFR_MAX_REF_FRAMES]; /**< [in]: This specifies an array of timestamps of the encoder references which the clients wants to invalidate*/
    NvU64                               ulCaptureTimeStamp;                            /**< [in]: This sepcifies the timestamp that the client wants to associate with current capruted frame. */
    NvU32                               dwIntraRefreshCnt;                             /**< [in]: Specifies the number of frames over which intra refresh will happen, this value will override dwIntraRefreshCnt set previously */
    NvU32                               dwReserved[51];                                /**< [in]: Reserved. Set to 0. */
    void*                               pReserved[32];                                 /**< [in]: Reserved. Set to NULL */
} NvIFR_H264HWEncoder_EncodeParams;
#define NVIFR_H264HWENCODER_PARAM_VER NVIFR_STRUCT_VERSION(NvIFR_H264HWEncoder_EncodeParams, 1)

typedef struct
{
    NvU32                               dwVersion;                                     /**< [in]: Set to NVIFR_H264HWENCODER_FRAMESTATS_VER. */
    NvU32                               dwByteSize;                                    /**< [out]: Size of the generated H.264 bitstream, in bytes. */
    NvU32                               bIsIFRame : 1;                                 /**< [out]: Set by the H.264 HW Encoder if the generated bitstream corresponds to an I frame */
    NvU32                               bReserved : 31;                                /**< [in]: Reserved. Set to 0 */
    NvU32                               dwReserved[30];                                /**< [in]: Reserved. Set to 0 */
    void*                               pReserved[32];                                 /**< [in]: Reserved. Set to 0 */
} NvIFR_H264HWEncoder_FrameStats;
#define NVIFR_H264HWENCODER_FRAMESTATS_VER NVIFR_STRUCT_VERSION(NvIFR_H264HWEncoder_FrameStats, 1)

typedef struct
{
    NvU32 dwVersion;                                                        /**< [in]: Struct version. Set to NVIFR_SETUP_H264_PARAMS_VER. */
    NVIFR_H264_STEREO_FORMAT eStreamStereoFormat;                           /**< [in]: Set to one of the NVIFR_H264_STEREO_FORMAT values. Default = NVIFR_H264_STEREO_NONE. */
    NvU32 dwTargetWidth;                                                    /**< [in]: Specifies target with to capture */
    NvU32 dwTargetHeight;                                                   /**< [in]: Specifies target height to capture */
    NvU32 dwNBuffers;                                                       /**< [in]: Specifies number of bitstream buffers to be created. */
    NvU32 dwBSMaxSize;                                                      /**< [in]: Specifies maximum bitstream buffer size in bytes. */
    NvIFR_H264HWEncoder_Config *pEncodeConfig;                              /**< [in]: Pointer to struct containing H.264 HW encoder initial configuration parameters. */
    unsigned char ** ppPageLockedBitStreamBuffers;                          /**< [out]: Container to hold the bitstream buffers allocated by NvIFR in system memory. */
    HANDLE * ppEncodeCompletionEvents;                                      /**< [out]: Container to hold the asynchronous encode completion events allocated by NvIFR. */
    NvU32 dwBitStreamOffset;                                                /**< [in]: Unsupported. Set to 0. */
    NvU32 bIsCustomYUV444;                                                  /**< [out]: Client requested for YUV444 encoding, but NvIFR needed to use a custom YUV444 representation. */
    NvU32 dwReserved[56];                                                   /**< [in]: Reserved. Set to 0. */
    void *pReserved[29];                                                    /**< [in]: Reserved. Set to NULL. */
} NVIFR_SETUP_H264_PARAMS;
#define NVIFR_SETUP_H264_PARAMS_VER NVIFR_STRUCT_VERSION(NVIFR_SETUP_H264_PARAMS, 2)

typedef struct
{
    NvU32 dwVersion;                                                        /**< [in]: Struct version. Set to NVIFR_TRANSFER_RT_TO_H264_PARAMS_VER. */
    NvU32 dwBufferIndex;                                                    /**< [in]: Index of the capture buffer to be used. */
    NvIFR_H264HWEncoder_EncodeParams * pNvIFR_H264HWEncoder_EncodeParams;   /**< [in]: Pointer to struct containing per-frame configuration parameters for the H264 HW Encoder. */
    NvU32 dwReserved[62];                                                   /**< [in]: Reserved. Set to 0. */
    void *pReserved[31];                                                    /**< [in]: Reserved. Set to NULL. */
} NVIFR_TRANSFER_RT_TO_H264_PARAMS;
#define NVIFR_TRANSFER_RT_TO_H264_PARAMS_VER NVIFR_STRUCT_VERSION(NVIFR_TRANSFER_RT_TO_H264_PARAMS, 1)

typedef struct
{
    NvU32 dwVersion;                                                        /**< [in]: Struct version. Set to NVIFR_TRANSFER_RT_TO_H264_PARAMS_VER. */
    NvU32 dwBufferIndex;                                                    /**< [in]: Index of the capture buffer to be used. */
    NvIFR_H264HWEncoder_FrameStats *pNvIFR_H264HWEncoder_FrameStats;        /**< [in/out]: Pointer to a client provided struct populated by the H264 HW Encoder with encoding stats. */
    NvU32 dwReserved[62];                                                   /**< [in]: Reserved. Set to 0. */
    void *pReserved[31];                                                    /**< [in]: Reserved. Set to NULL. */
} NVIFR_GET_H264_STATS_PARAMS;
#define NVIFR_GET_H264_STATS_PARAMS_VER NVIFR_STRUCT_VERSION(NVIFR_GET_H264_STATS_PARAMS, 1)

typedef struct
{
    NvU32 dwVersion;                                                        /**< [in]: Struct version. Set to NVIFR_GET_H264_HEADER_PARAMS_VER. */
    NvU32 dwReserved1;                                                      /**< [in]: Reserved. Set to 0. */
    NvU32 *pSize;                                                           /**< [out]: Size of SPS PPS header data generated by the H264 HW Encoder */
    NvU8  *pBuffer;                                                         /**< [out]: Output buffer containing SPS PPS header data generated by the H264 HW Encoder */
    NvU32 dwReserved[62];                                                   /**< [in]: Reserved. Set to 0. */
    void *pReserved[30];                                                    /**< [in]: Reserved. Set to NULL. */
} NVIFR_GET_H264_HEADER_PARAMS;
#define NVIFR_GET_H264_HEADER_PARAMS_VER NVIFR_STRUCT_VERSION(NVIFR_GET_H264_HEADER_PARAMS, 1)

class NvIFRToH264HWEncoder
{
public:
    // SetUpH264HWEncoder() call:
    // in parameter dwNBuffers is max 3
    // ppPageLockedBitStreamBuffers will receive dwNBuffers pointers, independently from the stereo mode
    // ppEncodeCompletionEvents will receive dwNBuffers event handles
    virtual NVIFRRESULT NvIFRSetUpH264HWEncoder(NVIFR_SETUP_H264_PARAMS *pParams)=0;

    // TransferRenderTargetToH264HWEncoder() and GetStatsFromH264HWEncoder calls:
    // in parameter dwBufferIndex is [0, dwNBuffers[
    virtual NVIFRRESULT NvIFRTransferRenderTargetToH264HWEncoder(NVIFR_TRANSFER_RT_TO_H264_PARAMS *pParams)=0;
    virtual NVIFRRESULT NvIFRGetStatsFromH264HWEncoder(NVIFR_GET_H264_STATS_PARAMS *pParams)=0;
    virtual NVIFRRESULT NvIFRGetHeaderFromH264HWEncoder(NVIFR_GET_H264_HEADER_PARAMS *pParams)=0;
    virtual NVIFRRESULT NvIFRRelease() = 0;
};

#endif // _NVIFR_H264_H_
