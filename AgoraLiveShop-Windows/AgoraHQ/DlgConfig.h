#pragma once


// CDlgConfig 对话框

#include "AgoraCameraManager.h"
#include "afxwin.h"

class CAgoraHQDlg;
class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConfig();
	int getVideoIndex();
	CString getVideoParam();
	void saveCameraID();

	int getVideoSourceProfile();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConfigSave();
	afx_msg void OnBnClickedButtonConfigCancle();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	void initData();
	void initCtrl();
	void uninitCtrl();
	void EnableAdvertiseCtrl(BOOL bEnable);
private:
	CEdit m_edAppId;
	CEdit m_edAppcertificatId;
	CEdit m_edMediaUid;
	CEdit m_edChannelName;
	CEdit m_edLanguage;
	CComboBox m_comLanguage;
#if 0
	CComboBox m_comServerAccount;
#endif

	CButton m_btnSatrtPreview;
	CButton m_btnAppcertificateEnable;
	CComboBox m_comSolutionIndex;
	CComboBox m_comCamera;
	CStatic m_trlTestVideo;

	CAgoraCameraManager CameraManager; 
	IRtcEngine* pRtcEngine;

public:
	afx_msg void OnBnClickedButtonVideotest();
	afx_msg void OnSelchangeComboCamera();
	CEdit m_edtAdvertiseMediaUID;
	CComboBox m_cmbAdvertiseVideoProfile;
	CComboBox m_cmbAdvertiseCamera;
	afx_msg void OnSelchangeComboAdvertisecamera();

	CAgoraHQDlg* pAgoraHQDlg = NULL;

	UINT idEvent = 1000;
	std::string GetCurrentCamera();
	void SetCurrentCamera();
};
