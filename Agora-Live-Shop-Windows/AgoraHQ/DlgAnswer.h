#pragma once


// CDlgAnswer 对话框
class CAgoraSignalInstance;
class CSingleCallBack;
class CAgoraHQDlg;
#include "AGButton.h"
#include "commonFun.h"
#include <map>

#include "DlgAnswerResultStatics.h"
#include "afxwin.h"
class CDlgAnswer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAnswer)

public:
	CDlgAnswer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAnswer();
	void updateStatusToPublish();

	void leaveChannel();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedCheckA();
	afx_msg void OnBnClickedCheckB();
	afx_msg void OnBnClickedCheckC();
	afx_msg void OnBnClickedCheckD();
	afx_msg void OnBnClickedButtonGetquestion();
//	afx_msg void OnBnClickedButtonStartMark();
	afx_msg void OnBnClickedButtonSetbonuses();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

/*	LRESULT onInputParam(WPARAM wParam, LPARAM lParam);
	LRESULT onSetDataTimeBonus(WPARAM wParam, LPARAM lParam);
	LRESULT onHttpPublish(WPARAM wParam, LPARAM lParam);
	LRESULT onRequestChannel(WPARAM wParam, LPARAM lParam);
	LRESULT onHttpStopAns(WPARAM wParam, LPARAM lParam);
	LRESULT onHttpReset(WPARAM wParam, LPARAM lParam);
	LRESULT onHttpInvite(WPARAM wParam, LPARAM lParam);
	LRESULT onHttpInviteStatus(WPARAM wParam, LPARAM lParam);*/
protected:
	void initCtrl();
	void uninitCtrl();

	void DrawClient();

	int getChannelName();
	void switchNewQuestion(const tagQuestionAnswer &newQuestion);
	void switchNewQuestion(int questionid, const CString questions, const std::vector<CString>& pOtions);
	void notifyQuestionAnswerStatics(const tagQuestionStatics &QuestionStatics);
	void notifyRoundListOfWinners(const std::vector<tagListOfWinners> &vecListOfWinner);

/*	bool error_info(WPARAM wParam, LPARAM lParam);
	bool ParseResponseJson(rapidjson::Document& document, LPARAM lParam, CString function_name);
*/
private:
	CString m_sQuestion;

private:
	std::string m_appId;
	std::string m_appCertificatId;
	std::string m_account;
	std::string m_serverAccount;
	bool m_isLogin;

	CStatic m_trlQuestion;
	CButton m_ckAnswerA;
	CButton m_ckAnswerB;
	CButton m_ckAnswerC;
	CButton m_ckAnswerD;

	CAGButton m_btnUpdateQuestion;
	//CAGButton m_btnStartAnswer;
	//CAGButton m_btnStopAnswer;
	CAGButton m_btnResetQuestion;
	CStatic m_ctlNoticeInfo;
	
	CDlgAnswerResultStatics m_DlgResult;
	int m_nQuestionId;
	CAGEngineEventHandler* m_pAgEngineEventHandle;
	CAgoraMetaDataObserver* m_metaDataObserver;

	CFileIO m_fileSigLog;

	int invite_status_count = 0;
	int invite_status_max = 30;
	int invite_status_interval = 1000;//1s 
	int invite_status_timer_event_id = 1001;
	std::map<std::string, CString> m_mapChn;
	//std::map<std::string, >
	int questionsCount = 0;
	int advertiseCount = 0;
public:
	CAgoraHQDlg* m_pAgoraHQDlg;
	afx_msg void OnBnClickedButtonSendproduct();
	CAGButton m_btnSendProduct;
};
