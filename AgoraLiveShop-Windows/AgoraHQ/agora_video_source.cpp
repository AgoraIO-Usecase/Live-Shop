/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/
#include "stdafx.h"
#include "agora_video_source.h"
#include "video_source_ipc.h"
#include "process_win.h"
#include "node_event.h"
#ifdef _WIN32
#include <Rpc.h>
#include <RpcDce.h>
#else
#include <uuid/uuid.h>
#endif
#include <thread>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>

#define DATA_IPC_NAME "avsipc"
#define PROCESS_RUN_EVENT_NAME "agora_video_source_process_ready_event_name"
namespace agora{
    namespace rtc{
        /**
         * AgoraVideoSource implementation
         * agoraVideoSource starts two thread, one for message transform, and one for video data transform.
         */

        class AgoraVideoSourceSink : public AgoraVideoSource, public AgoraIpcListener
        {
        public:
            AgoraVideoSourceSink();
            ~AgoraVideoSourceSink();

			virtual bool initialize(IAgoraVideoSourceEventHandler *eventHandler, HWND view, const char* appid, const char* token) override;
            virtual int join(const char* token, const char* cname, const char* chan_info, unsigned int uid) override;
            virtual int leave() override;
            virtual int release() override;
            virtual int renewVideoSourceToken(const char* token) override;
            virtual int setVideoSourceChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile, const char* permissionKey) override;
            virtual int setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight) override;
            virtual void onMessage(unsigned int msg, char* payload, unsigned int len) override;
            virtual int captureScreen(agora::rtc::WindowIDType id, int captureFreq, agora::rtc::Rect* rect, int bitrate) override;
            virtual int updateScreenCapture(agora::rtc::Rect* rect) override;
            virtual int stopCaptureScreen() override;
            virtual int startPreview() override;
            virtual int stopPreview() override;
            virtual int enableWebSdkInteroperability(bool enabled) override;
            virtual int enableDualStreamMode(bool enabled) override;
            virtual int setLogFile(const char* file) override;
            virtual void setParameters(const char* parameters) override;
			virtual int setCurrentCamera(const char* cameraid) override;
        private:
            void msgThread();
			static int deliverFrame(const char* payload, int len);
            void clear();

            void onStartPreviewComplete();

        private:
            std::thread m_msgThread;
            std::mutex m_lock;
            std::unique_ptr<IAgoraIpc> m_ipcMsg;
            std::unique_ptr<AgoraIpcDataReceiver> m_ipcReceiver;
            std::unique_ptr<CProcessWinImpl> m_sourceNodeProcess;
            //std::unique_ptr<NodeVideoRender> m_videoRender;
            std::string m_peerId;
            bool m_peerJoined;
            bool m_initialized;
            IAgoraVideoSourceEventHandler *m_eventHandler;
			HWND m_VideoSourceView;
            uid_t m_peerUid;
            NodeEvent m_event;
           // buffer_list m_buffers;
            std::vector<char> m_backBuf;

