
// AdvertiseVideoSourceDlg.h : header file
//

#pragma once

#include "video_source.h"
// CAdvertiseVideoSourceDlg dialog
class CAdvertiseVideoSourceDlg : public CDialogEx
{
// Construction
public:
	CAdvertiseVideoSourceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADVERTISEVIDEOSOURCE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<AgoraVideoSource> videoSource;

	void InitAgoraVideoSource();
};
