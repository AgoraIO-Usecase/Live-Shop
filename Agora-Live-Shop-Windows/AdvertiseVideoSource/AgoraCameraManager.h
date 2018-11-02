#pragma once
#include "IAgoraRtcEngine.h"
class CAgoraCameraManager
{
public:
	CAgoraCameraManager();
	~CAgoraCameraManager();

	BOOL Create(agora::rtc::IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(const char*);
	void TestCameraDevice(HWND hVideoWnd, BOOL bTestOn = TRUE);

	BOOL IsTesting() { return m_bTestingOn; };

private:
	agora::rtc::AVideoDeviceManager			*m_ptrDeviceManager;
	agora::rtc::IVideoDeviceCollection		*m_lpCollection;
	BOOL						m_bTestingOn;
};

