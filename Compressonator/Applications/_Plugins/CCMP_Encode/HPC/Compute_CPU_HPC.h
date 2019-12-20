//=====================================================================
// Copyright (c) 2019    Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/// \file Compute_CPU_HPC.h
//
//=====================================================================

#ifndef H_COMPUTE_CPU_HPC
#define H_COMPUTE_CPU_HPC

#define __CL_ENABLE_EXCEPTIONS

//#include <CL/opencl.h>

#include "Compressonator.h"
//#include "crc32.h"

#include <thread>
#include <atomic>

#define MAX_ENCODER_THREADS 128

//typedef enum _CODECError
//{
//    CE_OK = 0,
//    CE_Unknown,
//    CE_Aborted,
//} CodecError;


struct ThreadParam
{
    CMP_Encoder        *cmp_encoder;
    void                *in;
    void                *out;
    int                 x;
    int                 y;
    std::atomic<bool>   run;
    std::atomic<bool>   exit;
};

using namespace CMP_Compute_Base;

class CCPU_HPC :public RenderWindow
{
public:
    CCPU_HPC(void *kerneloptions);
    ~CCPU_HPC();
    virtual CMP_ERROR   Compress(KernelOptions *Options, MipSet  &SrcTexture, MipSet  &destTexture,CMP_Feedback_Proc pFeedback);
    virtual void        SetComputeOptions(ComputeOptions *CLOptions);

private:
    // Encoders
     CMP_Encoder*  m_encoder[MAX_ENCODER_THREADS];

    // Thread Code
    ThreadParam*    m_EncodeParameterStorage;
    std::thread*    m_EncodingThreadHandle;
    CMP_BOOL        m_Use_MultiThreading;
    CMP_WORD        m_NumEncodingThreads;
    CMP_WORD        m_LastThread;
    CodecError      CreateEncoderThreadPool();
    void            DeleteEncoderThreadPool();
    void            FinishThreadEncoding();
    CodecError      EncodeThreadBlock(int x, int y, void *in, void *out);
	bool            m_ThreadCodecInitialized;
;
    // Image data
    std::string      m_source_file;
    CMP_FORMAT       m_current_format;
    KernelOptions   *m_kernel_options;
    size_t           m_destination_size;
    size_t           m_source_buffer_size;
    Source_Info      m_SourceInfo;
    unsigned char*   p_destination;
    CMP_UINT         m_width_in_blocks;
    CMP_UINT         m_height_in_blocks;
    CMP_Vec4uc*      m_psource;

    // Codec
    PluginInterface_Encoder* m_plugin_compute;

    // HPC Setup
    void Init();
};

#endif
