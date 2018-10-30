
// AdvertiseVideoSource.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "agora_log.h"

// CAdvertiseVideoSourceApp:
// See AdvertiseVideoSource.cpp for the implementation of this class
//

class CAdvertiseVideoSourceApp : public CWinApp
{
public:
	CAdvertiseVideoSourceApp();
	CString ss;
	std::string cmdParam;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAdvertiseVideoSourceApp theApp;