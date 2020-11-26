// DlgAnswer.cpp : 实现文件
//

#include "stdafx.h"
#include "AgoraHQ.h"
#include "AgoraHQDlg.h"
#include "DlgAnswer.h"
#include "afxdialogex.h"
#include "generatorSignalToken.h"
#include "ProductAdvertise.h"
#include "Questions.h"
//using namespace HQ_LANG;
// CDlgAnswer 对话框
#define MAX_BONUS 10
IMPLEMENT_DYNAMIC(CDlgAnswer, CDialogEx)

CDlgAnswer::CDlgAnswer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAnswer::IDD, pParent),
	m_nQuestionId(-1),
	m_pAgEngineEventHandle(nullptr)
{
	std::string strLogPath = getSigSdkLogPath();
	m_fileSigLog.openLog(strLogPath);
}

CDlgAnswer::~CDlgAnswer()
{
//	AfxGetUrlService()->GetUrlCallback()->SetMsgReceiver(NULL);
	uninitCtrl();
	m_fileSigLog.close();
}

void CDlgAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QUESTION, m_trlQuestion);
	DDX_Control(pDX, IDC_CHECK_A, m_ckAnswerA);
	DDX_Control(pDX, IDC_CHECK_B, m_ckAnswerB);
	DDX_Control(pDX, IDC_CHECK_C, m_ckAnswerC);
	DDX_Control(pDX, IDC_CHECK_D, m_ckAnswerD);
	DDX_Control(pDX, IDC_BUTTON_GetQuestion, m_btnUpdateQuestion);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnResetQuestion);
	DDX_Control(pDX, IDC_STATIC_NotifyInfo, m_ctlNoticeInfo);
	DDX_Control(pDX, IDC_BUTTON_SENDPRODUCT, m_btnSendProduct);
}


BEGIN_MESSAGE_MAP(CDlgAnswer, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_A, &CDlgAnswer::OnBnClickedCheckA)
	ON_BN_CLICKED(IDC_CHECK_B, &CDlgAnswer::OnBnClickedCheckB)
	ON_BN_CLICKED(IDC_CHECK_C, &CDlgAnswer::OnBnClickedCheckC)
	ON_BN_CLICKED(IDC_CHECK_D, &CDlgAnswer::OnBnClickedCheckD)
	ON_BN_CLICKED(IDC_BUTTON_GetQuestion, &CDlgAnswer::OnBnClickedButtonGetquestion)
	ON_BN_CLICKED(IDC_BUTTON_SETBONUSES, &CDlgAnswer::OnBnClickedButtonSetbonuses)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgAnswer::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SENDPRODUCT, &CDlgAnswer::OnBnClickedButtonSendproduct)
END_MESSAGE_MAP()


// CDlgAnswer 消息处理程序
BOOL CDlgAnswer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//AfxGetUrlService()->GetUrlCallback()->SetMsgReceiver(m_hWnd);

	initCtrl();
	
	m_appId = gHQConfig.getAppId();
	m_appCertificatId = gHQConfig.getAppCertificateId();
	return TRUE;
}

void CDlgAnswer::initCtrl()
{
	m_account = gHQConfig.getSignalAccount();
	m_serverAccount = gHQConfig.getServerAccount();

	m_btnUpdateQuestion.EnableWindow(FALSE);
	m_btnUpdateQuestion.ShowWindow(SW_HIDE);
	m_btnResetQuestion.EnableWindow(FALSE);
	m_btnResetQuestion.ShowWindow(SW_HIDE);
	m_btnSendProduct.ShowWindow(SW_HIDE);
	m_btnSendProduct.EnableWindow(FALSE); 
	m_ckAnswerA.ShowWindow(SW_HIDE);
	m_ckAnswerB.ShowWindow(SW_HIDE);
	m_ckAnswerC.ShowWindow(SW_HIDE);
	m_ckAnswerD.ShowWindow(SW_HIDE);
	
	m_pAgEngineEventHandle = CAgoraObject::getEngineEventHandle();
	m_metaDataObserver = CAgoraObject::getAgoraMetaDataObserver();
	m_trlQuestion.SetWindowTextW(_T("Click the Join Channel button on the left to send the video to the Audience!"));//(_T("Welcome To AgoraHQ . Join a Channel to Start !"));

}

void CDlgAnswer::uninitCtrl()
{
	m_pAgEngineEventHandle = nullptr;
}

