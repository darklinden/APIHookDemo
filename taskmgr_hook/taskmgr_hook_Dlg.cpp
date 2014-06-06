// taskmgr_hook_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "taskmgr_hook.h"
#include "taskmgr_hook_Dlg.h"

#pragma comment(lib, "Kernel32.lib")

// taskmgr_hook_Dlg 对话框

IMPLEMENT_DYNAMIC(taskmgr_hook_Dlg, CDialog)

taskmgr_hook_Dlg::taskmgr_hook_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(taskmgr_hook_Dlg::IDD, pParent)
    , text_text(_T(""))
{

}

taskmgr_hook_Dlg::~taskmgr_hook_Dlg()
{
}

void taskmgr_hook_Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, text_text);
}


BEGIN_MESSAGE_MAP(taskmgr_hook_Dlg, CDialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON1, &taskmgr_hook_Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// taskmgr_hook_Dlg 消息处理程序

void taskmgr_hook_Dlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //CDialog::OnClose();
    this->ShowWindow(SW_HIDE);
}

void taskmgr_hook_Dlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    HMODULE hm = ::GetModuleHandleA("GDI32.dll");
    FARPROC fp = ::GetProcAddress(hm, "LineTo");

    char * s = new char [0x100];
    char * l = new char [0x100];
    void * p;

    ZeroMemory(s,0x100);
    sprintf_s(s,0x100,"GDI32.dll LineTo Addr: %x\r\n",(int)fp);
    
    text_text = s;
    UpdateData(FALSE);
    
    p = (void*)((int)fp - 2);
    ZeroMemory(s,100);
    memcpy_s( s , 0x100 , p , 0x100);

    for (int i = 0 ; i < 0x100 ; i++)
    {
        sprintf_s(l, 0x100, "%2x ", s[i]);
        text_text += l;
    }

    UpdateData(FALSE);

//////////////////////////////////////////////////////////////////////////

    //MessageBoxA(0,"hello","",0);
    //if( NULL != p) 
    //{ 
    //    DWORD                       dwOldProtect; 
    //    MEMORY_BASIC_INFORMATION    mbi; 
    //    ::VirtualQuery( p, &mbi, sizeof( MEMORY_BASIC_INFORMATION ) ); 
    //    ::VirtualProtect( p, 8, PAGE_READWRITE, &dwOldProtect );
    //    __asm
    //    {
    //        push eax
    //        mov eax, p
    //        mov byte ptr [eax],0xEB
    //        inc eax
    //        mov byte ptr [eax],0x3A
    //        //inc eax
    //        //mov byte ptr [eax],0x90
    //        //inc eax
    //        //mov byte ptr [eax],0x90
    //        pop eax
    //    }
    //    ::VirtualProtect( p, 8, dwOldProtect, 0 ); 
    //} 
    //MessageBoxA(0,"world","",0);

//////////////////////////////////////////////////////////////////////////

    //delete [] l;
    //delete [] s;
}

BOOL taskmgr_hook_Dlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化

    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
    {
        return FALSE;
    }
    LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
    tkp.PrivilegeCount=1;
    tkp.Privileges[0].Attributes= SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
    if(GetLastError()!=ERROR_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
