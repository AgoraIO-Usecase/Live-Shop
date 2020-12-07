/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#ifndef AGORA_VIDEO_SOURCE_SINK_H
#define AGORA_VIDEO_SOURCE_SINK_H

#include "IAgoraRtcEngine.h"
#include "AgoraBase.h"
#include <string>

namespace agora{
    namespace rtc{
		typedef HWND WindowIDType;

        /**
         * Video source need join the same channel, this class used to monitor video source's event.
         * More event maybe needed in future.
         */
        class IAgoraVideoSourceEventHandler
        {
        public:
            virtual ~IAgoraVideoSourceEventHandler(){}
            /**
             * Video source joined channel success event.
             * @param uid : video source's uid.
             */
            virtual void onVideoSourceJoinedChannel(agora::rtc::uid_t uid) = 0;

            /**
             * Video source request new token event.
             */
            virtual void onVideoSourceRequestNewToken() = 0;

            /**
             * Video source leaved channel event.
             */
            virtual void onVideoSourceLeaveChannel() = 0;

            virtual void onVideoSourceExit() = 0;
        };

        /**
         * This is video source stub interface.
         */
        class AgoraVideoSource
        {
        public:
            virtual ~AgoraVideoSource(){}

            /**
             * To initialize Video source.
             * @param eventHandler : video source event handler.
             */
			virtual bool initialize(IAgoraVideoSourceEventHandler *eventHandler, HWND hwnd, const char* appid, const char* token) = 0;

            /**
             * To ask video source to join channel with specified parameters.
             * @param token : token if it is enabled.
             * @param cname : channel name.
             * @param chan_info : channel information
             * @param uid : uid of video source.
             */
            virtual int join(const char* token, const char* cname, const char* chan_info, uid_t uid) = 0;

            /**
            * To ask video source to leave channel.
            */
            virtual int leave() = 0;

            /**
            * To start receive local video of video source.
            */
            virtual int startPreview() = 0;

            /**
            * To stop receive local video of video source.
            */
            virtual int stopPreview() = 0;

            /**
             * Release video source.
             */
            virtual int release() = 0;

            /**
             * To ask video source begin to share screen.
             * @param id : window id whose window will be shared. if the value is 0, then desktop is shared.
             * @param captureFreq : video frequency, 0-15.
             * @param rect : the shared area
             * @param bitrate : bitrate of video
             */
			//virtual int captureScreen(agora::rtc::WindowIDType id, int captureFreq, agora::rtc::Rectangle* rect, int bitrate) = 0;

            /**
             * To update shared window area
             * @param rect : updated area
             */
			//virtual int updateScreenCapture(agora::rtc::Rectangle* rect) = 0;

            /**
             * To stop screen share
             */
			virtual int stopCaptureScreen() = 0;

            /**
             * To renew video source's token.
             * @param token : new token
             */
			virtual int renewVideoSourceToken(const char* token) = 0;

            /**
             * To set video source channel profile
             * @param profile : video source's channel profile
             */
			virtual int setVideoSourceChannelProfile(agora::CHANNEL_PROFILE_TYPE profile, const char* permissionKey) = 0;

            /**
             * To set video source's video profile
             * @param profile : the video source's video profile
             * @param swapWidthAndHeight : whether adjust width and height
             */
			virtual int setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight) = 0;
            
            /**
             * Enable interoperability with the Agora Web SDK.
             * @param enabled : whether interoperability with the Agora Web SDK is enabled
             */
			virtual int enableWebSdkInteroperability(bool enabled) = 0;

            
            /**
             * Enable dual stream mode with the Agora Web SDK.
             * @param enabled : whether dual stream with the Agora Web SDK is enabled
             */
			virtual int enableDualStreamMode(bool enabled) = 0;

            /**
             * set log file path of videosource
             * @param file : filepath of log
             */
            virtual int setLogFile(const char* file) = 0;

            /**
            * To set parameters for video source.
            */
            virtual void setParameters(const char* parameters) = 0;
			/**
			 * set current camera
			 */
			virtual int setCurrentCamera(const char* cameraid) = 0;
        };

        /**
         * Video source may be has different implementation on different platforms. The API is used to generate video source.
         */
        AgoraVideoSource* createVideoSource();
    }
}

#endif