void CDlgAnswer::OnTimer(UINT_PTR nIDEvent)
{
	if (invite_status_timer_event_id == nIDEvent)
	{
		std::string gid = gHQConfig.getChannelName();
		//AfxGetUrlService()->invite_status(gid);
		if (++invite_status_count <= invite_status_max)
			SetTimer(invite_status_timer_event_id, invite_status_interval, NULL);
		else
			KillTimer(invite_status_timer_event_id);
	}
}
void CDlgAnswer::OnBnClickedCheckA()
{
	
	m_ckAnswerA.SetCheck(TRUE);
	bool bStatusA = m_ckAnswerA.GetCheck();
	if (bStatusA){
		m_ckAnswerB.SetCheck(FALSE);
		m_ckAnswerC.SetCheck(FALSE);
		m_ckAnswerD.SetCheck(FALSE);
	}
}


void CDlgAnswer::OnBnClickedCheckB()
{
		
	m_ckAnswerB.SetCheck(TRUE);
	bool bStatusB = m_ckAnswerB.GetCheck();
	if (bStatusB){
		m_ckAnswerA.SetCheck(FALSE);
		m_ckAnswerC.SetCheck(FALSE);
		m_ckAnswerD.SetCheck(FALSE);
	}
}


void CDlgAnswer::OnBnClickedCheckC()
{
	
	m_ckAnswerC.SetCheck(TRUE);
	bool bStatusC = m_ckAnswerC.GetCheck();
	if (bStatusC){
		m_ckAnswerA.SetCheck(FALSE);
		m_ckAnswerB.SetCheck(FALSE);
		m_ckAnswerD.SetCheck(FALSE);
	}
}


void CDlgAnswer::OnBnClickedCheckD()
{
	
	m_ckAnswerD.SetCheck(TRUE);
	bool bStatusD = m_ckAnswerD.GetCheck();
	if (bStatusD){
		m_ckAnswerA.SetCheck(FALSE);
		m_ckAnswerB.SetCheck(FALSE);
		m_ckAnswerC.SetCheck(FALSE);
	}
}

//publish_method
void CDlgAnswer::OnBnClickedButtonGetquestion()
{
	questionsCount = getQuestionsModule()->getQuestionsCount();
	if (questionsCount > 0){
		CQuestions question;
		getQuestionsModule()->next();
		bool bFind = getQuestionsModule()->getQuestions(question);
		std::string quesJson = getQuestionsModule()->getCurrentQuestionJson();
		if (!quesJson.empty())
			m_metaDataObserver->setSEI(quesJson);

		tagQuestionAnswer answer;
		answer.questionId = getQuestionsModule()->getCurrentIndex();
		answer.strQuestion = question.content;
		answer.vecQuestionAnswers = question.m_vecOptions;
		switchNewQuestion(answer);

		tagQuestionStatics questionStaticsTemp;
		if (bFind)
		{
			int nresult = getQuestionsModule()->getResult();
			std::vector<int> vecSpreads = question.m_vecSpread;

			std::map<std::string, int> mapSpreadTemp;
			std::string strAnswer; int nAnswerNum;
			for (size_t i = 0; i < vecSpreads.size(); i++){
				char option = 'A' + i;
				strAnswer = option;
				nAnswerNum = vecSpreads[i];
				mapSpreadTemp.insert(make_pair(strAnswer, nAnswerNum));
			}
			questionStaticsTemp.nresult = nresult;
			questionStaticsTemp.mapSpread = mapSpreadTemp;
			questionStaticsTemp.ncorrectNum = vecSpreads[nresult];
			questionStaticsTemp.nTotal = getQuestionsModule()->getQuestionsCount();
			questionStaticsTemp.nsid = m_nQuestionId;

			notifyQuestionAnswerStatics(questionStaticsTemp);

			std::vector<tagListOfWinners> vecWinnersTemp;
			int nIndex = 0;
			for (size_t i = 0; i < vecSpreads[nresult]; i++){
				tagListOfWinners winnerTemp;
				winnerTemp.nPlayerId = ++nIndex;
				winnerTemp.fPlayerBonus = (float)(rand() % MAX_BONUS);
				char szName[20] = { 0 };
				sprintf_s(szName, 20, "user%d", i);
				winnerTemp.strPlayerName = szName;
				vecWinnersTemp.push_back(winnerTemp);
			}
			notifyRoundListOfWinners(vecWinnersTemp);
			if (getQuestionsModule()->IsLastQuestion())
				m_btnUpdateQuestion.EnableScalar(FALSE);
		}
	}
	else if (questionsCount == 0){
		AfxMessageBox(_T("quetionsTable no data"));
	}
	m_ctlNoticeInfo.SetWindowTextW(_T(""));
}

