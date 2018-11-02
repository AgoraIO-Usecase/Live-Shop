#include "stdafx.h"
#include "AgoraCameraManager.h"


CAgoraCameraManager::CAgoraCameraManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


CAgoraCameraManager::~CAgoraCameraManager()
{
	Close();
}

BOOL CAgoraCameraManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new agora::rtc::AVideoDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL) {
		AfxMessageBox(_T("")); 
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
		return FALSE;
	}

	m_lpCollection = (*m_ptrDeviceManager)->enumerateVideoDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraCameraManager::Close()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}
	m_bTestingOn = FALSE;
}

UINT CAgoraCameraManager::GetDeviceCount()
{
	if (m_lpCollection == NULL)
		return 0;

	return (UINT)m_lpCollection->getCount();
}

std::string CAgoraCameraManager::GetCurDeviceID()
{
	CString		str;
	CHAR		szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH];

	memset(szDeviceID, 0x00, agora::rtc::MAX_DEVICE_ID_LENGTH);
	if (m_ptrDeviceManager != NULL &&  m_lpCollection != NULL)
		(*m_ptrDeviceManager)->getDevice(szDeviceID);

	return szDeviceID;
}

BOOL CAgoraCameraManager::SetCurDevice(const char* lpDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL || nullptr == m_lpCollection)
		return FALSE;

	int nRet = (*m_ptrDeviceManager)->setDevice(lpDeviceID);
	return nRet == 0 ? TRUE : FALSE;
}

void CAgoraCameraManager::TestCameraDevice(HWND hVideoWnd, BOOL bTestOn)
{
	if (m_ptrDeviceManager == NULL || nullptr == m_lpCollection)
		return;

	if (bTestOn && !m_bTestingOn) {
		ASSERT(hVideoWnd != NULL);
		//CAgoraObject::GetAgoraObject()->LocalVideoPreview(hVideoWnd, TRUE);
	}
	else if(!bTestOn && m_bTestingOn){
		//CAgoraObject::GetAgoraObject()->LocalVideoPreview(NULL, FALSE);
	}

	m_bTestingOn = bTestOn;
}