// taskmgr_hook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "taskmgr_hook.h"
#include "taskmgr_hook_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Ctaskmgr_hookApp

BEGIN_MESSAGE_MAP(Ctaskmgr_hookApp, CWinApp)
END_MESSAGE_MAP()


// Ctaskmgr_hookApp 构造

Ctaskmgr_hookApp::Ctaskmgr_hookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Ctaskmgr_hookApp 对象

Ctaskmgr_hookApp theApp;

//////////////////////////////////////////////////////////////////////////
taskmgr_hook_Dlg * DllDlg = new taskmgr_hook_Dlg;
HHOOK g_messhook;
HANDLE hthread = NULL;
//////////////////////////////////////////////////////////////////////////

// Ctaskmgr_hookApp 初始化

BOOL Ctaskmgr_hookApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

DWORD WINAPI threadproc(LPVOID lparameter)
{
    DllDlg->DoModal();
    return 0;
}
LRESULT CALLBACK messprc(int nCodes,WPARAM wParam,LPARAM lParam)
{
    DWORD threadid,processid;
    MSG *pMsg=(MSG*)lParam;
    if(nCodes==HC_ACTION)
    {		
        if(!hthread)
        {
            hthread=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadproc,0,0,&threadid);
            return CallNextHookEx(g_messhook,nCodes,wParam,lParam);
        }		 
        if(pMsg->message==WM_KEYDOWN)
        {   
            if(pMsg->wParam==VK_HOME)
            {
                GetWindowThreadProcessId(GetActiveWindow(),&processid);
                if(processid==GetCurrentProcessId())
                {					
                    if(DllDlg->IsWindowVisible())
                        DllDlg->ShowWindow(SW_HIDE);
                    else
                        DllDlg->ShowWindow(SW_SHOW);
                }
            }
        }   
    }
    return CallNextHookEx(g_messhook,nCodes,wParam,lParam);
}
//////////////////////////////////////////////////////////////////////////

_declspec(dllexport) DWORD starthook(DWORD tagthread)
{
    DWORD threadid;
    threadid=tagthread;
    g_messhook = SetWindowsHookEx(WH_GETMESSAGE,messprc,theApp.m_hInstance,threadid);
    return (DWORD)g_messhook;	
}

int Ctaskmgr_hookApp::ExitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类

    UnhookWindowsHookEx(g_messhook);
    TerminateThread(hthread,0);
    CloseHandle(hthread);
    CloseHandle(g_messhook);

    return CWinApp::ExitInstance();
}