//reset_method
void CDlgAnswer::OnBnClickedButtonReset()
{
	
	m_btnResetQuestion.EnableScalar(FALSE); 
	m_btnResetQuestion.EnableWindow(TRUE);
	m_btnUpdateQuestion.EnableScalar(TRUE);
	m_btnSendProduct.EnableScalar(TRUE);
	m_ctlNoticeInfo.SetWindowTextW(_T(""));
	
	tagQuestionAnswer answer;
	answer.questionId = -1;
	answer.strQuestion = "";
	answer.vecQuestionAnswers.clear();
	switchNewQuestion(answer);
	getQuestionsModule()->ResetQuestions();
	getProductAdModule()->ResetProductAdvertise();
	m_trlQuestion.SetWindowTextW(_T("please click the SendProduct button or SendQuestion button below to get the Question!"));
}

void CDlgAnswer::OnBnClickedButtonSetbonuses()
{
	
}

void CDlgAnswer::OnClose()
{
	
}

void CDlgAnswer::switchNewQuestion(int questionId, const CString questions, const std::vector<CString>& vecOptions)
{
	m_btnResetQuestion.ShowWindow(SW_SHOW);
	m_nQuestionId = questionId;
	m_trlQuestion.SetWindowTextW(questions);
	{
		m_ckAnswerA.ShowWindow(SW_HIDE);
		m_ckAnswerB.ShowWindow(SW_HIDE);
		m_ckAnswerC.ShowWindow(SW_HIDE);
		m_ckAnswerD.ShowWindow(SW_HIDE);

		m_ckAnswerA.SetCheck(FALSE);
		m_ckAnswerB.SetCheck(FALSE);
		m_ckAnswerC.SetCheck(FALSE);
		m_ckAnswerD.SetCheck(FALSE);
	}
	if (1 <= vecOptions.size()){
		m_ckAnswerA.ShowWindow(SW_SHOW);
		m_ckAnswerA.SetWindowTextW(vecOptions[0]);
	}
	if (2 <= vecOptions.size()){
		m_ckAnswerB.ShowWindow(SW_SHOW);
		m_ckAnswerB.SetWindowTextW(vecOptions[1]);
	}
	if (3 <= vecOptions.size()){
		m_ckAnswerC.ShowWindow(SW_SHOW);
		m_ckAnswerC.SetWindowTextW(vecOptions[2]);
	}
	if (4 == vecOptions.size()){
		m_ckAnswerD.ShowWindow(SW_SHOW);
		m_ckAnswerD.SetWindowTextW(vecOptions[3]);
	}
}

void CDlgAnswer::switchNewQuestion(const tagQuestionAnswer &newQuestion)
{
	m_btnResetQuestion.ShowWindow(SW_SHOW);
	m_nQuestionId = newQuestion.questionId;
	char chQuestionTitle[2048] = { '\0' };
	if (m_nQuestionId >= 0){
		sprintf_s(chQuestionTitle, "%d: %s", m_nQuestionId + 1, newQuestion.strQuestion.data());
		m_trlQuestion.SetWindowTextW(s2cs(chQuestionTitle));
	}
	{
		m_ckAnswerA.ShowWindow(SW_HIDE);
		m_ckAnswerB.ShowWindow(SW_HIDE);
		m_ckAnswerC.ShowWindow(SW_HIDE);
		m_ckAnswerD.ShowWindow(SW_HIDE);
	}
	if (1 <= newQuestion.vecQuestionAnswers.size()){
		m_ckAnswerA.ShowWindow(SW_SHOW);
		m_ckAnswerA.SetWindowTextW(s2cs(newQuestion.vecQuestionAnswers[0]));
	}
	if (2 <= newQuestion.vecQuestionAnswers.size()){
		m_ckAnswerB.ShowWindow(SW_SHOW);
		m_ckAnswerB.SetWindowTextW(s2cs(newQuestion.vecQuestionAnswers[1]));
	}
	if (3 <= newQuestion.vecQuestionAnswers.size()){
		m_ckAnswerC.ShowWindow(SW_SHOW);
		m_ckAnswerC.SetWindowTextW(s2cs(newQuestion.vecQuestionAnswers[2]));
	}
	if (4 == newQuestion.vecQuestionAnswers.size()){
		m_ckAnswerD.ShowWindow(SW_SHOW);
		m_ckAnswerD.SetWindowTextW(s2cs(newQuestion.vecQuestionAnswers[3]));
	}
}

