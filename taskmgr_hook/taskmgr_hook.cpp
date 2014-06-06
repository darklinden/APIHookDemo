// taskmgr_hook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "taskmgr_hook.h"
#include "taskmgr_hook_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Ctaskmgr_hookApp

BEGIN_MESSAGE_MAP(Ctaskmgr_hookApp, CWinApp)
END_MESSAGE_MAP()


// Ctaskmgr_hookApp ����

Ctaskmgr_hookApp::Ctaskmgr_hookApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Ctaskmgr_hookApp ����

Ctaskmgr_hookApp theApp;

//////////////////////////////////////////////////////////////////////////
taskmgr_hook_Dlg * DllDlg = new taskmgr_hook_Dlg;
HHOOK g_messhook;
HANDLE hthread = NULL;
//////////////////////////////////////////////////////////////////////////

// Ctaskmgr_hookApp ��ʼ��

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
    // TODO: �ڴ����ר�ô����/����û���

    UnhookWindowsHookEx(g_messhook);
    TerminateThread(hthread,0);
    CloseHandle(hthread);
    CloseHandle(g_messhook);

    return CWinApp::ExitInstance();
}