			const int ok = 0;
			const int api_status_generic_error = 1;
			const int api_status_invalid_args = 2;
			const int api_status_low_memory = 3;
			const int error = -4;
        private:
            static const unsigned int s_bufLen;
        };
        const unsigned int AgoraVideoSourceSink::s_bufLen = (const unsigned int)(2560 * 1600 * 1.5 + 1000);

        AgoraVideoSource* createVideoSource()
        {
            return new AgoraVideoSourceSink();
        }

        AgoraVideoSourceSink::AgoraVideoSourceSink()
            : m_msgThread()
            , m_initialized(false)
            , m_event(false)
        {
            m_backBuf.resize(AgoraVideoSourceSink::s_bufLen);
        }

        AgoraVideoSourceSink::~AgoraVideoSourceSink()
        {
            clear();
        }

        void AgoraVideoSourceSink::clear()
        {
            m_initialized = false;
            m_eventHandler = nullptr;
            m_ipcReceiver.reset();
            if (m_ipcMsg.get()) {
                m_ipcMsg->sendMessage(AGORA_IPC_DISCONNECT, nullptr, 0);
                m_ipcMsg->disconnect();
            }
            if(m_msgThread.joinable())
                m_msgThread.join();
        }

        int AgoraVideoSourceSink::release()
        {
            if (m_initialized){
                clear();
            }
			return error;
        }

		bool AgoraVideoSourceSink::initialize(IAgoraVideoSourceEventHandler *eventHandler, HWND view, const char* appid, const char* token)
        {
            if (m_initialized)
                return true;
            if (!appid)
                return false;
            clear();
            m_eventHandler = eventHandler;
			m_VideoSourceView = view;
#ifdef _WIN32
            UUID uuid = { 0 };
            RPC_CSTR struuid;

            if (UuidCreate(&uuid) != RPC_S_OK)
                return false;
            if (UuidToStringA(&uuid, &struuid) != RPC_S_OK)
                return false;
            m_peerId = (LPSTR)struuid;
            RpcStringFreeA(&struuid);
#else
            uuid_t uuid;
            uuid_generate(uuid);
            uuid_string_t uid = {0};
            uuid_unparse(uuid, uid);
            m_peerId = "/";
            m_peerId += uid;
            m_peerId = m_peerId.substr(0, 20);
#endif
            do {
                m_ipcMsg.reset(createAgoraIpc(this));
                if (!m_ipcMsg.get()) {
                    break;
                }

                if (!m_ipcMsg->initialize(m_peerId))
                    break;

                if (!m_ipcMsg->listen())
                    break;

                m_msgThread = std::thread(&AgoraVideoSourceSink::msgThread, this);
                std::string targetPath;
                if (!CProcessWinImpl::getCurrentModuleFileName(targetPath)) {
                    break;
                }

                size_t pos = targetPath.find_last_of("\\/");
                if (pos == targetPath.npos) {
                    break;
                }

                std::stringstream ss;
				ss << CProcessWinImpl::GetWinCurrentProcessId();
                std::string path = targetPath.substr(0, pos + 1);
                std::string cmdname = "AdvertiseVideoSource";
                std::string idparam = "id:" + m_peerId;
                std::string pidparam = "pid:" + ss.str();
                std::string appidparam = "appid:" + std::string(appid);
				std::string tokenparam = "token:" + std::string(token);
                const char* params[] = { cmdname.c_str(), idparam.c_str(), pidparam.c_str(), appidparam.c_str(), tokenparam.c_str(), nullptr };
                m_sourceNodeProcess.reset(CProcessWinImpl::CreateVideoProcess(path.c_str(), params));
                if (!m_sourceNodeProcess.get()) {
                    break;
                }

                NodeEvent::WaitResult result = m_event.WaitFor(5000);
                if (result != NodeEvent::WAIT_RESULT_SET) {
                    break;
                }

                m_sourceNodeProcess->Monitor([eventHandler](CProcessWinImpl* pProcess) {
                    eventHandler->onVideoSourceExit();
                });
             
                m_initialized = true;
                return true;
            } while (false);
            clear();
            return false;
		}	

        int AgoraVideoSourceSink::startPreview()
        {
			std::unique_ptr<LocalVideoCmd> cmd(new LocalVideoCmd());
			cmd->renderMode = agora::rtc::RENDER_MODE_FIT;
			cmd->view = m_VideoSourceView;
			return m_ipcMsg->sendMessage(AGORA_IPC_START_VS_PREVIEW, (char*)cmd.get(), sizeof(LocalVideoCmd)) ? ok : api_status_generic_error;
        }

        void AgoraVideoSourceSink::onStartPreviewComplete()
        {
            if (!m_initialized)
                return;
            m_ipcReceiver.reset(new AgoraIpcDataReceiver());
			if (!m_ipcReceiver->initialize(m_peerId + DATA_IPC_NAME/*, std::bind(&AgoraVideoSourceSink::deliverFrame, this, std::placeholders::_1, std::placeholders::_2)*/)) {
                m_ipcReceiver.reset();
                return;
            }
            m_ipcReceiver->run(true);
        }
		
        int AgoraVideoSourceSink::stopPreview()
        {
            if (!m_ipcReceiver)
				return error;

			return m_ipcMsg->sendMessage(AGORA_IPC_STOP_VS_PREVIEW, nullptr, 0) ? ok : api_status_generic_error;
        }
        
        int AgoraVideoSourceSink::enableWebSdkInteroperability(bool enabled)
        {
            if (m_initialized){
				return m_ipcMsg->sendMessage(AGORA_IPC_ENABLE_WEB_SDK_INTEROPERABILITY, (char*)&enabled, sizeof(enabled)) ? ok : api_status_generic_error;
            }
			return error;
        }
		
        int AgoraVideoSourceSink::enableDualStreamMode(bool enabled)
        {
            if (m_initialized){
				return m_ipcMsg->sendMessage(AGORA_IPC_ENABLE_DUAL_STREAM_MODE, (char*)&enabled, sizeof(enabled)) ? ok : api_status_generic_error;
            }
			return error;
        }
	
        int AgoraVideoSourceSink::setLogFile(const char* file)
        {
            if (m_initialized){
				return m_ipcMsg->sendMessage(AGORA_IPC_SET_LOGFILE, (char*)file, strlen(file)) ? ok : api_status_generic_error;
            }
			return error;
        }

        void AgoraVideoSourceSink::setParameters(const char* parameters)
        {
            if (!m_initialized)
                return;
            SetParameterCmd cmd;
            strncpy(cmd.parameters, parameters, MAX_PARAMETER_LEN);
            m_ipcMsg->sendMessage(AGORA_IPC_SET_PARAMETER, (char*)&cmd, sizeof(cmd));
        }

		int AgoraVideoSourceSink::setCurrentCamera(const char* cameraid)
		{
			std::unique_ptr<CameraIdCmd> cmd(new CameraIdCmd());
			if (cameraid){
				strncpy(cmd->cameraid, cameraid, agora::rtc::MAX_DEVICE_ID_LENGTH);
			}
			return m_ipcMsg->sendMessage(AGORA_IPC_SET_CAMERA_ID, (char*)cmd.get(), sizeof(CameraIdCmd)) ? ok : api_status_generic_error;
		}

        void AgoraVideoSourceSink::msgThread()
        {
            m_ipcMsg->run();
        }

        int AgoraVideoSourceSink::join(const char* token, const char* cname, const char* chan_info, uid_t uid)
        {
            if (m_initialized){
                m_peerUid = uid;
                std::unique_ptr<JoinChannelCmd> cmd(new JoinChannelCmd());
                if (token)
                    strncpy(cmd->token, token, MAX_TOKEN_LEN);
                if (cname) {
                    strncpy(cmd->cname, cname, MAX_CNAME_LEN);
                }
                if (chan_info)
                    strncpy(cmd->chan_info, chan_info, MAX_CHAN_INFO);
                cmd->uid = uid;
                return m_ipcMsg->sendMessage(AGORA_IPC_JOIN, (char*)cmd.get(), sizeof(JoinChannelCmd)) ? ok : api_status_generic_error;
            }
			return error;
        }

			
        int AgoraVideoSourceSink::leave()
        {
            if (m_initialized) {
                return m_ipcMsg->sendMessage(AGORA_IPC_LEAVE_CHANNEL, nullptr, 0) ? ok : api_status_generic_error;
            }
			return error;
        }

        int AgoraVideoSourceSink::renewVideoSourceToken(const char* token)
        {
            if (!token)
                return api_status_invalid_args;
            if (m_initialized) {
                return m_ipcMsg->sendMessage(AGORA_IPC_RENEW_TOKEN, (char*)token, strlen(token)) ? ok : api_status_generic_error;
            }
			return error;
        }
        
        int AgoraVideoSourceSink::setVideoSourceChannelProfile(agora::rtc::CHANNEL_PROFILE_TYPE profile, const char* permissionKey)
        {
            if (m_initialized){
                std::unique_ptr<ChannelProfileCmd> cmd(new ChannelProfileCmd());
				cmd->profile = profile;
#if defined(_WIN32)
                //if (permissionKey)
                //    strncpy(cmd->permissionKey, permissionKey, MAX_PERMISSION_KEY);
#endif
                return m_ipcMsg->sendMessage(AGORA_IPC_SET_CHANNEL_PROFILE, (char*)&profile, sizeof(profile)) ? ok : api_status_generic_error;
            }
			return error;
        }
        
        int AgoraVideoSourceSink::setVideoSourceVideoProfile(agora::rtc::VIDEO_PROFILE_TYPE profile, bool swapWidthAndHeight)
        {
            if (m_initialized){
                VideoProfileCmd cmd(profile, swapWidthAndHeight);
                return m_ipcMsg->sendMessage(AGORA_IPC_SET_VIDEO_RPOFILE, (char*)&cmd, sizeof(cmd)) ? ok : api_status_generic_error;
            }
			return error;
        }

        void AgoraVideoSourceSink::onMessage(unsigned int msg, char* payload, unsigned int len)
        {
           // LOG_INFO("Receive msg : %d\n", msg);
            if (msg == AGORA_IPC_SOURCE_READY) {
                m_event.notifyAll();
            }
            if (!m_initialized)
                return;
           if (msg == AGORA_IPC_JOIN_SUCCESS){
                m_peerJoined = true;
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceJoinedChannel(*((agora::rtc::uid_t*)payload));
                }
            }
            else if (msg == AGORA_IPC_LEAVE_CHANNEL){
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceLeaveChannel();
                }
            }
            else if (msg == AGORA_IPC_RENEW_TOKEN){
                if (m_eventHandler){
                    m_eventHandler->onVideoSourceRequestNewToken();
                }
            }
            else if (msg == AGORA_IPC_RENDER_READY){
                /* TBD */
            }
            else if (msg == AGORA_IPC_START_VS_PREVIEW_COMPLETE) {
                onStartPreviewComplete();
            }
            else if (msg == AGORA_IPC_STOP_VS_PREVIEW_COMPLETE) {
                m_ipcReceiver.reset();
            }
        }

        int AgoraVideoSourceSink::captureScreen(agora::rtc::WindowIDType id, int captureFreq, agora::rtc::Rect* rect, int bitrate)
        {
            if (m_initialized && m_peerJoined){
                CaptureScreenCmd cmd;
                cmd.windowid = id;
                cmd.captureFreq = captureFreq;
                if (rect){
                    cmd.rect = *rect;
                }
                cmd.bitrate = bitrate;
                return m_ipcMsg->sendMessage(AGORA_IPC_CAPTURE_SCREEN, (char*)&cmd, sizeof(cmd)) ? ok : api_status_generic_error;
            }
			return error;
        }

        int AgoraVideoSourceSink::updateScreenCapture(agora::rtc::Rect* rect)
        {
            if (!rect)
                return api_status_invalid_args;
            if (m_initialized && m_peerJoined){
                return m_ipcMsg->sendMessage(AGORA_IPC_UPDATE_CAPTURE_SCREEN, (char*)rect, sizeof(*rect)) ? ok : api_status_generic_error;
            }
			return error;
        }

        int AgoraVideoSourceSink::stopCaptureScreen()
        {
            if (m_initialized && m_peerJoined){
                return m_ipcMsg->sendMessage(AGORA_IPC_STOP_CAPTURE_SCREEN, nullptr, 0) ? ok : api_status_generic_error;
            }
			return error;
        }
    }
}