void CDlgAnswer::notifyQuestionAnswerStatics(const tagQuestionStatics &QuestionStatics)
{
	CRect rect;
	CWnd* parentWnd = GetParent();
	GetClientRect(&rect);
	parentWnd->ClientToScreen(&rect);
	if (NULL == m_DlgResult.m_hWnd){

		m_DlgResult.Create(CDlgAnswerResultStatics::IDD);
	}
	rect.left = rect.left - 100;
	rect.bottom = rect.bottom - 50;
	//rect.right = rect.right + 100;
	
	if (1 == QuestionStatics.mapSpread.size()){

		switch (QuestionStatics.nresult)
		{
		case 0:m_ckAnswerA.SetCheck(TRUE); 
			break;
		case 1:m_ckAnswerA.SetCheck(FALSE);
			break;
		case 2:m_ckAnswerA.SetCheck(FALSE);
			break;
		case 3:m_ckAnswerA.SetCheck(FALSE);
			break;
		default:
			break;
		}
	}
	if (2 == QuestionStatics.mapSpread.size()){

		switch (QuestionStatics.nresult)
		{
		case 0:m_ckAnswerA.SetCheck(TRUE); m_ckAnswerB.SetCheck(FALSE); 
			break;
		case 1:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(TRUE); 
			break;
		case 2:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE);
			break;;
		case 3:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE);
			break;
		default:
			break;
		}
	}
	if (3 == QuestionStatics.mapSpread.size()){

		switch (QuestionStatics.nresult)
		{
		case 0:m_ckAnswerA.SetCheck(TRUE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(FALSE); 
			break;
		case 1:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(TRUE); m_ckAnswerC.SetCheck(FALSE); 
			break;
		case 2:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(TRUE); 
			break;;
		case 3:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(FALSE);
			break;
		default:
			break;
		}
	}
	if (4 == QuestionStatics.mapSpread.size()){

		switch (QuestionStatics.nresult)
		{
		case 0:m_ckAnswerA.SetCheck(TRUE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(FALSE); m_ckAnswerD.SetCheck(FALSE);
			break;
		case 1:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(TRUE); m_ckAnswerC.SetCheck(FALSE); m_ckAnswerD.SetCheck(FALSE);
			break;
		case 2:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(TRUE); m_ckAnswerD.SetCheck(FALSE);
			break;;
		case 3:m_ckAnswerA.SetCheck(FALSE); m_ckAnswerB.SetCheck(FALSE); m_ckAnswerC.SetCheck(FALSE); m_ckAnswerD.SetCheck(TRUE);
			break;
		default:
			break;
		}
	}

	m_DlgResult.ShowWindow(SW_SHOW);
	m_DlgResult.MoveWindow(&rect, TRUE);
	m_DlgResult.setContext(QuestionStatics);
}

void CDlgAnswer::notifyRoundListOfWinners(const std::vector<tagListOfWinners> &vecListOfWinner)
{
	CRect rect;
	CWnd* parentWnd = GetParent();
	GetClientRect(&rect);
	parentWnd->ClientToScreen(&rect);
	if (NULL == m_DlgResult.m_hWnd){

		m_DlgResult.Create(CDlgAnswerResultStatics::IDD);
	}
	rect.right = rect.right + 10;
	rect.bottom = rect.bottom - 50;
	
	m_btnUpdateQuestion.EnableScalar(TRUE);
	m_btnResetQuestion.EnableWindow(TRUE);

	m_DlgResult.ShowWindow(SW_SHOW);
	m_DlgResult.MoveWindow(&rect, TRUE);
	m_DlgResult.setContext(vecListOfWinner);
}

void CDlgAnswer::updateStatusToPublish()
{
	m_trlQuestion.SetWindowTextW(_T("Congratulations, you have joined the room channel, please click the SendQuestion button below to get the Question!"));
	m_btnUpdateQuestion.ShowWindow(SW_SHOW);
	m_btnUpdateQuestion.EnableScalar(TRUE);
	m_btnSendProduct.ShowWindow(SW_SHOW);
	m_btnSendProduct.EnableScalar(TRUE);

	m_btnResetQuestion.ShowWindow(SW_SHOW);
	m_btnResetQuestion.EnableWindow(TRUE);
    
}

void CDlgAnswer::leaveChannel()
{
	initCtrl();
}

void CDlgAnswer::OnBnClickedButtonSendproduct()
{
	advertiseCount = getProductAdModule()->getProductCount();
	if (advertiseCount > 0){
		getProductAdModule()->next();
		std::string advertiseJson = getProductAdModule()->getCurrentProductJson();
		if (!advertiseJson.empty())
			m_metaDataObserver->setSEI(advertiseJson);
		{
			m_trlQuestion.SetWindowText(_T(""));
			m_ckAnswerA.ShowWindow(SW_HIDE);
			m_ckAnswerB.ShowWindow(SW_HIDE);
			m_ckAnswerC.ShowWindow(SW_HIDE);
			m_ckAnswerD.ShowWindow(SW_HIDE);
		}

		if (getProductAdModule()->IsLastProduct())
			m_btnSendProduct.EnableScalar(FALSE);
	}
	else
		AfxMessageBox(_T("no data in productAdvertisementTabl"));
	
}