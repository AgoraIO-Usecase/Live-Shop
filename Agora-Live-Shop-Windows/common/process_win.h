/*
* Copyright (c) 2018 Agora.io
* All rights reserved.
* Proprietary and Confidential -- Agora.io
*/

/*
*  Created by Wang Yongli, 2018
*/

#ifndef NODE_PROCESS_H
#define NODE_PROCESS_H

#include <functional>

class CProcessWinImpl
{
public:
	CProcessWinImpl(HANDLE hProcess, int pid);
	~CProcessWinImpl();
	/*
	* To mointor the process event, like if the process exit.
	*/
	void Monitor(std::function<void(CProcessWinImpl*)> callback);
	int GetProcessId();

	bool TerminateNodeProcess();

	/*
	* To create process according to the path and flag.
	*/
	static CProcessWinImpl* CreateVideoProcess(const char* path, const char** params, unsigned int flag = 0);

	static CProcessWinImpl* OpenVideoProcess(int pid);

	static int GetWinCurrentProcessId();

	/**
	* To destory the process.
	* @param terminate : If process is live, whether to terminate the process.
	*/
	static void DestroyProcess(CProcessWinImpl*, bool terminate = false);

	static bool getCurrentModuleFileName(std::string& path);
private:
	int m_pid;
	HANDLE m_hProcess;
};
#endif
