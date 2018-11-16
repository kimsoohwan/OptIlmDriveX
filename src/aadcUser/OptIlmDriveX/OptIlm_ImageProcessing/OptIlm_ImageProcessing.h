﻿/*********************************************************************
Copyright (c) 2018
Audi Autonomous Driving Cup. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3.  All advertising materials mentioning features or use of this software must display the following acknowledgement: ?This product includes software developed by the Audi AG and its contributors for Audi Autonomous Driving Cup.?
4.  Neither the name of Audi nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY AUDI AG AND CONTRIBUTORS AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL AUDI AG OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/
#include "aadc_structs.h"


#pragma once

//*************************************************************************************************
#define CID_COPENCVTEMPLATE_DATA_TRIGGERED_FILTER "OptIlm_ImageProcessing.filter.user.aadc.cid"

using namespace adtf_util;
using namespace ddl;
using namespace adtf::ucom;
using namespace adtf::base;
using namespace adtf::streaming;
using namespace adtf::mediadescription;
using namespace adtf::filter;
using namespace std;
using namespace cv;


/*! the main class of the open cv template. */
class cOptIlm_ImageProcessing : public cTriggerFunction
{
private:

    //properties
    adtf::base::property_variable<tBool>    m_processed_video = tTrue;
    adtf::base::property_variable<tBool>    m_debug_video = tFalse;

    //Pins
    /*! Reader of an InPin. video */
    cPinReader m_oReader;
    /*! Writer to an OutPin. video */
    cPinWriter m_oWriter;
    cPinWriter m_oImageDebug;

    //Stream Formats
        /*! The input format */
    adtf::streaming::tStreamImageFormat m_sImageFormat;

    /*! The clock */
    object_ptr<adtf::services::IReferenceClock> m_pClock;


    /*! A ddl laser scanner data identifier. */
    struct
    {
        tSize laneDetectMode;
        tSize parameter_k;
        tSize parameter_m;
        tSize parameter_b;
        tSize laneWidth;
        tSize leftOrRightLine;
        tSize adult_flag;
        tSize child_flag;
    } m_ddlLaneDataId;
    adtf::mediadescription::cSampleCodecFactory m_LaneDataStructSampleFactory;

    /*! Media Descriptions. */
    struct tSignalValueId
    {
        tSize timeStamp;
        tSize value;
    } m_ddlSignalValueId;
    adtf::mediadescription::cSampleCodecFactory m_SignalValueSampleFactory;

    /*! Reader of an InPin. Info */
    cPinReader m_oInput_steering_angle;
    cPinReader m_oInput_control_mode;




    /*! Writer to an OutPin. Info*/
    cPinWriter m_oLaneData;

    tTimeStamp m_lastSampleTime;
    tFloat64 f64SampleTime;

public:

    /*! Default constructor. */
    cOptIlm_ImageProcessing();


    /*! Destructor. */
    virtual ~cOptIlm_ImageProcessing() = default;

    /**
    * Overwrites the Configure
    * This is to Read Properties prepare your Trigger Function
    */
    tResult Configure() override;
    /**
    * Overwrites the Process
    * You need to implement the Reading and Writing of Samples within this function
    * MIND: Do Reading until the Readers queues are empty or use the IPinReader::GetLastSample()
    * This FUnction will be called if the Run() of the TriggerFunction was called.
    */
    tResult Process(tTimeStamp tmTimeOfTrigger) override;

    void CalculateVehiclePath(float steering);
    tResult TransmitLaneData(void);
    tResult GetSteeringAngle();
    tResult GetImageMode();



    tResult Transfer_YUV_to_YUY2(int Im_width, int Im_height, const cv::Mat& image);
    tResult Transfer_YUY2_to_YUV(int Im_width, int Im_height, cv::Mat& image);
    tResult Transfer_YUY2_to_BGR(int Im_width, int Im_height, cv::Mat& image);
    tResult ImageBufferDownsamplingBGR_to_YUY2(int Im_width, int Im_height, const cv::Mat& image, char type);
    tResult DrawDebugImage(int Im_width, int Im_height, cv::Mat& image);


    /*! The mutex */
    std::mutex m_oMutex;
};


//*************************************************************************************************
