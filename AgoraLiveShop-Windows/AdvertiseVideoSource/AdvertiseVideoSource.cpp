
// AdvertiseVideoSource.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdvertiseVideoSource.h"
#include "AdvertiseVideoSourceDlg.h"
#include "process_win.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_CMD_LINE_LEN 1024
// CAdvertiseVideoSourceApp

BEGIN_MESSAGE_MAP(CAdvertiseVideoSourceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAdvertiseVideoSourceApp construction

void initLogService()
{
	std::string currentPath;
	CProcessWinImpl::getCurrentModuleFileName(currentPath);
	std::string logFile = currentPath + ".txt";
	startLogService(logFile.c_str());
}
CAdvertiseVideoSourceApp::CAdvertiseVideoSourceApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAdvertiseVideoSourceApp object

CAdvertiseVideoSourceApp theApp;


// CAdvertiseVideoSourceApp initialization

BOOL CAdvertiseVideoSourceApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	initLogService();

	CString strCmd = GetCommandLine();
	strCmd.TrimLeft();
	strCmd.TrimRight();
	int pos = strCmd.Find(_T(" "));
	if (pos > 0){
		strCmd = strCmd.Mid(pos + 1);
	}
	CHAR szParam[MAX_CMD_LINE_LEN] = { 0 };
	ss = strCmd;
	::WideCharToMultiByte(CP_ACP, 0, strCmd.GetBuffer(0), -1, szParam, MAX_CMD_LINE_LEN, NULL, NULL);
	cmdParam = szParam;

	CAdvertiseVideoSourceDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CAdvertiseVideoSourceApp::ExitInstance()
{
	stopLogService();
	return 0;
}

