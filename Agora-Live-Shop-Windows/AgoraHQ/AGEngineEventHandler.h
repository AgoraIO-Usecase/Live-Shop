#pragma once

using namespace agora::rtc;
#include "agora_video_source.h"
#include <unordered_map>

class CAgoraMetaDataObserver : public IMetadataObserver
{
public:
	void SetMsgReceiver(HWND hWnd) { m_hMainWnd = hWnd; }
	/*
		get max meta data size of byte.
	*/
	virtual int getMaxMetadataSize()override;
	/*
		The sender is ready to send MetadataThis callback method
		is triggered when the SDK is ready to receive and send Metadata.
		annotations:
		Make sure that the Metadata size passed in to this method does not
		exceed the value set in getMaxMetadataSize.
		parameter:
		metadata :Metadata that the user wants to send.
		return:
		True: send
		False: don't send
	*/
	virtual bool onReadyToSendMetadata(Metadata &metadata)override;
	/*
		The receiver has received Metadata.The SDK triggers the callback when it
		receives Metadata sent by the remote user.
		parameter:
		metadata:Received Metadata.
	*/
	virtual void onMetadataReceived(const Metadata &metadata)override;
	//set max meta data size.
	void SetMaxMetadataSize(int maxSize);
	//set send string.
	void SetSendSEI(std::string utf8Msg);
private:
	bool m_bActive;
	std::unordered_map<std::string, std::string> m_mapTypeToJson;
	int m_maxSize = 1024;
	std::string m_strSEI;
	HWND m_hMainWnd;
};
class CAGEngineEventHandler :
	public IRtcEngineEventHandler
{
public:
	CAGEngineEventHandler(void);
	~CAGEngineEventHandler(void);

	void SetMsgReceiver(HWND hWnd = NULL);
	HWND GetMsgReceiver() {return m_hMainWnd;};

	virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
	virtual void onRejoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
	virtual void onWarning(int warn, const char* msg);
	virtual void onError(int err, const char* msg);
	virtual void onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost);
	virtual void onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume);
	
	virtual void onLeaveChannel(const RtcStats& stat);
	virtual void onRtcStats(const RtcStats& stat);
	virtual void onMediaEngineEvent(int evt);

	virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState);
	virtual void onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState);

	virtual void onNetworkQuality(int quality);
	virtual void onFirstLocalVideoFrame(int width, int height, int elapsed);
	virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed);
	virtual void onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed);
	virtual void onUserJoined(uid_t uid, int elapsed);
	virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason);
	virtual void onUserMuteAudio(uid_t uid, bool muted);
	virtual void onUserMuteVideo(uid_t uid, bool muted);
	virtual void onApiCallExecuted(const char* api, int error);

	virtual void onLocalVideoStats(const LocalVideoStats& stats);
	virtual void onRemoteVideoStats(const RemoteVideoStats& stats);
	virtual void onCameraReady();
	virtual void onVideoStopped();
	virtual void onConnectionLost();
	virtual void onConnectionInterrupted();

	virtual void onUserEnableVideo(uid_t uid, bool enabled);

	virtual void onStartRecordingService(int error);
	virtual void onStopRecordingService(int error);
	virtual void onRefreshRecordingServiceStatus(int status);

private:
	HWND		m_hMainWnd;
};
//advertise process callback
class CAgoraVideoSourceEventHandler : public IAgoraVideoSourceEventHandler
{
	//advertise callback
	virtual void onVideoSourceJoinedChannel(agora::rtc::uid_t uid) override;
	virtual void onVideoSourceRequestNewToken() override;
	virtual void onVideoSourceLeaveChannel() override;
	virtual void onVideoSourceExit() override;
};

